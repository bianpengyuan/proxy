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
#include "google/protobuf/struct.pb.h"
#include "gtest/gtest.h"

namespace Stackdriver {
namespace Common {

// Test ExtraceNodeMetadata with all possible available metadata.
TEST(RegistryTest, ExtractNodeMetadata) {
  google::protobuf::Value istio_metadata;
  auto metadata_struct = istio_metadata.mutable_struct_value();
  auto metadata_fields = metadata_struct->mutable_fields();

  google::protobuf::Value pod_name;
  *pod_name.mutable_string_value() = "test_pod";
  (*metadata_fields)[kMetadataPodNameKey] = pod_name;
  google::protobuf::Value namespace_name;
  *namespace_name.mutable_string_value() = "test_namespace";
  (*metadata_fields)[kMetadataNamespaceKey] = namespace_name;
  google::protobuf::Value workload_name;
  *workload_name.mutable_string_value() = "test_workload";
  (*metadata_fields)[kMetadataWorkloadNameKey] = workload_name;
  google::protobuf::Value owner;
  *owner.mutable_string_value() = "test_owner";
  (*metadata_fields)[kMetadataOwnerKey] = owner;

  google::protobuf::Value node_metadata;
  (*node_metadata.mutable_struct_value()->mutable_fields())[kIstioMetadata] =
      istio_metadata;

  NodeInfo node_info;
  bool success = ExtractNodeMetadata(node_metadata, &node_info);
  EXPECT_TRUE(success);
  EXPECT_EQ(node_info.name, "test_pod");
  EXPECT_EQ(node_info.namespace_name, "test_namespace");
  EXPECT_EQ(node_info.owner, "test_owner");
  EXPECT_EQ(node_info.workload_name, "test_workload");
}

// Test Istio metadata missing from node metadata.
TEST(RegistryTest, ExtractNodeMetadataNoMetadataField) {
  google::protobuf::Value node_metadata;
  NodeInfo node_info;
  bool success = ExtractNodeMetadata(node_metadata, &node_info);
  EXPECT_FALSE(success);
  EXPECT_EQ(node_info.name, "");
  EXPECT_EQ(node_info.namespace_name, "");
  EXPECT_EQ(node_info.owner, "");
  EXPECT_EQ(node_info.workload_name, "");
}

// Test wrong type of Istio metadata.
TEST(RegistryTest, ExtractNodeMetadataMetadataFieldNotStruct) {
  google::protobuf::Value metadata_string;
  *metadata_string.mutable_string_value() = "some_string_metadata";

  google::protobuf::Value node_metadata;
  (*node_metadata.mutable_struct_value()->mutable_fields())[kIstioMetadata] =
      metadata_string;

  NodeInfo node_info;
  bool success = ExtractNodeMetadata(node_metadata, &node_info);
  EXPECT_FALSE(success);
  EXPECT_EQ(node_info.name, "");
  EXPECT_EQ(node_info.namespace_name, "");
  EXPECT_EQ(node_info.owner, "");
  EXPECT_EQ(node_info.workload_name, "");
}

// Test missing Istio metadata fields.
TEST(RegistryTest, ExtractNodeMetadataFieldNotFound) {
  google::protobuf::Value istio_metadata;
  auto metadata_struct = istio_metadata.mutable_struct_value();
  auto metadata_fields = metadata_struct->mutable_fields();

  google::protobuf::Value pod_name;
  *pod_name.mutable_string_value() = "test_pod";
  (*metadata_fields)[kMetadataPodNameKey] = pod_name;
  google::protobuf::Value namespace_name;
  *namespace_name.mutable_string_value() = "test_namespace";
  (*metadata_fields)[kMetadataNamespaceKey] = namespace_name;

  google::protobuf::Value node_metadata;
  (*node_metadata.mutable_struct_value()->mutable_fields())[kIstioMetadata] =
      istio_metadata;

  NodeInfo node_info;
  bool success = ExtractNodeMetadata(node_metadata, &node_info);
  EXPECT_TRUE(success);
  // For the missing fields, the value should just be empty string.
  EXPECT_EQ(node_info.name, "test_pod");
  EXPECT_EQ(node_info.namespace_name, "test_namespace");
  EXPECT_EQ(node_info.owner, "");
  EXPECT_EQ(node_info.workload_name, "");
}

}  // namespace Common
}  // namespace Stackdriver
