#include "include/proxy-wasm/wasm.h"
#include "absl/container/flat_hash_map.h"
#include "opencensus/stats/measure.h"
#include "opencensus/stats/stats.h"
#include "opencensus/stats/tag_key.h"
#include "src/envoy/extensions/wasm/opencensus/opencensus_spec.pb.h"

namespace Envoy {
namespace Extensions {
namespace Common {
namespace Wasm {
namespace Istio {

class OpencensusStatsContext {
 public:
  static OpencensusStatsContext* getInstance();

  uint32_t registerTag(const absl::string_view tag_name) ABSL_LOCKS_EXCLUDED(mutex_);
  uint32_t registerMeasure(const ::istio::src::envoy::extensions::wasm::opencensus::MeasureSpec& measure_spec) ABSL_LOCKS_EXCLUDED(mutex_);
  uint32_t registerView(const ::istio::src::envoy::extensions::wasm::opencensus::MeasureSpec& measure_spec) ABSL_LOCKS_EXCLUDED(mutex_);
  uint32_t registerStackdriverExporter(const ::istio::src::envoy::extensions::wasm::opencensus::MeasureSpec& measure_spec) ABSL_LOCKS_EXCLUDED(mutex_);

 private:
  OpencensusStatsContext() {}
  static OpencensusStatsContext* stats_context_;

  absl::Mutex mutex_;
  // Tags
  absl::flat_hash_map<uint32_t, const opencensus::tags::TagKey> tags_ ABSL_GUARDED_BY(mutex_);
  absl::flat_hash_map<absl::string_view, uint32_t> tag_tokens_ ABSL_GUARDED_BY(mutex_);

  // Measures
  absl::flat_hash_map<uint32_t, const opencensus::stats::MeasureInt64> int_measures_ ABSL_GUARDED_BY(mutex_);
  absl::flat_hash_map<absl::string_view, uint32_t> int_measure_tokens_ ABSL_GUARDED_BY(mutex_);
  absl::flat_hash_map<uint32_t, const opencensus::stats::MeasureDouble> double_measures_ ABSL_GUARDED_BY(mutex_);
  absl::flat_hash_map<absl::string_view, uint32_t> double_measure_tokens_ ABSL_GUARDED_BY(mutex_);
//   absl::flat_hash_map<uint32_t, opencensus::stats::View> views ABSL_GUARDED_BY(mutex_);
};

}  // namespace Istio
}  // namespace Wasm
}  // namespace Common
}  // namespace Extensions
}  // namespace Envoy
