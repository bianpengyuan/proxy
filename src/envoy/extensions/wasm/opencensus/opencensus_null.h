#pragma once

#include "include/proxy-wasm/wasm.h"
#include "include/proxy-wasm/null_plugin.h"
#include "include/proxy-wasm/compat.h"
#include "include/proxy-wasm/exports.h"
#include "include/proxy-wasm/wasm_api_impl.h"
#include "src/envoy/extensions/wasm/opencensus/opencensus_impl.h"

namespace proxy_wasm {
namespace null_plugin {

#define WS(_x) Word(static_cast<uint64_t>(_x))
#define WR(_x) Word(reinterpret_cast<uint64_t>(_x))

inline WasmResult istio_opencensus_register_tag(const char *tag_name_ptr,
                                                size_t tag_name_size,
                                                uint32_t *tag_id_ptr) {
  return wordToWasmResult(
      ::Envoy::Extensions::Common::Wasm::Istio::istio_opencensus_register_tag(
          current_context_, WR(tag_name_ptr), WS(tag_name_size),
          WR(tag_id_ptr)));
}

inline WasmResult istio_opencensus_register_measure(
    const char *measure_spec_ptr, size_t measure_spec_size,
    uint32_t *measure_id_ptr) {
  return wordToWasmResult(::Envoy::Extensions::Common::Wasm::Istio::
                              istio_opencensus_register_measure(
                                  current_context_, WR(measure_spec_ptr),
                                  WS(measure_spec_size), WR(measure_id_ptr)));
}

}  // namespace null_plugin
}  // namespace proxy_wasm
