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

#include "extensions/common/context.h"

#include "google/protobuf/util/json_util.h"

// WASM_PROLOG
#ifndef NULL_PLUGIN
#include "proxy_wasm_intrinsics.h"

#else  // NULL_PLUGIN

#include "absl/strings/str_split.h"
#include "extensions/common/wasm/null/null_plugin.h"

using Envoy::Extensions::Common::Wasm::HeaderMapType;
using Envoy::Extensions::Common::Wasm::MetadataType;
using Envoy::Extensions::Common::Wasm::StreamType;
using Envoy::Extensions::Common::Wasm::WasmResult;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getCurrentTimeNanoseconds;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getHeaderMapValue;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getStringValue;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getStructValue;
using Envoy::Extensions::Common::Wasm::Null::Plugin::getValue;

#endif  // NULL_PLUGIN

// END WASM_PROLOG

namespace Wasm {
namespace Common {

const char kRbacFilterName[] = "envoy.filters.http.rbac";
const char kRbacPermissivePolicyIDField[] = "shadow_effective_policy_id";
const char kRbacPermissiveEngineResultField[] = "shadow_engine_result";

namespace {

// Extract fqdn from Istio cluster name. If cluster name does not follow Istio
// convention, fqdn will be left as empty string.
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

using google::protobuf::util::JsonStringToMessage;
using google::protobuf::util::MessageToJsonString;

google::protobuf::util::Status extractNodeMetadata(
    const google::protobuf::Struct& metadata,
    wasm::common::NodeInfo* node_info) {
  google::protobuf::util::JsonOptions json_options;
  std::string metadata_json_struct;
  auto status =
      MessageToJsonString(metadata, &metadata_json_struct, json_options);
  if (status != google::protobuf::util::Status::OK) {
    return status;
  }
  google::protobuf::util::JsonParseOptions json_parse_options;
  json_parse_options.ignore_unknown_fields = true;
  return JsonStringToMessage(metadata_json_struct, node_info,
                             json_parse_options);
}

google::protobuf::util::Status extractLocalNodeMetadata(
    wasm::common::NodeInfo* node_info) {
  google::protobuf::Struct node;
  if (!getStructValue({"node", "metadata"}, &node)) {
    return google::protobuf::util::Status(
        google::protobuf::util::error::Code::NOT_FOUND, "metadata not found");
  }
  return extractNodeMetadata(node, node_info);
}

void populateHTTPRequestInfo(bool outbound, RequestInfo* request_info) {
  // TODO: switch to stream_info.requestComplete() to avoid extra compute.
  request_info->end_timestamp = getCurrentTimeNanoseconds();

  // Fill in request info.
  int64_t response_code = 0;
  if (getValue({"response", "code"}, &response_code)) {
    request_info->response_code = response_code;
  }

  if (kGrpcContentTypes.count(getHeaderMapValue(HeaderMapType::RequestHeaders,
                                                kContentTypeHeaderKey)
                                  ->toString()) != 0) {
    request_info->request_protocol = kProtocolGRPC;
  } else {
    // TODO Add http/1.1, http/1.0, http/2 in a separate attribute.
    // http|grpc classification is compatible with Mixerclient
    request_info->request_protocol = kProtocolHTTP;
  }

  // Try to get fqdn of destination service from cluster name. If not found, use host header
  // instead.
  std::string cluster_name = "";
  getStringValue({"cluster_name"}, &cluster_name);
  extractFqdn(cluster_name, &request_info->destination_service_host);
  if (request_info->destination_service_host.empty()) {
    // fallback to host header.
    request_info->destination_service_host =
        getHeaderMapValue(HeaderMapType::RequestHeaders, kAuthorityHeaderKey)
            ->toString();
  } else {
    // cluster name follows Istio convention, so extract out service name.
    extractServiceName(request_info->destination_service_host,
                       &request_info->destination_service_name);
  }

  // Get rbac labels from dynamic metadata.
  getStringValue({"metadata", kRbacFilterName, kRbacPermissivePolicyIDField},
                 &request_info->rbac_permissive_policy_id);
  getStringValue(
      {"metadata", kRbacFilterName, kRbacPermissiveEngineResultField},
      &request_info->rbac_permissive_engine_result);

  request_info->request_operation =
      getHeaderMapValue(HeaderMapType::RequestHeaders, kMethodHeaderKey)
          ->toString();

  int64_t destination_port = 0;
  std::string tls_version;

  if (outbound) {
    getValue({"upstream", "port"}, &destination_port);
    getValue({"upstream", "mtls"}, &request_info->mTLS);
    getStringValue({"upstream", "tls_version"}, &tls_version);
  } else {
    getValue({"destination", "port"}, &destination_port);
    getValue({"connection", "mtls"}, &request_info->mTLS);
    getStringValue({"connection", "tls_version"}, &tls_version);
  }
  request_info->destination_port = destination_port;
}

}  // namespace Common
}  // namespace Wasm
