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

load(
    "@envoy//bazel/wasm:wasm.bzl",
    "wasm_cc_binary",
)

wasm_cc_binary(
    name = "stats.wasm",
    srcs = glob(
        [
            "common/context.cc",
            "common/util.cc",
            "stats/plugin.cc",
        ],
    ) + glob(
        [
            "stats/*.h",
            "common/*.h",
        ],
    ),
    copts = ["-UNULL_PLUGIN"],
    deps = [
        "//extensions/common:json_util_wasm",
        "@com_google_absl_wasm//absl/strings",
        "@com_google_absl_wasm//absl/time",
        "@proxy_wasm_cpp_sdk//:proxy_wasm_intrinsics",
        "@proxy_wasm_cpp_sdk//contrib:contrib_lib",
    ],
)

wasm_cc_binary(
    name = "metadata_exchange.wasm",
    srcs = glob(
        [
            "common/context.cc",
            "common/util.cc",
            "common/proto_util.cc",
            "metadata_exchange/plugin.cc",
        ],
    ) + glob(
        [
            "common/*.h",
            "metadata_exchange/*.h",
        ],
    ),
    copts = ["-UNULL_PLUGIN"],
    deps = [
        "//extensions/metadata_exchange:declare_property_proto_cc",
        "//extensions/common:json_util_wasm",
        "@com_google_absl_wasm//absl/strings",
        "@com_google_absl_wasm//absl/time",
        "@proxy_wasm_cpp_sdk//:proxy_wasm_intrinsics_full",
        "@proxy_wasm_cpp_sdk//contrib:contrib_lib",
    ],
)


wasm_cc_binary(
    name = "attributegen.wasm",
    srcs = glob(
        [
            "common/context.cc",
            "common/util.cc",
            "attributegen/plugin.cc",
        ],
    ) + glob(
        [
            "common/*.h",
            "attributegen/*.h",
        ],
    ),
    copts = ["-UNULL_PLUGIN"],
    deps = [
        "//extensions/common:json_util_wasm",
        "@com_google_absl_wasm//absl/strings",
        "@com_google_absl_wasm//absl/time",
        "@proxy_wasm_cpp_sdk//:proxy_wasm_intrinsics_full",
        "@proxy_wasm_cpp_sdk//contrib:contrib_lib",
    ],
)
