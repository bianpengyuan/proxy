
/*
 * Extended ABI with Opencensus.
 */

#pragma once

extern "C" WasmResult istio_opencensus_register_tag(const char *tag_name_ptr,
                                                    size_t tag_name_size,
                                                    uint32_t *tag_id_ptr);

extern "C" WasmResult istio_opencensus_register_measure(
    const char *measure_spec_ptr, size_t measure_spec_size,
    uint32_t *measure_id_ptr);

extern "C" WasmResult istio_opencensus_register_view(const char *view_spec_ptr,
                                                     size_t view_spec_size,
                                                     const char **view_id_ptr,
                                                     size_t *view_id_size);

extern "C" WasmResult istio_opencensus_register_stackdriver_stats_exporter(
    const char *stackdriver_exporter_spec_ptr,
    size_t stackdriver_exporter_spec_size);

// proxy_wasm::Word increment_integer_measurement(void* raw_context, uint32_t
// measure_token, proxy_wasm::Word tags_ptr, proxy_wasm::Word tags_size, int64_t
// val);

// proxy_wasm::Word increment_double_measurement(void* raw_context, uint32_t
// measure_token, proxy_wasm::Word tags_ptr, proxy_wasm::Word tags_size, double
// val);

// proxy_wasm::Word record_integer_measurement(void* raw_context, uint32_t
// measure_token, proxy_wasm::Word tags_ptr, proxy_wasm::Word tags_size, int64_t
// val);

// proxy_wasm::Word record_double_measurement(void* raw_context, uint32_t
// measure_token, proxy_wasm::Word tags_ptr, proxy_wasm::Word tags_size, double
// val);
