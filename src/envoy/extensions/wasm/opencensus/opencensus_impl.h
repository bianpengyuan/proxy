#pragma once

#include <memory>

#include "include/proxy-wasm/word.h"

namespace Envoy {
namespace Extensions {
namespace Common {
namespace Wasm {
namespace Istio {

proxy_wasm::Word istio_opencensus_register_tag(void *raw_context,
                                               proxy_wasm::Word tag_name_ptr,
                                               proxy_wasm::Word tag_name_size,
                                               proxy_wasm::Word tag_id_ptr);

proxy_wasm::Word istio_opencensus_register_measure(
    void *raw_context, proxy_wasm::Word measure_spec_ptr,
    proxy_wasm::Word measure_spec_size, proxy_wasm::Word measure_id_ptr);

proxy_wasm::Word istio_opencensus_register_view(void *raw_context,
                                                proxy_wasm::Word view_spec_ptr,
                                                proxy_wasm::Word view_spec_size,
                                                proxy_wasm::Word view_id_ptr);

proxy_wasm::Word istio_opencensus_register_stackdriver_stats_exporter(
    void *raw_context, proxy_wasm::Word stackdriver_exporter_spec_ptr,
    proxy_wasm::Word stackdriver_exporter_spec_size);

}  // namespace Istio
}  // namespace Wasm
}  // namespace Common
}  // namespace Extensions
}  // namespace Envoy
