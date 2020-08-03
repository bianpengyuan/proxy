#include "src/envoy/extensions/wasm/opencensus/opencensus_context.h"

#include "opencensus/exporters/stats/stackdriver/stackdriver_exporter.h"

namespace Envoy {
namespace Extensions {
namespace Common {
namespace Wasm {
namespace Istio {

using ::istio::src::envoy::extensions::wasm::opencensus::MeasureSpec;
using proxy_wasm::WasmResult;
using proxy_wasm::Word;

OpencensusStatsContext* OpencensusStatsContext::stats_context_ = nullptr;

OpencensusStatsContext* OpencensusStatsContext::getInstance() {
  if (stats_context_ == nullptr) {
    stats_context_ = new OpencensusStatsContext();
  }
  return stats_context_;
}

uint32_t OpencensusStatsContext::registerTag(const absl::string_view tag_name) {
  absl::WriterMutexLock lock(&mutex_);
  auto iter = tag_tokens_.find(tag_name);
  if (iter != tag_tokens_.end()) {
    return iter->second;
  }
  uint32_t new_tag_token = tags_.size() + 1;
  static const auto tag_key = opencensus::tags::TagKey::Register(tag_name);
  tags_.insert({new_tag_token, tag_key});
  tag_tokens_.insert({tag_name, new_tag_token});
  return new_tag_token;
}

uint32_t OpencensusStatsContext::registerMeasure(const MeasureSpec& measure_spec) {
  absl::WriterMutexLock lock(&mutex_);
  uint32_t new_measure_token = int_measures_.size() + double_measures_.size() + 1;
  if (measure_spec.type() == MeasureSpec::INT) {
    auto iter = int_measure_tokens_.find(measure_spec.name());
    if (iter != int_measure_tokens_.end()) {
      return iter->second;
    }
    static const auto measure = opencensus::stats::MeasureInt64::Register(measure_spec.name(), "", "1");
    int_measures_.insert({new_measure_token, measure});
    int_measure_tokens_.insert({measure_spec.name(), new_measure_token});
  } else {
    auto iter = double_measure_tokens_.find(measure_spec.name());
    if (iter != double_measure_tokens_.end()) {
      return iter->second;
    }
    static const auto measure = opencensus::stats::MeasureDouble::Register(measure_spec.name(), "", "1");
    double_measures_.insert({new_measure_token, measure});
    double_measures_.insert({new_measure_token, measure});
    double_measure_tokens_.insert({measure_spec.name(), new_measure_token});
  }
  return new_measure_token;
}

}  // namespace Istio
}  // namespace Wasm
}  // namespace Common
}  // namespace Extensions
}  // namespace Envoy
