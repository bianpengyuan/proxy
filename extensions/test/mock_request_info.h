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

#include <string>
#include <unordered_map>

#include "extensions/common/request_info.h"
#include "gmock/gmock.h"

namespace Wasm {
namespace Common {
namespace Context {

class MockRequestInfo : public RequestInfo {
 public:
  MOCK_METHOD0(requestTimestamp, const google::protobuf::Timestamp&());
  MOCK_METHOD0(responseTimestamp, const google::protobuf::Timestamp&());
  MOCK_METHOD0(requestSize, int64_t());
  MOCK_METHOD0(responseSize, int64_t());
  MOCK_METHOD0(destinationPort, int64_t());
  MOCK_METHOD0(requestProtocol, const std::string&());
  MOCK_METHOD0(responseCode, int64_t());
  MOCK_METHOD0(destinationServiceHost, const std::string&());
  MOCK_METHOD0(destinationServiceName, const std::string&());
  MOCK_METHOD0(sourceAddress, const std::string&());
  MOCK_METHOD0(destinationAddress, const std::string&());
  MOCK_METHOD0(requestOperation, const std::string&());
  MOCK_METHOD0(serviceAuthenticationPolicy, ServiceAuthenticationPolicy());
  MOCK_METHOD0(sourcePrincipal, const std::string&());
  MOCK_METHOD0(destinationPrincipal, const std::string&());
  MOCK_METHOD0(rbacPermissivePolicyID, const std::string&());
  MOCK_METHOD0(rbacPermissiveEngineResult, const std::string&());
  MOCK_METHOD0(duration, const google::protobuf::Duration&());
  MOCK_METHOD0(responseDuration, const google::protobuf::Duration&());
  MOCK_METHOD0(responseFlag, const std::string&());
  MOCK_METHOD0(requestedServerName, const std::string&());
  MOCK_METHOD0(isOutbound, bool());

  MOCK_METHOD0(referer, const std::string&());
  MOCK_METHOD0(userAgent, const std::string&());
  MOCK_METHOD0(urlPath, const std::string&());
  MOCK_METHOD0(requestHost, const std::string&());
  MOCK_METHOD0(requestScheme, const std::string&());
  MOCK_METHOD0(requestID, const std::string&());
  MOCK_METHOD0(b3TraceID, const std::string&());
  MOCK_METHOD0(b3SpanID, const std::string&());
  MOCK_METHOD0(b3TraceSampled, bool());
};

}  // namespace Context
}  // namespace Common
}  // namespace Wasm
