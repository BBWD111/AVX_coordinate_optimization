load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")
load("//tools/bazel:helpers.bzl", "cc_test_if_exists")

cc_library(
    name = "lib",
    srcs = ["coordinate_transform.cpp"],
    hdrs = ["coordinate_transform.h"],
    copts = [
        "-mavx",
    ],
)

cc_test(
    name = "public_tests",
    srcs = ["test.cpp"],
    deps = [
        ":lib",
        "//tools/bazel:catch2",
    ],
    copts = [
        "-mavx",
    ],
)
