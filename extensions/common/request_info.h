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

namespace Wasm {
namespace Common {

class RequestInfo {
 public:
  virtual ~RequestInfo() {};

  virtual int64_t startTimestamp() = 0;
  virtual int64_t endTimestamp() = 0;
  virtual int64_t requestSize() = 0;
  virtual int64_t responseSize() = 0;
  virtual uint32_t destinationPort() = 0;
  virtual const std::string& requestProtocol() = 0;
  virtual uint32_t responseCode() = 0;
  virtual const std::string& responseFlag() = 0;
  virtual const std::string& destinationServiceHost() = 0;
  virtual const std::string& destiantionServiceName() = 0;
  virtual const std::string& requestOperation() = 0;
  virtual bool mTLS() = 0;
  virtual const std::string& sourcePrincipal() = 0;
  virtual const std::string& destinationPrincipal() = 0;
  virtual const std::string& rbacPermissivePolicyID() = 0;
  virtual const std::string& rbacPermissiveEngineResult() = 0;
  virtual int64_t duration() = 0;
  virtual int64_t responseDuration() = 0;
  virtual const std::string& requestedServerName() = 0;

  // Important headers
  virtual const std::string& referer() = 0;
  virtual const std::string& userAgent() = 0;
  virtual const std::string& url() = 0;
  virtual const std::string& requestID() = 0;
  virtual const std::string& traceID() = 0;
  virtual const std::string& spanID() = 0;
  virtual bool traceSampled() = 0;

  // TODO(bianpengyuan): remove these two setters when start/end timestamp is ready in upstream API.
  virtual void setStartTimestamp() = 0;
  virtual void setEndTimestmap() = 0;
};

// RequestInfo lazily load request related information.
class RequestInfoImpl : RequestInfo {
  ~RequestInfoImpl() {}

  int64_t startTimestamp();
  int64_t endTimestamp();
  int64_t requestSize();
  int64_t responseSize();
  uint32_t destinationPort();
  const std::string& requestProtocol();
  uint32_t responseCode();
  const std::string& responseFlag();
  const std::string& destinationServiceHost();
  const std::string& destiantionServiceName();
  const std::string& requestOperation();
  bool mTLS();
  const std::string& sourcePrincipal();
  const std::string& destinationPrincipal();
  const std::string& rbacPermissivePolicyID();
  const std::string& rbacPermissiveEngineResult();
  int64_t duration();
  int64_t responseDuration();
  const std::string& requestedServerName();

  // Important headers
  const std::string& referer();
  const std::string& userAgent();
  const std::string& url();
  const std::string& requestID();
  const std::string& traceID();
  const std::string& spanID();
  bool traceSampled();

 private:
  std::unordered_map<std::string, std::string> string_attributes_;
  std::unordered_map<std::string, bool> boolean_attributes_;
  std::unordered_map<std::string, int64_t> int_attributes_;
  std::unordered_map<std::string, double> float_attributes_;  
};

}  // namespace Common
}  // namespace Wasm
