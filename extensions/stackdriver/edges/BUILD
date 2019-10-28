load("@io_bazel_rules_go//go:def.bzl", "go_library")
load("@io_bazel_rules_go//proto:def.bzl", "go_proto_library")

# Copyright 2019 Istio Authors. All Rights Reserved.
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

licenses(["notice"])

cc_proto_library(
    name = "edges_cc_proto",
    visibility = ["//visibility:public"],
    deps = ["edges_proto"],
)

proto_library(
    name = "edges_proto",
    srcs = ["edges.proto"],
    deps = [
        "@com_google_protobuf//:timestamp_proto",
    ],
)

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_cc_test",
)

envoy_cc_library(
    name = "mesh_edges_service_client",
    srcs = [
        "mesh_edges_service_client.cc",
    ],
    hdrs = [
        "mesh_edges_service_client.h",
    ],
    repository = "@envoy",
    visibility = ["//visibility:public"],
    deps = [
        ":edges_cc_proto",
        "//extensions/common:context",
        "@envoy//source/extensions/common/wasm/null:null_plugin_lib",
    ],
)

envoy_cc_library(
    name = "edge_reporter",
    srcs = [
        "edge_reporter.cc",
    ],
    hdrs = [
        "edge_reporter.h",
    ],
    repository = "@envoy",
    visibility = ["//visibility:public"],
    deps = [
        ":edges_cc_proto",
        ":mesh_edges_service_client",
        "//extensions/common:context",
        "//extensions/stackdriver/common:constants",
        "@envoy//source/extensions/common/wasm/null:null_plugin_lib",
    ],
)

envoy_cc_test(
    name = "edge_reporter_test",
    size = "small",
    srcs = ["edge_reporter_test.cc"],
    repository = "@envoy",
    deps = [
        ":edge_reporter",
        "@envoy//source/extensions/common/wasm:wasm_lib",
    ],
)

proto_library(
    name = "google_cloud_meshtelemetry_v1alpha1_proto",
    srcs = ["edges.proto"],
    visibility = ["//visibility:public"],
    deps = ["@com_google_protobuf//:timestamp_proto"],
)

go_proto_library(
    name = "google_cloud_meshtelemetry_v1alpha1_go_proto",
    compilers = ["@io_bazel_rules_go//proto:go_grpc"],
    importpath = "istio.io/proxy/extensions/stackdriver/edges",
    proto = ":google_cloud_meshtelemetry_v1alpha1_proto",
    visibility = ["//visibility:public"],
)

go_library(
    name = "go_default_library",
    srcs = [
        "edge_reporter.h",
        "mesh_edges_service_client.h",
    ],
    embed = [":google_cloud_meshtelemetry_v1alpha1_go_proto"],
    importpath = "istio.io/proxy/extensions/stackdriver/edges",
    visibility = ["//visibility:public"],
)
