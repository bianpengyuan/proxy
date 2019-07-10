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

#include <google/protobuf/util/json_util.h>
#include <random>
#include <string>
#include <unordered_map>

#include "extensions/stackdriver/stackdriver.h"

#ifndef NULL_PLUGIN
#include "api/wasm/cpp/proxy_wasm_intrinsics.h"
#else

#include "extensions/common/wasm/null/null.h"

using namespace opencensus::exporters::stats;

namespace Envoy {
namespace Extensions {
namespace Common {
namespace Wasm {
namespace Null {
namespace Plugin {
#endif

namespace Stackdriver {

constexpr char kStackdriverExporter[] = "stackdriver_exporter";
constexpr char kExporterRegistered[] = "registered";

// NodeMetadata key is the key in the node metadata struct that is passed
// between peers.
constexpr char kNodeMetadataKey[] = "istio.io/metadata";

void StackdriverRootContext::onConfigure(
    std::unique_ptr<WasmData> configuration) {
  // Parse configuration JSON string.
  google::protobuf::util::JsonParseOptions json_options;
  google::protobuf::util::Status status =
      google::protobuf::util::JsonStringToMessage(configuration->toString(),
                                                  &config_, json_options);

  // Initialize metric recorder based on reporter kind.
  recorder_ = std::make_unique<::Stackdriver::Metric::Recorder>(config_.kind());

  // Get node metadata.
  auto metadata =
      getMetadataValue(Common::Wasm::MetadataType::Node, kNodeMetadataKey);
  ::Stackdriver::Common::ExtractNodeMetadata(metadata, &local_node_info_);

  // Register OC Stackdriver exporter and views to be exported.
  // Note exporter and views are global singleton so they should only be
  // registered once.
  auto registered = getSharedData(kStackdriverExporter);
  if (!registered->view().empty()) {
    // Exporter and view should only be registered once.
    return;
  }
  setSharedData(kStackdriverExporter, kExporterRegistered);

  opencensus::exporters::stats::StackdriverExporter::Register(
      ::Stackdriver::Metric::GetStackdriverOptions(local_node_info_));
  ::Stackdriver::Metric::RegisterViews();
}

void StackdriverRootContext::onStart() {
#ifndef NULL_PLUGIN
// TODO: Start a timer to trigger exporting
#endif
}

void StackdriverRootContext::onTick() {
#ifndef NULL_PLUGIN
// TODO: Add exporting logic with WASM gRPC API
#endif
}

void StackdriverRootContext::Record(
    const ::Stackdriver::Common::RequestInfo &request_info) {
  recorder_->Record(local_node_info_, request_info);
}

void StackdriverContext::onCreate() {
  request_info_.start_timestamp = proxy_getCurrentTimeNanoseconds();
}

FilterHeadersStatus StackdriverContext::onRequestHeaders() {
  request_info_.request_protocol = getProtocol(StreamType::Request)->toString();
  request_info_.destination_service_host =
      getHeaderMapValue(HeaderMapType::RequestHeaders, ":authority")
          ->toString();
  request_info_.request_operation = 
      getHeaderMapValue(HeaderMapType::RequestHeaders, ":method")
          ->toString();
  // TODO: If this is an inbound listener, get peer node metadata
  return FilterHeadersStatus::Continue;
}

FilterDataStatus StackdriverContext::onRequestBody(size_t body_buffer_length,
                                                   bool) {
  request_info_.request_size += body_buffer_length;
  return FilterDataStatus::Continue;
}

FilterHeadersStatus StackdriverContext::onResponseHeaders() {
  // TODO: If this is an outbound listener, get peer node metadata
  return FilterHeadersStatus::Continue;
}

FilterDataStatus StackdriverContext::onResponseBody(size_t body_buffer_length,
                                                    bool) {
  request_info_.response_size += body_buffer_length;
  return FilterDataStatus::Continue;
}

void StackdriverContext::onDelete() {
  request_info_.end_timestamp = proxy_getCurrentTimeNanoseconds();
}

void StackdriverContext::onLog() { GetRootContext()->Record(request_info_); }

StackdriverRootContext *StackdriverContext::GetRootContext() {
  RootContext *root = this->root();
  return static_cast<StackdriverRootContext *>(root);
}

} // namespace Stackdriver

#ifdef NULL_PLUGIN
} // namespace Plugin
} // namespace Null
} // namespace Wasm
} // namespace Common
} // namespace Extensions
} // namespace Envoy
#endif
