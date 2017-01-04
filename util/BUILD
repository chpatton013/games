package(default_visibility = ["//visibility:public"])

cc_library(
    name="delegate",
    srcs=["delegate.h"],
    hdrs=["delegate.h"],
)

cc_library(
    name="expected",
    srcs=["expected.h"],
    hdrs=["expected.h"],
    deps=[
        "//external:boost",
        "//external:glog",
    ],
)

cc_test(
    name="expected_test",
    srcs=["expected_test.cpp"],
    deps=[
        ":expected",
        "//external:gtest_main",
    ],
)

cc_library(
    name="error",
    srcs=["error.h"],
    hdrs=["error.h"],
    deps=[
        ":expected",
        "//external:glog",
    ],
)

cc_test(
    name="error_test",
    srcs=["error_test.cpp"],
    deps=[
        ":error",
        "//external:gtest_main",
    ],
)

cc_library(
    name="rel_ops",
    srcs=["rel_ops.h"],
    hdrs=["rel_ops.h"],
)

cc_test(
    name="rel_ops_test",
    srcs=["rel_ops_test.cpp"],
    deps=[
        ":rel_ops",
        "//external:gtest_main",
    ],
)

cc_library(
    name="identifier",
    srcs=["identifier.h"],
    hdrs=["identifier.h"],
)

cc_test(
    name="identifier_test",
    srcs=["identifier_test.cpp"],
    deps=[
        ":identifier",
        "//external:gtest_main",
    ],
)
