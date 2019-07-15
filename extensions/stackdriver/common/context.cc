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

#include "extensions/stackdriver/common/context.h"
#include "extensions/stackdriver/common/constants.h"

namespace Stackdriver {
namespace Common {

void FillNodeMetadataField(
    const google::protobuf::Map<std::string, google::protobuf::Value> &metadata,
    const std::string &name, std::string *field) {
  auto iter = metadata.find(name);
  if (iter == metadata.end()) {
    return;
  }
  *field = iter->second.string_value();
}

bool ExtractNodeMetadata(const google::protobuf::Value &metadata,
                         NodeInfo *node_info_) {
  const auto &node_metadata_struct = metadata.struct_value();
  if (node_metadata_struct.fields().empty()) {
    return false;
  }

  const auto &node_metadata_fields = node_metadata_struct.fields();
  auto iter = node_metadata_fields.find(kIstioMetadata);
  if (iter == node_metadata_fields.end()) {
    return false;
  }

  const auto &istio_metadata_struct = iter->second.struct_value();
  if (istio_metadata_struct.fields().empty()) {
    return false;
  }
  const auto &istio_metadata_fields = istio_metadata_struct.fields();

  FillNodeMetadataField(istio_metadata_fields, kMetadataPodNameKey,
                        &node_info_->name);
  FillNodeMetadataField(istio_metadata_fields, kMetadataNamespaceKey,
                        &node_info_->namespace_name);
  FillNodeMetadataField(istio_metadata_fields, kMetadataOwnerKey,
                        &node_info_->owner);
  FillNodeMetadataField(istio_metadata_fields, kMetadataWorkloadNameKey,
                        &node_info_->workload_name);

  return true;
}

}  // namespace Common
}  // namespace Stackdriver
