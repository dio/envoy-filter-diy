licenses(["notice"])  # Apache 2

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_test",
)

envoy_cc_test(
    name = "basic_auth_integration_test",
    srcs = ["basic_auth_integration_test.cc"],
    repository = "@envoy",
    deps = [
        "//source/extensions/filters/http/basic_auth:config",
        "@envoy//test/integration:http_integration_lib",
    ],
)