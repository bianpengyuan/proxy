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
#include "extensions/stackdriver/common/constants.h"
#include "google/api/monitored_resource.pb.h"

using namespace ::Stackdriver::Common;

namespace Stackdriver {
namespace Metric {

opencensus::exporters::stats::StackdriverOptions GetStackdriverOptions(
    const Stackdriver::Common::NodeInfo &local_node_info) {
  opencensus::exporters::stats::StackdriverOptions options;
  options.project_id = local_node_info.project_id;

  // Inbound Monitored Resource
  google::api::MonitoredResource inbound_monitored_resource;
  inbound_monitored_resource.set_type(kPodMonitoredResource);
  (*inbound_monitored_resource.mutable_labels())[kProjectIDLabel] =
      local_node_info.project_id;
  (*inbound_monitored_resource.mutable_labels())[kLocationLabel] =
      local_node_info.location;
  (*inbound_monitored_resource.mutable_labels())[kClusterNameLabel] =
      local_node_info.cluster_name;
  (*inbound_monitored_resource.mutable_labels())[kNamespaceNameLabel] =
      local_node_info.namespace_name;
  (*inbound_monitored_resource.mutable_labels())[kPodNameLabel] =
      local_node_info.name;
  // TODO: Add monitored resource to options

  // Outbound Monitored Resource
  google::api::MonitoredResource outbound_monitored_resource;
  outbound_monitored_resource.set_type(kContainerMonitoredResource);
  (*outbound_monitored_resource.mutable_labels())[kProjectIDLabel] =
      local_node_info.project_id;
  (*outbound_monitored_resource.mutable_labels())[kLocationLabel] =
      local_node_info.location;
  (*outbound_monitored_resource.mutable_labels())[kClusterNameLabel] =
      local_node_info.cluster_name;
  (*outbound_monitored_resource.mutable_labels())[kNamespaceNameLabel] =
      local_node_info.namespace_name;
  (*outbound_monitored_resource.mutable_labels())[kPodNameLabel] =
      local_node_info.name;
  // TODO: Add monitored resource to options

  return options;
}

/*
 *  macros to register views
 */
#define REGISTER_COUNT_VIEW(_v)                                       \
  void Register##_v##View() {                                         \
    const opencensus::stats::ViewDescriptor view_descriptor =         \
        opencensus::stats::ViewDescriptor()                           \
            .set_name(k##_v##View)                                    \
            .set_measure(k##_v##Measure)                              \
            .set_aggregation(opencensus::stats::Aggregation::Count()) \
                ADD_TAGS;                                             \
    opencensus::stats::View view(view_descriptor);                    \
    view_descriptor.RegisterForExport();                              \
  }

#define REGISTER_DISTRIBUTION_VIEW(_v)                                       \
  void Register##_v##View() {                                                \
    const opencensus::stats::ViewDescriptor view_descriptor =                \
        opencensus::stats::ViewDescriptor()                                  \
            .set_name(k##_v##View)                                           \
            .set_measure(k##_v##Measure)                                     \
            .set_aggregation(opencensus::stats::Aggregation::Distribution(   \
                opencensus::stats::BucketBoundaries::Exponential(20, 1, 2))) \
                ADD_TAGS;                                                    \
    opencensus::stats::View view(view_descriptor);                           \
    view_descriptor.RegisterForExport();                                     \
  }

#define ADD_TAGS                                     \
  .add_column(RequestOperationKey())                 \
      .add_column(RequestProtocolKey())              \
      .add_column(ServiceAuthenticationPolicyKey())  \
      .add_column(MeshUIDKey())                      \
      .add_column(DestinationServiceNameKey())       \
      .add_column(DestinationServiceNamespaceKey())  \
      .add_column(DestinationPortKey())              \
      .add_column(ResponseCodeKey())                 \
      .add_column(SourcePrincipalKey())              \
      .add_column(SourceWorkloadNameKey())           \
      .add_column(SourceWorkloadNamespaceKey())      \
      .add_column(SourceOwnerKey())                  \
      .add_column(DestinationPrincipalKey())         \
      .add_column(DestinationWorkloadNameKey())      \
      .add_column(DestinationWorkloadNamespaceKey()) \
      .add_column(DestinationOwnerKey())

// Functions to register opencensus views to export.
REGISTER_COUNT_VIEW(ServerRequestCount)
REGISTER_DISTRIBUTION_VIEW(ServerRequestBytes)
REGISTER_DISTRIBUTION_VIEW(ServerResponseBytes)
REGISTER_DISTRIBUTION_VIEW(ServerResponseLatencies)
REGISTER_COUNT_VIEW(ClientRequestCount)
REGISTER_DISTRIBUTION_VIEW(ClientRequestBytes)
REGISTER_DISTRIBUTION_VIEW(ClientResponseBytes)
REGISTER_DISTRIBUTION_VIEW(ClientRoundtripLatencies)

void RegisterViews() {
  RegisterServerRequestCountView();
  RegisterServerRequestBytesView();
  RegisterServerResponseBytesView();
  RegisterServerResponseLatenciesView();
  RegisterClientRequestCountView();
  RegisterClientRequestBytesView();
  RegisterClientResponseBytesView();
  RegisterClientRoundtripLatenciesView();
}

/*
 * tag key function macros
 */
#define TAG_KEY_FUNC(_t, _f)                                              \
  opencensus::tags::TagKey _f##Key() {                                    \
    static const auto _t##_key = opencensus::tags::TagKey::Register(#_t); \
    return _t##_key;                                                      \
  }

// Tag key functions
TAG_KEY_FUNC(request_operation, RequestOperation)
TAG_KEY_FUNC(request_protocol, RequestProtocol)
TAG_KEY_FUNC(service_authentication_policy, ServiceAuthenticationPolicy)
TAG_KEY_FUNC(mesh_uid, MeshUID)
TAG_KEY_FUNC(destination_service_name, DestinationServiceName)
TAG_KEY_FUNC(destination_service_namespace, DestinationServiceNamespace)
TAG_KEY_FUNC(destination_port, DestinationPort)
TAG_KEY_FUNC(response_code, ResponseCode)
TAG_KEY_FUNC(source_principal, SourcePrincipal)
TAG_KEY_FUNC(source_workload_name, SourceWorkloadName)
TAG_KEY_FUNC(source_workload_namespace, SourceWorkloadNamespace)
TAG_KEY_FUNC(source_owner, SourceOwner)
TAG_KEY_FUNC(destination_principal, DestinationPrincipal)
TAG_KEY_FUNC(destination_workload_name, DestinationWorkloadName)
TAG_KEY_FUNC(destination_workload_namespace, DestinationWorkloadNamespace)
TAG_KEY_FUNC(destination_owner, DestinationOwner)

/*
 * measure function macros
 */
#define MEASURE_FUNC(_m, _u, _t)                                           \
  opencensus::stats::Measure##_t _m##Measure() {                           \
    static const opencensus::stats::Measure##_t measure =                  \
        opencensus::stats::Measure##_t::Register(k##_m##Measure, "", #_u); \
    return measure;                                                        \
  }

// Meausre functions
MEASURE_FUNC(ServerRequestCount, 1, Int64)
MEASURE_FUNC(ServerRequestBytes, By, Int64)
MEASURE_FUNC(ServerResponseBytes, By, Int64)
MEASURE_FUNC(ServerResponseLatencies, ms, Double)
MEASURE_FUNC(ClientRequestCount, 1, Int64)
MEASURE_FUNC(ClientRequestBytes, By, Int64)
MEASURE_FUNC(ClientResponseBytes, By, Int64)
MEASURE_FUNC(ClientRoundtripLatencies, ms, Double)

}  // namespace Metric
}  // namespace Stackdriver