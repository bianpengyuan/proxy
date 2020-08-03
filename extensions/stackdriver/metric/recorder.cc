/* Copyright 2019 Istio Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "extensions/stackdriver/metric/recorder.h"

namespace Extensions {
namespace Stackdriver {
namespace Metric {

using proxy_wasm::WasmResult;
using proxy_wasm::null_plugin::istio_opencensus_register_tag;
using proxy_wasm::null_plugin::StringView;

void Recorder::registerTag(const std::string& tag_name) {
  uint32_t tag_id;
  if (istio_opencensus_register_tag(tag_name.data(), tag_name.size(),
                                    &tag_id) != WasmResult::Ok) {
    return;
  }
  tags_.insert({tag_name, tag_id});
}

void Recorder::registerMeasure(
    const ::istio::src::envoy::extensions::wasm::opencensus::MeasureSpec&
        measure_spec) {
  uint32_t measure_id;
  std::string measure_spec_str;
  measure_spec.SerializeToString(&measure_spec_str);

  if (istio_opencensus_register_tag(measure_spec_str.data(),
                                    measure_spec_str.size(),
                                    &measure_id) != WasmResult::Ok) {
    return;
  }
  measures_.insert({measure_spec.name(), measure_id});
}

void Recorder::registerView(
      const ::istio::src::envoy::extensions::wasm::opencensus::ViewSpec&
          view_spec) {
  uint32_t measure_id;
  std::string measure_spec_str;
  view_spec.SerializeToString(&measure_spec_str);

  if (istio_opencensus_register_tag(measure_spec_str.data(),
                                    measure_spec_str.size(),
                                    &measure_id) != WasmResult::Ok) {
    return;
  }
  measures_.insert({measure_spec.name(), measure_id});
}

void Recorder::registerStackdriverExporter(
    const ::istio::src::envoy::extensions::wasm::opencensus::StackdriverExporterSpec&
        sd_exporter_spec) {

}

}  // namespace Metric
}  // namespace Stackdriver
}  // namespace Extensions
