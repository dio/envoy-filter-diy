package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # Apache 2

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
)
load("@envoy_api//bazel:api_build_system.bzl", "api_proto_library")

envoy_cc_library(
    name = "basic_auth_filter_lib",
    srcs = ["basic_auth_filter.cc"],
    hdrs = ["basic_auth_filter.h"],
    repository = "@envoy",
    deps = [
        ":config_proto_cc",
        "@envoy//source/exe:envoy_common_lib",
    ],
)

envoy_cc_library(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    repository = "@envoy",
    deps = [
        ":basic_auth_filter_lib",
        "//source/extensions/filters/http:well_known_names",
        "@envoy//source/exe:envoy_common_lib",
    ],
)

api_proto_library(
    name = "config_proto",
    srcs = ["config.proto"],
)
