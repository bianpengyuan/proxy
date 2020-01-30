#include "extensions/stackdriver/stats/sink.h"
#include "extensions/stackdriver/config/v1alpha1/stackdriver_stats_sink_config.pb.h"

#include "opencensus/stats/measure.h"
#include "opencensus/stats/stats.h"
#include "opencensus/stats/tag_key.h"

#include <memory>

namespace Envoy {
namespace Extensions {
namespace StatSinks {
namespace Stackdriver {

namespace {

constexpr char kEnvoyListenerLDSUpdate[] =
    "envoy/lds_update";

opencensus::tags::TagKey listenerUpdateType() {
  static const auto key = opencensus::tags::TagKey::Register("type");
  return key;
}

opencensus::stats::MeasureInt64 envoyListenerLDSUpdateMeasure() {
  static const opencensus::stats::MeasureInt64 listener_updates =
      opencensus::stats::MeasureInt64::Register(
          kEnvoyListenerLDSUpdate, "listener updates", "1");
  return listener_updates;
}

}

Stats::SinkPtr StackdriverSinkFactory::createStatsSink(
    const Protobuf::Message& /*config*/, Server::Instance& /*server*/) {
  envoyListenerLDSUpdateMeasure();
  const opencensus::stats::ViewDescriptor listener_update_view =
      opencensus::stats::ViewDescriptor()
          .set_name(kEnvoyListenerLDSUpdate)
          .set_measure(kEnvoyListenerLDSUpdate)
          .set_aggregation(opencensus::stats::Aggregation::Count())
          .add_column(listenerUpdateType());
  opencensus::stats::View view(listener_update_view);
  listener_update_view.RegisterForExport();
  return std::make_unique<Stackdriver::StackdriverSink>();
}

ProtobufTypes::MessagePtr StackdriverSinkFactory::createEmptyConfigProto() {
  return std::make_unique<stackdriver::config::v1alpha1::StackdriverSink>();
}

std::string StackdriverSinkFactory::name() const {
  return "envoy.stackdriver_stats_sink";
}

StackdriverSink::StackdriverSink() {}
void StackdriverSink::flush(Stats::MetricSnapshot& snapshot) {
  for (const auto& counter : snapshot.counters()) {
    if (counter.counter_.get().used()) {
      if (counter.counter_.get().name() == "listener_manager.lds.update_attempt") {
        opencensus::stats::Record(
          {{envoyListenerLDSUpdateMeasure(), counter.delta_}},
          {{listenerUpdateType(), "attempt"}});
      }
    }
  }
}

/**
 * Static registration for the statsd sink factory. @see RegisterFactory.
 */
REGISTER_FACTORY(StackdriverSinkFactory,
                 Server::Configuration::StatsSinkFactory);

}  // namespace Stackdriver
}  // namespace StatSinks
}  // namespace Extensions
}  // namespace Envoy
