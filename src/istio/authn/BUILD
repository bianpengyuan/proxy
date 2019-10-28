load("@io_bazel_rules_go//go:def.bzl", "go_library")
load("@io_bazel_rules_go//proto:def.bzl", "go_proto_library")

# Copyright 2018 Istio Authors. All Rights Reserved.
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

package(default_visibility = ["//visibility:public"])

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_proto_library",
)

envoy_proto_library(
    name = "context_proto",
    srcs = ["context.proto"],
    external_deps = ["well_known_protos"],
)

proto_library(
    name = "istio_authn_proto",
    srcs = ["context.proto"],
    deps = ["@com_google_protobuf//:struct_proto"],
)

go_proto_library(
    name = "istio_authn_go_proto",
    importpath = "istio.io/proxy/src/istio/authn",
    proto = ":istio_authn_proto",
)

go_library(
    name = "go_default_library",
    embed = [":istio_authn_go_proto"],
    importpath = "istio.io/proxy/src/istio/authn",
)
