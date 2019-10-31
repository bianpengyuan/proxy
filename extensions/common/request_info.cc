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

#endif  // NULL_PLUGIN

namespace Wasm {
namespace Common {

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
  // TODO(bianpengyuan): Get start timestamp when upstream API exposes it.
  return context_.start_timestamp();
}

google::protobuf::Timestamp RequestInfoImpl::endTimestamp() {
  // TODO(bianpengyuan): Get end timestamp when upstream API exposes it.
  return context_.end_timestamp();
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
    context_.mutable_request_protocol().set_value(kProtocolGRPC);
  } else {
    context_.mutable_request_protocol().set_value(kProtocolHTTP);
  }

  // string_attributes_.emplace(RequestProtocolKey, proto);
  return string_attributes_[RequestProtocolKey];
}

// uint32_t RequestInfoImpl::responseCode() {}
// const std::string& RequestInfoImpl::responseFlag() {}
// const std::string& RequestInfoImpl::destinationServiceHost() {}
// const std::string& RequestInfoImpl::destiantionServiceName() {}
// const std::string& RequestInfoImpl::requestOperation() {}
// bool RequestInfoImpl::mTLS() {}
// const std::string& RequestInfoImpl::sourcePrincipal() {}
// const std::string& RequestInfoImpl::destinationPrincipal() {}
// const std::string& RequestInfoImpl::rbacPermissivePolicyID() {}
// const std::string& RequestInfoImpl::rbacPermissiveEngineResult() {}
// int64_t RequestInfoImpl::duration() {}
// int64_t RequestInfoImpl::responseDuration() {}
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
