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

#include "extensions/stackdriver/metric/registry.h"
#include "gtest/gtest.h"

namespace Stackdriver {
namespace Metric {

TEST(RegistryTest, GetStackdriverOptions) {
  Stackdriver::Common::NodeInfo node_info;
  node_info.name = "pod_name";
  node_info.namespace_name = "pod_namespace_name";
  node_info.project_id = "project_id";
  node_info.cluster_name = "test_cluster";
  node_info.location = "test_zone";

  auto option = GetStackdriverOptions(node_info);
  EXPECT_EQ(option.project_id, node_info.project_id);
}

}  // namespace Metric
}  // namespace Stackdriver
