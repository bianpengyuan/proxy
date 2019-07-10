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

#include <google/protobuf/struct.pb.h>
#include <string>
#include <unordered_map>

namespace Stackdriver {
namespace Common {

// NodeInfo represents the information extracted from proxy node metadata, or
// peer node metadata header This is used to fill metrics and log labels.
struct NodeInfo {
  // Name of the node. e.g. in k8s, name is the pod name.
  std::string name;

  // Namespaace that the node runs in.
  std::string namespace_name;

  // K8s workload attributes.
  std::unordered_map<int, std::string> ports_containers_map;
  std::string owner;
  std::string workload_name;

  // GCP project and k8s cluster metadata, used to fill in metric monitored
  // resource.
  std::string project_id;
  std::string cluster_name;
  std::string location;
};

// RequestInfo represents the information collected from filter stream
// callbacks. This is used to write metrics and logs.
struct RequestInfo {
  // Start timestamp in nanoseconds.
  int64_t start_timestamp = 0;

  // End timestamp in nanoseconds.
  int64_t end_timestamp = 0;

  // Request total size in bytes, include header, body, and trailer.
  int64_t request_size = 0;

  // Response total size in bytes, include header, body, and trailer.
  int64_t response_size = 0;

  // Node information of the peer that the request sent to or came from.
  NodeInfo peer_node_info;

  // Destination port that the request targets.
  int64_t destination_port = 0;

  // Protocol used the request (HTTP/1.1, gRPC, etc).
  std::string request_protocol;

  // Response code of the request.
  int64_t response_code;

  // Host of destination copied from :host or :authority header.
  std::string destination_service_host;

  // Opeartion of the request, i.e. HTTP method or gRPC API method.
  std::string request_operation;

  // Indicates if the request uses mTLS.
  bool mTLS = false;

  // Principal of source and destination workload extracted from TLS
  // certificate.
  std::string source_principal;
  std::string destination_principal;
};

// Extracts NodeInfo from node metadata.
bool ExtractNodeMetadata(const google::protobuf::Value &metadata,
                         NodeInfo *node_info);

} // namespace Common
} // namespace Stackdriver
