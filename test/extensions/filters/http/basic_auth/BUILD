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