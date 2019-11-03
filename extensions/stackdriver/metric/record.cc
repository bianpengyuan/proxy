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

#include "extensions/stackdriver/metric/record.h"
#include "extensions/stackdriver/common/constants.h"
#include "extensions/stackdriver/metric/registry.h"

namespace Extensions {
namespace Stackdriver {
namespace Metric {

void record(bool is_outbound, const ::wasm::common::NodeInfo &local_node_info,
            const ::wasm::common::NodeInfo &peer_node_info,
            ::Wasm::Common::LogInfo &log_info) {
  if (is_outbound) {
    opencensus::stats::Record(
        {{clientRequestCountMeasure(), 1},
         {clientRequestBytesMeasure(), log_info.requestSize()},
         {clientResponseBytesMeasure(), log_info.responseSize()},
         {clientRoundtripLatenciesMeasure(),
          log_info.duration().nanos() /
              Stackdriver::Common::kNanosecondsPerMillisecond}},
        {{meshUIDKey(), local_node_info.mesh_id()},
         {requestOperationKey(), log_info.requestOperation()},
         {requestProtocolKey(), log_info.requestProtocol()},
         {serviceAuthenticationPolicyKey(),
          ::Wasm::Common::AuthenticationPolicyString(
              log_info.serviceAuthenticationPolicy())},
         {destinationServiceNameKey(), log_info.destinationServiceName()},
         {destinationServiceNamespaceKey(), peer_node_info.namespace_()},
         {destinationPortKey(), std::to_string(log_info.destinationPort())},
         {responseCodeKey(), std::to_string(log_info.responseCode())},
         {sourcePrincipalKey(), log_info.sourcePrincipal()},
         {sourceWorkloadNameKey(), local_node_info.workload_name()},
         {sourceWorkloadNamespaceKey(), local_node_info.namespace_()},
         {sourceOwnerKey(), local_node_info.owner()},
         {destinationPrincipalKey(), log_info.destinationPrincipal()},
         {destinationWorkloadNameKey(), peer_node_info.workload_name()},
         {destinationWorkloadNamespaceKey(), peer_node_info.namespace_()},
         {destinationOwnerKey(), peer_node_info.owner()}});
    return;
  }

  opencensus::stats::Record(
      {{serverRequestCountMeasure(), 1},
       {serverRequestBytesMeasure(), log_info.requestSize()},
       {serverResponseBytesMeasure(), log_info.responseSize()},
       {serverResponseLatenciesMeasure(),
        log_info.duration().nanos() /
            Stackdriver::Common::kNanosecondsPerMillisecond}},
      {{meshUIDKey(), local_node_info.mesh_id()},
       {requestOperationKey(), log_info.requestOperation()},
       {requestProtocolKey(), log_info.requestProtocol()},
       {serviceAuthenticationPolicyKey(),
        ::Wasm::Common::AuthenticationPolicyString(
            log_info.serviceAuthenticationPolicy())},
       {destinationServiceNameKey(), log_info.destinationServiceName()},
       {destinationServiceNamespaceKey(), local_node_info.namespace_()},
       {destinationPortKey(), std::to_string(log_info.destinationPort())},
       {responseCodeKey(), std::to_string(log_info.responseCode())},
       {sourcePrincipalKey(), log_info.sourcePrincipal()},
       {sourceWorkloadNameKey(), peer_node_info.workload_name()},
       {sourceWorkloadNamespaceKey(), peer_node_info.namespace_()},
       {sourceOwnerKey(), peer_node_info.owner()},
       {destinationPrincipalKey(), log_info.destinationPrincipal()},
       {destinationWorkloadNameKey(), local_node_info.workload_name()},
       {destinationWorkloadNamespaceKey(), local_node_info.namespace_()},
       {destinationOwnerKey(), local_node_info.owner()}});
}

}  // namespace Metric
}  // namespace Stackdriver
}  // namespace Extensions
