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
#include "extensions/stackdriver/metric/registry.h"

namespace Stackdriver {
namespace Metric {

class Recorder {
 public:
  Recorder(stackdriver::config::PluginConfig::ReporterKind kind) {
    kind_ = kind;
  }
  ~Recorder() = default;

  void Record(const ::Stackdriver::Common::NodeInfo &local_node_info,
              const ::Stackdriver::Common::RequestInfo &request_info);

 private:
  stackdriver::config::PluginConfig::ReporterKind kind_;
};

}  // namespace Metric
}  // namespace Stackdriver
