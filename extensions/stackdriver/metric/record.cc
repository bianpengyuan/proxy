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

constexpr char kMutualTLS[] = "MUTUAL_TLS";
constexpr char kNone[] = "NONE";

void record(bool is_outbound, const ::wasm::common::NodeInfo &local_node_info,
            const ::wasm::common::NodeInfo &peer_node_info,
            ::Wasm::Common::RequestInfo &request_info) {
  if (is_outbound) {
    opencensus::stats::Record(
        {{clientRequestCountMeasure(), 1},
         {clientRequestBytesMeasure(), request_info.requestSize()},
         {clientResponseBytesMeasure(), request_info.responseSize()},
         {clientRoundtripLatenciesMeasure(),  
            request_info.duration().nanos() / Stackdriver::Common::kNanosecondsPerMillisecond}},
        {{meshUIDKey(), local_node_info.mesh_id()},
         {requestOperationKey(), request_info.requestOperation()},
         {requestProtocolKey(), request_info.requestProtocol()},
         {serviceAuthenticationPolicyKey(),
          request_info.mTLS() ? kMutualTLS : kNone},
         {destinationServiceNameKey(), request_info.destinationServiceHost()},
         {destinationServiceNamespaceKey(), peer_node_info.namespace_()},
         {destinationPortKey(), std::to_string(request_info.destinationPort())},
         {responseCodeKey(), std::to_string(request_info.responseCode())},
         {sourcePrincipalKey(), request_info.sourcePrincipal()},
         {sourceWorkloadNameKey(), local_node_info.workload_name()},
         {sourceWorkloadNamespaceKey(), local_node_info.namespace_()},
         {sourceOwnerKey(), local_node_info.owner()},
         {destinationPrincipalKey(), request_info.destinationPrincipal()},
         {destinationWorkloadNameKey(), peer_node_info.workload_name()},
         {destinationWorkloadNamespaceKey(), peer_node_info.namespace_()},
         {destinationOwnerKey(), peer_node_info.owner()}});
    return;
  }

  opencensus::stats::Record(
      {{serverRequestCountMeasure(), 1},
       {serverRequestBytesMeasure(), request_info.requestSize()},
       {serverResponseBytesMeasure(), request_info.responseSize()},
       {serverResponseLatenciesMeasure(),  
         request_info.duration().nanos() / Stackdriver::Common::kNanosecondsPerMillisecond}},
      {{meshUIDKey(), local_node_info.mesh_id()},
       {requestOperationKey(), request_info.requestOperation()},
       {requestProtocolKey(), request_info.requestProtocol()},
       {serviceAuthenticationPolicyKey(),
        request_info.mTLS() ? kMutualTLS : kNone},
       {destinationServiceNameKey(), request_info.destinationServiceHost()},
       {destinationServiceNamespaceKey(), local_node_info.namespace_()},
       {destinationPortKey(), std::to_string(request_info.destinationPort())},
       {responseCodeKey(), std::to_string(request_info.responseCode())},
       {sourcePrincipalKey(), request_info.sourcePrincipal()},
       {sourceWorkloadNameKey(), peer_node_info.workload_name()},
       {sourceWorkloadNamespaceKey(), peer_node_info.namespace_()},
       {sourceOwnerKey(), peer_node_info.owner()},
       {destinationPrincipalKey(), request_info.destinationPrincipal()},
       {destinationWorkloadNameKey(), local_node_info.workload_name()},
       {destinationWorkloadNamespaceKey(), local_node_info.namespace_()},
       {destinationOwnerKey(), local_node_info.owner()}});
}

}  // namespace Metric
}  // namespace Stackdriver
}  // namespace Extensions
