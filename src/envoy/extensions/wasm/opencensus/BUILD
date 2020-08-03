# Copyright 2020 Istio Authors. All Rights Reserved.
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
    "envoy_proto_library",
)

envoy_cc_library(
    name = "opencensus_lib",
    hdrs = [
        "opencensus_context.h",
        "opencensus_impl.h",
        "opencensus_null.h",
    ],
    srcs = [
        "opencensus_context.cc",
        "opencensus_impl.cc",
    ],
    repository = "@envoy",
    visibility = ["//visibility:public"],
    deps = [
        ":opencensus_spec_cc_proto",
        "//src/envoy/extensions/wasm:context",
        "@envoy//source/extensions/common/wasm:wasm_hdr",
        "@envoy//source/extensions/common/wasm:wasm_lib",
        "@io_opencensus_cpp//opencensus/exporters/stats/stackdriver:stackdriver_exporter",
        "@io_opencensus_cpp//opencensus/stats",
        "@proxy_wasm_cpp_host//:lib",
    ],
)

cc_library(
    name = "opencensus_wasm",
    hdrs = [
        "opencensus_wasm.h",
    ],
    visibility = ["//visibility:public"],
)

cc_proto_library(
    name = "opencensus_spec_cc_proto",
    deps = [
        ":opencensus_spec",
    ],
    visibility = ["//visibility:public"],
)

proto_library(
    name = "opencensus_spec",
    srcs = ["opencensus_spec.proto"],
    deps = [
        "@proxy_wasm_cpp_sdk//:proxy_wasm_intrinsics_proto",
        "@com_google_googleapis//google/api:distribution_proto",
        "@com_google_googleapis//google/api:monitored_resource_proto",
        "@com_google_googleapis//google/monitoring/v3:monitoring_proto",
    ],
)
