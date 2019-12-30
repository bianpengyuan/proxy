/* Copyright 2019 Istio Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License") {}
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

#include "extensions/common/request_info.h"

#include "extensions/common/util.h"

// WASM_PROLOG
#ifndef NULL_PLUGIN
#include "proxy_wasm_intrinsics.h"

#else  // NULL_PLUGIN

#include "extensions/common/wasm/null/null_plugin.h"

using Envoy::Extensions::Common::Wasm::HeaderMapType;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getHeaderMapValue;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getStringValue;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getValue;

#endif  // NULL_PLUGIN

namespace Wasm {
namespace Common {
namespace Context {

const char RbacFilterName[] = "envoy.filters.http.rbac";
const char RbacPermissivePolicyIDField[] = "shadow_effective_policy_id";
const char RbacPermissiveEngineResultField[] = "shadow_engine_result";
const char kBlackHoleCluster[] = "BlackHoleCluster";
const char kPassThroughCluster[] = "PassthroughCluster";
const char kInboundPassthroughClusterIpv4[] = "InboundPassthroughClusterIpv4";
const char kInboundPassthroughClusterIpv6[] = "InboundPassthroughClusterIpv6";
const char B3TraceID[] = "x-b3-traceid";
const char B3SpanID[] = "x-b3-spanid";
const char B3TraceSampled[] = "x-b3-sampled";

namespace {

// Extract service name from service host.
void extractServiceName(const std::string& host,
                        const std::string& destination_namespace,
                        std::string* service_name) {
  auto name_pos = host.find_first_of(".:");
  if (name_pos == std::string::npos) {
    // host is already a short service name. return it directly.
    *service_name = host;
    return;
  }
  if (host[name_pos] == ':') {
    // host is `short_service:port`, return short_service name.
    *service_name = host.substr(0, name_pos);
    return;
  }

  auto namespace_pos = host.find_first_of(".:", name_pos + 1);
  std::string service_namespace = "";
  if (namespace_pos == std::string::npos) {
    service_namespace = host.substr(name_pos + 1);
  } else {
    int namespace_size = namespace_pos - name_pos - 1;
    service_namespace = host.substr(name_pos + 1, namespace_size);
  }
  // check if namespace in host is same as destination namespace.
  // If it is the same, return the first part of host as service name.
  // Otherwise fallback to request host.
  if (service_namespace == destination_namespace) {
    *service_name = host.substr(0, name_pos);
  } else {
    *service_name = host;
  }
}

// Get destination service host and name based on destination cluster name and
// host header.
// * If cluster name is one of passthrough and blackhole clusters, use cluster
//   name as destination service name and host header as destination host.
// * If cluster name follows Istio convention (four parts separated by pipe),
//   use the last part as destination host; Otherwise, use host header as
//   destination host. To get destination service name from host: if destination
//   host is already a short name, use that as destination service; otherwise if
//   the second part of destination host is destination namespace, use first
//   part as destination service name. Otherwise, fallback to use destination
//   host for destination service name.
void getDestinationService(const std::string& dest_namespace,
                           bool use_host_header, std::string* dest_svc_host,
                           std::string* dest_svc_name) {
  std::string cluster_name;
  getStringValue({"cluster_name"}, &cluster_name);
  *dest_svc_host = use_host_header
                       ? getHeaderMapValue(HeaderMapType::RequestHeaders,
                                           kAuthorityHeaderKey)
                             ->toString()
                       : "unknown";

  if (cluster_name == kBlackHoleCluster ||
      cluster_name == kPassThroughCluster ||
      cluster_name == kInboundPassthroughClusterIpv4 ||
      cluster_name == kInboundPassthroughClusterIpv6) {
    *dest_svc_name = cluster_name;
    return;
  }

  std::vector<absl::string_view> parts = absl::StrSplit(cluster_name, '|');
  if (parts.size() == 4) {
    *dest_svc_host = std::string(parts[3].data(), parts[3].size());
  }

  extractServiceName(*dest_svc_host, dest_namespace, dest_svc_name);
}

void EncodeDuration(absl::Duration duration,
                    google::protobuf::Duration* proto) {
  // s and n may both be negative, per the Duration proto spec.
  const int64_t s = absl::IDivDuration(duration, absl::Seconds(1), &duration);
  const int64_t n =
      absl::IDivDuration(duration, absl::Nanoseconds(1), &duration);
  proto->set_seconds(s);
  proto->set_nanos(n);
}

void EncodeTime(absl::Time time, google::protobuf::Timestamp* proto) {
  const int64_t s = absl::ToUnixSeconds(time);
  proto->set_seconds(s);
  proto->set_nanos((time - absl::FromUnixSeconds(s)) / absl::Nanoseconds(1));
}

}  // namespace

StringView AuthenticationPolicyString(ServiceAuthenticationPolicy policy) {
  switch (policy) {
    case ServiceAuthenticationPolicy::None:
      return kNone;
    case ServiceAuthenticationPolicy::MutualTLS:
      return kMutualTLS;
    default:
      break;
  }
  return {};
}

bool RequestInfoImpl::isOutbound() {
  if (!context_.has_traffic_direction()) {
    int64_t direction;
    getValue({"listener_direction"}, &direction);
    context_.mutable_traffic_direction()->set_value(direction);
  }
  return static_cast<::Wasm::Common::TrafficDirection>(
             context_.traffic_direction().value()) ==
         ::Wasm::Common::TrafficDirection::Outbound;
}

const google::protobuf::Timestamp& RequestInfoImpl::requestTimestamp() {
  if (!context_.has_request_timestamp()) {
    absl::Time request_time;
    getValue({"request", "time"}, &request_time);
    EncodeTime(request_time, context_.mutable_request_timestamp());
  }
  return context_.request_timestamp();
}

const google::protobuf::Timestamp& RequestInfoImpl::responseTimestamp() {
  if (!context_.has_response_timestamp()) {
    absl::Time response_time;
    getValue({"response", "time"}, &response_time);
    EncodeTime(response_time, context_.mutable_response_timestamp());
  }
  return context_.response_timestamp();
}

const google::protobuf::Duration& RequestInfoImpl::duration() {
  if (!context_.has_duration()) {
    absl::Duration duration;
    getValue({"request", "duration"}, &duration);
    EncodeDuration(duration, context_.mutable_duration());
  }
  return context_.duration();
}

const google::protobuf::Duration& RequestInfoImpl::responseDuration() {
  if (!context_.has_response_duration()) {
    absl::Duration duration;
    getValue({"response", "duration"}, &duration);
    EncodeDuration(duration, context_.mutable_response_duration());
  }
  return context_.response_duration();
}

const std::string& RequestInfoImpl::responseFlag() {
  if (!context_.has_response_flag()) {
    uint64_t response_flags = 0;
    getValue({"response", "flags"}, &response_flags);
    context_.mutable_response_flag()->set_value(
        parseResponseFlag(response_flags));
  }
  return context_.response_flag().value();
}

int64_t RequestInfoImpl::requestSize() {
  if (!context_.has_request_size()) {
    int64_t request_total_size;
    getValue({"request", "total_size"}, &request_total_size);
    context_.mutable_request_size()->set_value(request_total_size);
  }
  return context_.request_size().value();
}

int64_t RequestInfoImpl::responseSize() {
  if (!context_.has_response_size()) {
    int64_t response_total_size;
    getValue({"response", "total_size"}, &response_total_size);
    context_.mutable_response_size()->set_value(response_total_size);
  }
  return context_.response_size().value();
}

int64_t RequestInfoImpl::destinationPort() {
  if (!context_.has_destination_port()) {
    int64_t destination_port = 0;
    if (isOutbound()) {
      getValue({"upstream", "port"}, &destination_port);
    } else {
      getValue({"destination", "port"}, &destination_port);
    }
    context_.mutable_destination_port()->set_value(destination_port);
  }
  return context_.destination_port().value();
}

const std::string& RequestInfoImpl::sourceAddress() {
  if (!context_.has_source_address()) {
    getStringValue({"source", "address"},
                   context_.mutable_source_address()->mutable_value());
  }
  return context_.source_address().value();
}

const std::string& RequestInfoImpl::destinationAddress() {
  if (!context_.has_destination_address()) {
    getStringValue({"destination", "address"},
                   context_.mutable_destination_address()->mutable_value());
  }
  return context_.destination_address().value();
}

const std::string& RequestInfoImpl::requestProtocol() {
  if (!context_.has_request_protocol()) {
    // TODO Add http/1.1, http/1.0, http/2 in a separate attribute.
    // http|grpc classification is compatible with Mixerclient
    if (kGrpcContentTypes.count(getHeaderMapValue(HeaderMapType::RequestHeaders,
                                                  kContentTypeHeaderKey)
                                    ->toString()) != 0) {
      context_.mutable_request_protocol()->set_value(kProtocolGRPC);
    } else {
      context_.mutable_request_protocol()->set_value(kProtocolHTTP);
    }
  }

  return context_.request_protocol().value();
}

int64_t RequestInfoImpl::responseCode() {
  if (!context_.has_response_code()) {
    int64_t response_code;
    getValue({"response", "code"}, &response_code);
    context_.mutable_response_code()->set_value(response_code);
  }
  return context_.response_code().value();
}

const std::string& RequestInfoImpl::destinationServiceHost() {
  if (!context_.has_destination_service_host()) {
    // Get destination service name and host based on cluster name and host
    // header.
    getDestinationService(
        destination_namespace_, use_traffic_data_,
        context_.mutable_destination_service_host()->mutable_value(),
        context_.mutable_destination_service_name()->mutable_value());
  }
  return context_.destination_service_host().value();
}

const std::string& RequestInfoImpl::destinationServiceName() {
  if (!context_.has_destination_service_name()) {
    // Get destination service name and host based on cluster name and host
    // header.
    getDestinationService(
        destination_namespace_, use_traffic_data_,
        context_.mutable_destination_service_host()->mutable_value(),
        context_.mutable_destination_service_name()->mutable_value());
  }
  return context_.destination_service_name().value();
}

const std::string& RequestInfoImpl::requestOperation() {
  if (!context_.has_request_operation()) {
    getStringValue({"request", "method"},
                   context_.mutable_request_operation()->mutable_value());
  }
  return context_.request_operation().value();
}

::Wasm::Common::ServiceAuthenticationPolicy
RequestInfoImpl::serviceAuthenticationPolicy() {
  if (isOutbound()) {
    return ::Wasm::Common::ServiceAuthenticationPolicy::Unspecified;
  }
  if (!context_.has_mtls()) {
    bool mtls = false;
    getValue({"connection", "mtls"}, &mtls);
    context_.mutable_mtls()->set_value(mtls);
  }
  return context_.mtls().value()
             ? ::Wasm::Common::ServiceAuthenticationPolicy::MutualTLS
             : ::Wasm::Common::ServiceAuthenticationPolicy::None;
}

const std::string& RequestInfoImpl::sourcePrincipal() {
  if (!context_.has_source_principal()) {
    if (isOutbound()) {
      getStringValue({"upstream", "uri_san_local_certificate"},
                     context_.mutable_source_principal()->mutable_value());
    } else {
      getStringValue({"connection", "uri_san_peer_certificate"},
                     context_.mutable_source_principal()->mutable_value());
    }
  }
  return context_.source_principal().value();
}

const std::string& RequestInfoImpl::destinationPrincipal() {
  if (!context_.has_destination_principal()) {
    if (isOutbound()) {
      getStringValue({"upstream", "uri_san_peer_certificate"},
                     context_.mutable_destination_principal()->mutable_value());
    } else {
      getStringValue({"connection", "uri_san_local_certificate"},
                     context_.mutable_destination_principal()->mutable_value());
    }
  }
  return context_.destination_principal().value();
}

const std::string& RequestInfoImpl::rbacPermissivePolicyID() {
  if (!context_.has_rbac_permissive_policy_id()) {
    getStringValue(
        {"metadata", RbacFilterName, RbacPermissivePolicyIDField},
        context_.mutable_rbac_permissive_policy_id()->mutable_value());
  }
  return context_.rbac_permissive_policy_id().value();
}

const std::string& RequestInfoImpl::rbacPermissiveEngineResult() {
  if (!context_.has_rbac_permissive_engine_result()) {
    getStringValue(
        {"metadata", RbacFilterName, RbacPermissiveEngineResultField},
        context_.mutable_rbac_permissive_engine_result()->mutable_value());
  }
  return context_.rbac_permissive_engine_result().value();
}

const std::string& RequestInfoImpl::requestedServerName() {
  if (!context_.has_requested_server_name()) {
    getStringValue({"connection", "requested_server_name"},
                   context_.mutable_requested_server_name()->mutable_value());
  }
  return context_.requested_server_name().value();
}

const std::string& RequestInfoImpl::referer() {
  if (!context_.has_referer()) {
    getStringValue({"request", "referer"},
                   context_.mutable_referer()->mutable_value());
  }
  return context_.referer().value();
}

const std::string& RequestInfoImpl::userAgent() {
  if (!context_.has_user_agent()) {
    getStringValue({"request", "user_agent"},
                   context_.mutable_user_agent()->mutable_value());
  }
  return context_.user_agent().value();
}

const std::string& RequestInfoImpl::urlPath() {
  if (!context_.has_url_path()) {
    getStringValue({"request", "url_path"},
                   context_.mutable_url_path()->mutable_value());
  }
  return context_.url_path().value();
}

const std::string& RequestInfoImpl::requestHost() {
  if (!context_.has_url_host()) {
    getStringValue({"request", "host"},
                   context_.mutable_url_host()->mutable_value());
  }
  return context_.url_host().value();
}

const std::string& RequestInfoImpl::requestScheme() {
  if (!context_.has_url_scheme()) {
    getStringValue({"request", "scheme"},
                   context_.mutable_url_scheme()->mutable_value());
  }
  return context_.url_scheme().value();
}

const std::string& RequestInfoImpl::requestID() {
  if (!context_.has_request_id()) {
    getStringValue({"request", "id"},
                   context_.mutable_request_id()->mutable_value());
  }
  return context_.request_id().value();
}

const std::string& RequestInfoImpl::b3SpanID() {
  if (!context_.has_b3_span_id()) {
    getStringValue({"request", "headers", B3SpanID},
                   context_.mutable_b3_span_id()->mutable_value());
  }
  return context_.b3_span_id().value();
}

const std::string& RequestInfoImpl::b3TraceID() {
  if (!context_.has_b3_span_id()) {
    getStringValue({"request", "headers", B3TraceID},
                   context_.mutable_b3_span_id()->mutable_value());
  }
  return context_.b3_span_id().value();
}

bool RequestInfoImpl::b3TraceSampled() {
  if (!context_.has_b3_trace_sampled()) {
    bool sampled = false;
    getValue({"request", "headers", B3TraceSampled}, &sampled);
    context_.mutable_b3_trace_sampled()->set_value(sampled);
  }
  return context_.b3_trace_sampled().value();
}

}  // namespace Context
}  // namespace Common
}  // namespace Wasm
