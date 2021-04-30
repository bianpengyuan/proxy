# Copyright 2016 Istio Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
################################################################################
#

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_cc_test",
)

envoy_cc_library(
    name = "authn_lib",
    srcs = [
        "authn.cc",
    ],
    hdrs = [
        "authn.h",
    ],
    repository = "@envoy",
    visibility = ["//visibility:public"],
    deps = [
        ":filter_names_lib",
        ":utils_lib",
        "//src/istio/authn:context_proto_cc_proto",
        "//src/istio/utils:attribute_names_lib",
        "//src/istio/utils:utils_lib",
        "@envoy//source/exe:envoy_common_lib",
    ],
)

envoy_cc_library(
    name = "utils_lib",
    srcs = [
        "utils.cc",
    ],
    hdrs = [
        "trace_headers.h",
        "utils.h",
    ],
    repository = "@envoy",
    visibility = ["//visibility:public"],
    deps = [
        "//extensions/common:json_util",
        "@envoy//source/exe:envoy_common_lib",
    ],
)

envoy_cc_test(
    name = "authn_test",
    srcs = [
        "authn_test.cc",
    ],
    repository = "@envoy",
    deps = [
        ":authn_lib",
        "@envoy//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "utils_test",
    srcs = [
        "utils_test.cc",
    ],
    repository = "@envoy",
    deps = [
        ":utils_lib",
        "@envoy//test/mocks/network:network_mocks",
        "@envoy//test/mocks/ssl:ssl_mocks",
        "@envoy//test/mocks/stream_info:stream_info_mocks",
        "@envoy//test/test_common:utility_lib",
    ],
)

cc_library(
    name = "filter_names_lib",
    srcs = [
        "filter_names.cc",
    ],
    hdrs = [
        "filter_names.h",
    ],
    visibility = ["//visibility:public"],
)

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_cc_test",
)

package(
    default_visibility = [
        "//src/envoy:__subpackages__",
    ],
)

envoy_cc_library(
    name = "json_struct_lib",
    srcs = ["json_struct.cc"],
    hdrs = ["json_struct.h"],
    repository = "@envoy",
    deps = [
        "@envoy//source/common/buffer:buffer_lib",
        "@envoy//source/common/grpc:status_lib",
        "@envoy//source/common/protobuf:utility_lib",
    ],
)

envoy_cc_test(
    name = "json_struct_test",
    srcs = [
        "json_struct_test.cc",
    ],
    repository = "@envoy",
    deps = [
        ":json_struct_lib",
        "@envoy//test/test_common:utility_lib",
    ],
)

envoy_cc_library(
    name = "filter_state_utils_lib",
    srcs = [
        "filter_state_utils.cc",
    ],
    hdrs = [
        "filter_state_utils.h",
    ],
    repository = "@envoy",
    deps = [
        "@envoy//source/common/common:empty_string",
        "@envoy//source/common/router:string_accessor_lib",
        "@envoy//source/exe:envoy_common_lib",
    ],
)

envoy_cc_test(
    name = "filter_state_utils_test",
    srcs = ["filter_state_utils_test.cc"],
    repository = "@envoy",
    deps = [
        ":filter_state_utils_lib",
        "@envoy//source/common/common:empty_string",
        "@envoy//source/common/stream_info:filter_state_lib",
        "@envoy//test/test_common:utility_lib",
    ],
)

envoy_cc_library(
    name = "http_header_utils_lib",
    srcs = ["http_header_utils.cc"],
    hdrs = ["http_header_utils.h"],
    repository = "@envoy",
    deps = [
        "@envoy//source/common/common:empty_string",
        "@envoy//source/exe:envoy_common_lib",
    ],
)

envoy_cc_library(
    name = "rc_detail_utils_lib",
    srcs = ["rc_detail_utils.cc"],
    hdrs = ["rc_detail_utils.h"],
    repository = "@envoy",
)

envoy_cc_test(
    name = "http_header_utils_test",
    srcs = ["http_header_utils_test.cc"],
    repository = "@envoy",
    deps = [
        ":http_header_utils_lib",
        "@envoy//source/common/common:empty_string",
        "@envoy//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "rc_detail_utils_test",
    srcs = ["rc_detail_utils_test.cc"],
    repository = "@envoy",
    deps = [
        "rc_detail_utils_lib",
    ],
)
