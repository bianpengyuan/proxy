#include "src/envoy/extensions/wasm/opencensus/opencensus_impl.h"

#include <memory>

#include "include/proxy-wasm/word.h"
#include "src/envoy/extensions/wasm/context.h"
#include "src/envoy/extensions/wasm/opencensus/opencensus_context.h"
#include "src/envoy/extensions/wasm/opencensus/opencensus_spec.pb.h"

#define WASM_CONTEXT(_c)                                                      \
  static_cast<IstioContext *>(proxy_wasm::exports::ContextOrEffectiveContext( \
      static_cast<proxy_wasm::ContextBase *>((void)_c,                        \
                                             proxy_wasm::current_context_)))

namespace Envoy {
namespace Extensions {
namespace Common {
namespace Wasm {
namespace Istio {

using proxy_wasm::WasmResult;
using proxy_wasm::Word;

Word istio_opencensus_register_tag(void *raw_context, Word tag_name_ptr,
                                   Word tag_name_size, Word tag_id_ptr) {
  auto context = WASM_CONTEXT(raw_context);
  auto tag = context->wasmVm()->getMemory(tag_name_ptr, tag_name_size);
  if (!tag) {
    return WasmResult::InvalidMemoryAccess;
  }
  auto new_tag = OpencensusStatsContext::getInstance()->addTag(tag.value());
  if (!context->wasm()->setDatatype(tag_id_ptr, new_tag)) {
    return WasmResult::InvalidMemoryAccess;
  }
  return WasmResult::Ok;
}

Word istio_opencensus_register_measure(void *raw_context,
                                       proxy_wasm::Word measure_spec_ptr,
                                       proxy_wasm::Word measure_spec_size,
                                       proxy_wasm::Word measure_id_ptr) {
  auto context = WASM_CONTEXT(raw_context);
  auto measure_spec_view =
      context->wasmVm()->getMemory(measure_spec_ptr, measure_spec_size);
  if (!measure_spec_view) {
    return WasmResult::InvalidMemoryAccess;
  }

  ::istio::src::envoy::extensions::wasm::opencensus::MeasureSpec measure_spec;
  if (!measure_spec.ParseFromArray(measure_spec_view->data(),
                                   measure_spec_view->size())) {
    return WasmResult::BadArgument;
  }


  auto new_measure = OpencensusStatsContext::getInstance()->addMeasure(measure_spec);
  if (!context->wasm()->setDatatype(measure_id_ptr, new_measure)) {
    return WasmResult::InvalidMemoryAccess;
  }
  return WasmResult::Ok;
}

}  // namespace Istio
}  // namespace Wasm
}  // namespace Common
}  // namespace Extensions
}  // namespace Envoy
