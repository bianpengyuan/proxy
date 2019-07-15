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

namespace Stackdriver {
namespace Metric {

constexpr double kNanosecondsPerMillisecond = 1000000.0;
constexpr char kMutualTLS[] = "MUTUAL_TLS";
constexpr char kNone[] = "NONE";

void Recorder::Record(const ::Stackdriver::Common::NodeInfo &local_node_info,
                      const ::Stackdriver::Common::RequestInfo &request_info) {
  double latency_ms =
      (request_info.end_timestamp - request_info.start_timestamp) /
      kNanosecondsPerMillisecond;
  if (kind_ == stackdriver::config::PluginConfig::ReporterKind::
                   PluginConfig_ReporterKind_INBOUND) {
    opencensus::stats::Record(
        {{ServerRequestCountMeasure(), 1},
         {ServerRequestBytesMeasure(), request_info.request_size},
         {ServerResponseBytesMeasure(), request_info.response_size},
         {ServerResponseLatenciesMeasure(), latency_ms}},
        {{RequestOperationKey(), request_info.request_operation},
         {RequestProtocolKey(), request_info.request_protocol},
         {ServiceAuthenticationPolicyKey(),
          request_info.mTLS ? kMutualTLS : kNone},
         {DestinationServiceNameKey(), request_info.destination_service_host},
         {DestinationServiceNamespaceKey(), local_node_info.namespace_name},
         {DestinationPortKey(), std::to_string(request_info.destination_port)},
         {ResponseCodeKey(), std::to_string(request_info.response_code)},
         {SourcePrincipalKey(), request_info.source_principal},
         {SourceWorkloadNameKey(), request_info.peer_node_info.workload_name},
         {SourceWorkloadNamespaceKey(),
          request_info.peer_node_info.namespace_name},
         {SourceOwnerKey(), request_info.peer_node_info.owner},
         {DestinationPrincipalKey(), request_info.destination_principal},
         {DestinationWorkloadNameKey(), local_node_info.workload_name},
         {DestinationWorkloadNamespaceKey(), local_node_info.namespace_name},
         {DestinationOwnerKey(), local_node_info.owner}});
  } else if (kind_ == stackdriver::config::PluginConfig::ReporterKind::
                          PluginConfig_ReporterKind_OUTBOUND) {
    opencensus::stats::Record(
        {{ClientRequestCountMeasure(), 1},
         {ClientRequestBytesMeasure(), request_info.request_size},
         {ClientResponseBytesMeasure(), request_info.response_size},
         {ClientRoundtripLatenciesMeasure(), latency_ms}},
        {{RequestOperationKey(), request_info.request_operation},
         {RequestProtocolKey(), request_info.request_protocol},
         {ServiceAuthenticationPolicyKey(),
          request_info.mTLS ? kMutualTLS : kNone},
         {DestinationServiceNameKey(), request_info.destination_service_host},
         {DestinationServiceNamespaceKey(),
          request_info.peer_node_info.namespace_name},
         {DestinationPortKey(), std::to_string(request_info.destination_port)},
         {ResponseCodeKey(), std::to_string(request_info.response_code)},
         {SourcePrincipalKey(), request_info.source_principal},
         {SourceWorkloadNameKey(), local_node_info.workload_name},
         {SourceWorkloadNamespaceKey(), local_node_info.namespace_name},
         {SourceOwnerKey(), local_node_info.owner},
         {DestinationPrincipalKey(), request_info.destination_principal},
         {DestinationWorkloadNameKey(),
          request_info.peer_node_info.workload_name},
         {DestinationWorkloadNamespaceKey(),
          request_info.peer_node_info.namespace_name},
         {DestinationOwnerKey(), request_info.peer_node_info.owner}});
  } else {
    // log something
  }
}

}  // namespace Metric
}  // namespace Stackdriver
