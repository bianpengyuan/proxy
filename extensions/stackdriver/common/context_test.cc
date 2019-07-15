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
#include "gtest/gtest.h"

namespace Stackdriver {
namespace Common {

TEST(RegistryTest, ExtractNodeMetadata) {
  google::protobuf::Value metadata;
  auto metadata_struct = metadata.mutable_struct_value();
  auto metadata_fields = metadata_struct.mutable_fields();
  
  
}

TEST(RegistryTest, ExtractNodeMetadataNoMetadataField) {
    
}

TEST(RegistryTest, ExtractNodeMetadataMetadataFieldNotStruct) {
    
}

TEST(RegistryTest, ExtractNodeMetadataFieldNotFound) {
  google::protobuf::Value metadata;
  auto metadata_struct = metadata.mutable_struct_value();
  auto metadata_fields = metadata_struct.mutable_fields();

}

} // namespace Common
} // namespace Stackdriver
