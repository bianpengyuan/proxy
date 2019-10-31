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
#include "extensions/common/context.h"

// WASM_PROLOG
#ifndef NULL_PLUGIN
#include "proxy_wasm_intrinsics.h"

#else  // NULL_PLUGIN

#include "extensions/common/wasm/null/null_plugin.h"

using Envoy::Extensions::Common::Wasm::HeaderMapType;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getHeaderMapValue;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getValue;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getStringValue;

#endif  // NULL_PLUGIN

namespace Wasm {
namespace Common {

namespace {

// Extract fqdn from Istio cluster name, e.g.
// inbound|9080|http|productpage.default.svc.cluster.local. If cluster name does
// not follow Istio convention, fqdn will be left as empty string.
void extractFqdn(const std::string& cluster_name, std::string* fqdn) {
  const std::vector<std::string>& parts = absl::StrSplit(cluster_name, '|');
  if (parts.size() == 4) {
    *fqdn = parts[3];
  }
}

// Extract service name from service fqdn.
void extractServiceName(const std::string& fqdn, std::string* service_name) {
  const std::vector<std::string>& parts = absl::StrSplit(fqdn, '.');
  if (parts.size() > 0) {
    *service_name = parts[0];
  }
}

}  // namespace

bool RequestInfoImpl::isOutbound() {
  if (context_.has_traffic_direction()) {
    return static_cast<::Wasm::Common::TrafficDirection>(
               context_.traffic_direction().value()) ==
           ::Wasm::Common::TrafficDirection::Outbound;
  }
  int64_t direction;
  getValue({"listener_direction"}, &direction);
  context_.mutable_traffic_direction()->set_value(direction);
  return static_cast<::Wasm::Common::TrafficDirection>(direction) ==
         ::Wasm::Common::TrafficDirection::Outbound;
  ;
}

google::protobuf::Timestamp RequestInfoImpl::startTimestamp() {
  if (!context_.has_request_timestamp()) {
    getValue({"request", "time"}, context_.mutable_request_timestamp());
  }
  return context_.request_timestamp();
}

google::protobuf::Timestamp RequestInfoImpl::responseTimestamp() {
  if (!context_.has_response_timestamp()) {
    getValue({"response", "time"}, context_.mutable_response_timestamp());
  }
  return context_.response_timestamp();
}

google::protobuf::Duration RequestInfoImpl::duration() {
  if (!context_.has_duration()) {
    getValue({"request", "duration"}, context_.mutable_duration());
  }
  return context_.duration();
}

google::protobuf::Duration RequestInfoImpl::responseDuration() {
  if (!context_.has_response_duration()) {
    getValue({"response", "duration"}, context_.mutable_response_duration());
  }
  return context_.response_duration();
}

int64_t RequestInfoImpl::requestSize() {
  if (context_.has_request_size()) {
    return context_.request_size().value();
  }
  int64_t request_total_size;
  getValue({"request", "total_size"}, &request_total_size);
  context_.mutable_request_size()->set_value(request_total_size);
  return request_total_size;
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
    int64_t destination_port;
    if (isOutbound()) {
      getValue({"upstream", "port"}, &destination_port);
    } else {
      getValue({"destination", "port"}, &destination_port);
    }
    context_.mutable_destination_port()->set_value(destination_port);
  }
  return context_.destination_port().value();
}

const std::string& RequestInfoImpl::requestProtocol() {
  if (context_.has_request_protocol()) {
    return context_.request_protocol().value();
  }

  // TODO Add http/1.1, http/1.0, http/2 in a separate attribute.
  // http|grpc classification is compatible with Mixerclient
  if (kGrpcContentTypes.count(getHeaderMapValue(HeaderMapType::RequestHeaders,
                                                kContentTypeHeaderKey)
                                  ->toString()) != 0) {
    context_.mutable_request_protocol()->set_value(kProtocolGRPC);
  } else {
    context_.mutable_request_protocol()->set_value(kProtocolHTTP);
  }

  // string_attributes_.emplace(RequestProtocolKey, proto);
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

const std::string& RequestInfoImpl::responseFlag() {
  if (context_.has_request_protocol()) {
    return context_.request_protocol().value();
  }

  // TODO Add http/1.1, http/1.0, http/2 in a separate attribute.
  // http|grpc classification is compatible with Mixerclient
  if (kGrpcContentTypes.count(getHeaderMapValue(HeaderMapType::RequestHeaders,
                                                kContentTypeHeaderKey)
                                  ->toString()) != 0) {
    context_.mutable_request_protocol()->set_value(kProtocolGRPC);
  } else {
    context_.mutable_request_protocol()->set_value(kProtocolHTTP);
  }

  // string_attributes_.emplace(RequestProtocolKey, proto);
  return context_.request_protocol().value();
}

const std::string& RequestInfoImpl::destinationServiceHost() {
  if (!context_.has_destination_service_host()) {
    // Try to get fqdn of destination service from cluster name. If not found, use
    // host header instead.
    std::string cluster_name = "";
    getStringValue({"cluster_name"}, &cluster_name);
    extractFqdn(cluster_name, context_.mutable_destination_service_host()->mutable_value());
    if (context_.destination_service_host().value().empty()) {
      // fallback to host header.
      context_.mutable_destination_service_host()->set_value(
          getHeaderMapValue(HeaderMapType::RequestHeaders, kAuthorityHeaderKey)
              ->toString());
      // TODO: what should be the fallback here?
      context_.mutable_destination_service_name()->set_value(cluster_name);
    } else {
      extractServiceName(context_.destination_service_host().value(),
                         context_.mutable_destination_service_name()->mutable_value());
    }
  }
  return context_.destination_service_host().value();
}

const std::string& RequestInfoImpl::destiantionServiceName() {
  if (!context_.has_destination_service_name()) {
    destinationServiceHost();
  }
  return context_.destination_service_name().value();
}

// const std::string& RequestInfoImpl::requestOperation() {

// }

// bool RequestInfoImpl::mTLS() {}
// const std::string& RequestInfoImpl::sourcePrincipal() {}
// const std::string& RequestInfoImpl::destinationPrincipal() {}
// const std::string& RequestInfoImpl::rbacPermissivePolicyID() {}
// const std::string& RequestInfoImpl::rbacPermissiveEngineResult() {}
// const std::string& RequestInfoImpl::requestedServerName() {}

// // Important headers
// const std::string& RequestInfoImpl::referer() {}
// const std::string& RequestInfoImpl::userAgent() {}
// const std::string& RequestInfoImpl::url() {}
// const std::string& RequestInfoImpl::requestID() {}
// const std::string& RequestInfoImpl::traceID() {}
// const std::string& RequestInfoImpl::spanID() {}
// bool RequestInfoImpl::traceSampled() {}

}  // namespace Common
}  // namespace Wasm
