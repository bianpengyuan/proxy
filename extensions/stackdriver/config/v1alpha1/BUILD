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

cc_proto_library(
    name = "stackdriver_plugin_config_cc_proto",
    visibility = [
        "//extensions/stackdriver:__pkg__",
        "//extensions/stackdriver/metric:__pkg__",
    ],
    deps = ["stackdriver_plugin_config_proto"],
)

proto_library(
    name = "stackdriver_plugin_config_proto",
    srcs = ["stackdriver_plugin_config.proto"],
    deps = [
        "@com_google_protobuf//:duration_proto",
    ],
)

proto_library(
    name = "stackdriver_config_v1alpha1_proto",
    srcs = ["stackdriver_plugin_config.proto"],
    visibility = ["//visibility:public"],
    deps = ["@com_google_protobuf//:duration_proto"],
)

go_proto_library(
    name = "stackdriver_config_v1alpha1_go_proto",
    compilers = ["@io_bazel_rules_go//proto:go_grpc"],
    importpath = "istio.io/proxy/extensions/stackdriver/config/v1alpha1",
    proto = ":stackdriver_config_v1alpha1_proto",
    visibility = ["//visibility:public"],
)

go_library(
    name = "go_default_library",
    embed = [":stackdriver_config_v1alpha1_go_proto"],
    importpath = "istio.io/proxy/extensions/stackdriver/config/v1alpha1",
    visibility = ["//visibility:public"],
)
