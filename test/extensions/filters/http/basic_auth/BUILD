licenses(["notice"])  # Apache 2

load(
    "@envoy//bazel:envoy_build_system.bzl",
    "envoy_cc_test",
)

envoy_cc_test(
    name = "basic_auth_test",
    srcs = ["basic_auth_test.cc"],
    repository = "@envoy",
    deps = [
        "//source/extensions/filters/http/basic_auth",
        "@envoy//test/mocks/http:http_mocks",
    ],
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

envoy_cc_test(
    name = "config_test",
    srcs = ["config_test.cc"],
    repository = "@envoy",
    deps = [
        "//source/extensions/filters/http/basic_auth:config",
        "@envoy//test/mocks/server:server_mocks",
    ],
)