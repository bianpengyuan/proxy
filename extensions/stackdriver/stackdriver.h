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

#pragma once

#include "extensions/stackdriver/common/context.h"
#include "extensions/stackdriver/config/stackdriver_plugin_config.pb.h"
#include "extensions/stackdriver/metric/recorder.h"

#ifndef NULL_PLUGIN
#include "api/wasm/cpp/proxy_wasm_intrinsics.h"
#else

#include "extensions/common/wasm/null/null.h"

namespace Envoy {
namespace Extensions {
namespace Common {
namespace Wasm {
namespace Null {
namespace Plugin {
#endif

namespace Stackdriver {

#ifdef NULL_PLUGIN
NULL_PLUGIN_ROOT_REGISTRY;
#endif

// StackdriverRootContext is the root context for all streams processed by the
// thread. It has the same lifetime as the worker thread and acts as target for
// interactions that outlives individual stream, e.g. timer, async calls.
class StackdriverRootContext : public RootContext {
 public:
  StackdriverRootContext(uint32_t id, StringView root_id)
      : RootContext(id, root_id) {}
  ~StackdriverRootContext() = default;

  void onConfigure(std::unique_ptr<WasmData> configuration) override;
  void onStart() override;
  void onTick() override;

  // Record metrics based on the given request info
  void Record(const ::Stackdriver::Common::RequestInfo &request_info);

 private:
  // Config for Stackdriver plugin.
  stackdriver::config::PluginConfig config_;

  // Local node info extracted from node metadata.
  ::Stackdriver::Common::NodeInfo local_node_info_;

  // Recorder records metrics based on local and peer node information and
  // request information.
  std::unique_ptr<::Stackdriver::Metric::Recorder> recorder_;
};

// StackdriverContext is per stream context. It has the same lifetime as
// the request stream itself.
class StackdriverContext : public Context {
 public:
  StackdriverContext(uint32_t id, RootContext *root) : Context(id, root) {}

  void onLog() override;

  void onCreate() override;
  void onDelete() override;

  FilterHeadersStatus onRequestHeaders() override;

  FilterDataStatus onRequestBody(size_t body_buffer_length,
                                 bool end_of_stream) override;

  FilterHeadersStatus onResponseHeaders() override;

  FilterDataStatus onResponseBody(size_t body_buffer_length,
                                  bool end_of_stream) override;

 private:
  // Request information collected from stream callbacks, used when record
  // metrics and access logs.
  ::Stackdriver::Common::RequestInfo request_info_;

  // Gets root context that this request context associated with.
  StackdriverRootContext *GetRootContext();
};

static RegisterContextFactory register_StackdriverContext(
    CONTEXT_FACTORY(StackdriverContext), ROOT_FACTORY(StackdriverRootContext));

}  // namespace Stackdriver

#ifdef NULL_PLUGIN
}  // namespace Plugin
}  // namespace Null
}  // namespace Wasm
}  // namespace Common
}  // namespace Extensions
}  // namespace Envoy
#endif
