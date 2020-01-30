

package(default_visibility = ["//visibility:public"])

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
)

envoy_cc_library(
    name = "sink",
    srcs = ["sink.cc"],
    hdrs = ["sink.h"],
    repository = "@envoy",
    deps = [
        "//extensions/stackdriver/config/v1alpha1:stackdriver_stats_sink_config_cc_proto",
        "@envoy//source/server:configuration_lib",
        "@io_opencensus_cpp//opencensus/stats",
    #     "//include/envoy/registry",
    #     "//source/common/network:address_lib",
    #     "//source/common/network:resolver_lib",
    #     "//source/extensions/stat_sinks:well_known_names",
    #     "//source/extensions/stat_sinks/hystrix:hystrix_lib",
    #     "//source/server:configuration_lib",
    #     "@envoy_api//envoy/config/metrics/v3:pkg_cc_proto",
    ],
)

# envoy_cc_library(
#     name = "hystrix_lib",
#     srcs = ["hystrix.cc"],
#     hdrs = ["hystrix.h"],
#     deps = [
#         "//include/envoy/server:admin_interface",
#         "//include/envoy/server:instance_interface",
#         "//include/envoy/stats:stats_interface",
#         "//source/common/buffer:buffer_lib",
#         "//source/common/common:logger_lib",
#         "//source/common/config:well_known_names",
#         "//source/common/http:headers_lib",
#         "//source/common/stats:symbol_table_lib",
#         "//source/common/stats:utility_lib",
#     ],
# )
