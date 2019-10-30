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


namespace Wasm {
namespace Common {

constexpr char StartTimeStamp[] = "start_timestamp";
constexpr char EndTimeStamp[] = "end_timestamp";
constexpr char RequestSize[] = "request_size";
constexpr char ResponseSize[] = "response_size";
constexpr char DestinationPort[] = "destination_port";

int64_t RequestInfoImpl::startTimestamp() {
  auto iter = int_attributes_.find("start_timestamp");
}

int64_t RequestInfoImpl::endTimestamp() {}
int64_t RequestInfoImpl::requestSize() {}
int64_t RequestInfoImpl::responseSize() {}
uint32_t RequestInfoImpl::destinationPort() {}
const std::string& RequestInfoImpl::requestProtocol() {}
uint32_t RequestInfoImpl::responseCode() {}
const std::string& RequestInfoImpl::responseFlag() {}
const std::string& RequestInfoImpl::destinationServiceHost() {}
const std::string& RequestInfoImpl::destiantionServiceName() {}
const std::string& RequestInfoImpl::requestOperation() {}
bool RequestInfoImpl::mTLS() {}
const std::string& RequestInfoImpl::sourcePrincipal() {}
const std::string& RequestInfoImpl::destinationPrincipal() {}
const std::string& RequestInfoImpl::rbacPermissivePolicyID() {}
const std::string& RequestInfoImpl::rbacPermissiveEngineResult() {}
int64_t RequestInfoImpl::duration() {}
int64_t RequestInfoImpl::responseDuration() {}
const std::string& RequestInfoImpl::requestedServerName() {}

// Important headers
const std::string& RequestInfoImpl::referer() {}
const std::string& RequestInfoImpl::userAgent() {}
const std::string& RequestInfoImpl::url() {}
const std::string& RequestInfoImpl::requestID() {}
const std::string& RequestInfoImpl::traceID() {}
const std::string& RequestInfoImpl::spanID() {}
bool RequestInfoImpl::traceSampled() {}

}  // namespace Common
}  // namespace Wasm
