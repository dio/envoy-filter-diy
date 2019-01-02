package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # Apache 2

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
)

envoy_cc_library(
    name = "well_known_names",
    hdrs = ["well_known_names.h"],
    repository = "@envoy",
    deps = [
        "@envoy//source/common/config:well_known_names",
        "@envoy//source/common/singleton:const_singleton",
    ],
)
