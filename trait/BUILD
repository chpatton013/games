package(default_visibility = ["//visibility:public"])

cc_library(
    name="trait",
    hdrs=["trait.h"],
)

cc_library(
    name="eq_ops",
    hdrs=["eq_ops.h"],
    deps=[":trait"],
)

cc_test(
    name="eq_ops_test",
    srcs=["eq_ops_test.cpp"],
    deps=[
        ":eq_ops",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name="rel_ops",
    hdrs=["rel_ops.h"],
    deps=[":trait"],
)

cc_test(
    name="rel_ops_test",
    srcs=["rel_ops_test.cpp"],
    deps=[
        ":rel_ops",
        "@gtest//:gtest_main",
    ],
)

cc_library(
    name="to_string",
    hdrs=["to_string.h"],
    deps=[":trait"],
)

cc_test(
    name="to_string_test",
    srcs=["to_string_test.cpp"],
    deps=[
        ":to_string",
        "@gtest//:gtest_main",
    ],
)
