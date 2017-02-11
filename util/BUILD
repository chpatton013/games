package(default_visibility = ["//visibility:public"])

cc_library(
    name="macros",
    hdrs=["macros.h"],
)

cc_library(
    name="static_string",
    hdrs=["static_string.h"],
    deps=[
        "//trait:rel_ops",
        "//trait:to_string",
        "@glog//:glog",
    ],
)

cc_test(
    name="static_string_test",
    srcs=["static_string_test.cpp"],
    deps=[
        ":static_string",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name="expected",
    hdrs=["expected.h"],
    deps=[
        ":macros",
        "//trait:eq_ops",
        "//trait:to_string",
        "@boost_variant//:boost_variant",
        "@glog//:glog",
    ],
)

cc_test(
    name="expected_test",
    srcs=["expected_test.cpp"],
    deps=[
        ":expected",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name="result",
    hdrs=["result.h"],
    deps=[
        ":expected",
        ":static_string",
        "@glog//:glog",
    ],
)

cc_test(
    name="result_test",
    srcs=["result_test.cpp"],
    deps=[
        ":result",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name="result_or",
    hdrs=["result_or.h"],
    deps=[
        ":expected",
        ":result",
    ],
)

cc_library(
    name="identifier",
    hdrs=["identifier.h"],
)

cc_test(
    name="identifier_test",
    srcs=["identifier_test.cpp"],
    deps=[
        ":identifier",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name="defer",
    hdrs=["defer.h"],
    deps=["@gtest//:gtest_prod"],
)

cc_test(
    name="defer_test",
    srcs=["defer_test.cpp"],
    deps=[
        ":defer",
        "@gtest//:gtest_main",
    ],
)
