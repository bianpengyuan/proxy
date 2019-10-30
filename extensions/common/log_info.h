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

#include "extensions/common/log_info.pb.h"

namespace Wasm {
namespace Common {

class LogInfo {
 public:
  virtual ~LogInfo(){};

  virtual const google::protobuf::Timestamp& requestTimestamp() = 0;
  virtual const google::protobuf::Timestamp& responseTimestamp() = 0;
  virtual int64_t requestSize() = 0;
  virtual int64_t responseSize() = 0;
  virtual int64_t destinationPort() = 0;
  virtual const std::string& requestProtocol() = 0;
  virtual int64_t responseCode() = 0;
  // virtual const std::string& responseFlag() = 0;
  virtual const std::string& destinationServiceHost() = 0;
  virtual const std::string& destiantionServiceName() = 0;
  virtual const std::string& requestOperation() = 0;
  virtual bool mTLS() = 0;
  virtual const std::string& sourcePrincipal() = 0;
  virtual const std::string& destinationPrincipal() = 0;
  virtual const std::string& rbacPermissivePolicyID() = 0;
  virtual const std::string& rbacPermissiveEngineResult() = 0;
  virtual const google::protobuf::Duration& duration() = 0;
  virtual const google::protobuf::Duration& responseDuration() = 0;
  virtual const std::string& requestedServerName() = 0;
  virtual bool isOutbound() = 0;

  // Important headers
  virtual const std::string& referer() = 0;
  virtual const std::string& userAgent() = 0;
  virtual const std::string& urlPath() = 0;
  virtual const std::string& requestHost() = 0;
  virtual const std::string& requestScheme() = 0;
  virtual const std::string& requestID() = 0;
  virtual const std::string& b3TraceID() = 0;
  virtual const std::string& b3SpanID() = 0;
  virtual bool b3TraceSampled() = 0;
};

// LogInfo lazily load request related information.
class LogInfoImpl : public LogInfo {
  ~LogInfoImpl() {}

  const google::protobuf::Timestamp& requestTimestamp() override;
  const google::protobuf::Timestamp& responseTimestamp() override;
  int64_t requestSize() override;
  int64_t responseSize() override;
  int64_t destinationPort() override;
  const std::string& requestProtocol() override;
  int64_t responseCode() override;
  // const std::string& responseFlag() override;
  const std::string& destinationServiceHost() override;
  const std::string& destiantionServiceName() override;
  const std::string& requestOperation() override;
  bool mTLS() override;
  const std::string& sourcePrincipal() override;
  const std::string& destinationPrincipal() override;
  const std::string& rbacPermissivePolicyID() override;
  const std::string& rbacPermissiveEngineResult() override;
  const google::protobuf::Duration& duration() override;
  const google::protobuf::Duration& responseDuration() override;
  const std::string& requestedServerName() override;
  bool isOutbound() override;

  // Important headers
  const std::string& referer() override;
  const std::string& userAgent() override;
  const std::string& urlPath() override;
  const std::string& requestHost() override;
  const std::string& requestScheme() override;
  const std::string& requestID() override;
  const std::string& b3TraceID() override;
  const std::string& b3SpanID() override;
  bool b3TraceSampled() override;

 private:
  ::wasm::common::LogInfo context_;
};

}  // namespace Common
}  // namespace Wasm
