package(default_visibility=["//visibility:public"])

cc_library(
    name="task",
    srcs=["task.h"],
    hdrs=["task.h"],
    deps=[
        "//util:delegate",
        "//util:identifier",
        "//external:boost",
    ],
)

cc_library(
    name="registry",
    srcs=["registry.h"],
    hdrs=["registry.h"],
    deps=[
        ":task",
        "//external:boost",
    ],
)

cc_library(
    name="schedule",
    srcs=["schedule.h"],
    hdrs=["schedule.h"],
    deps=[
        ":task",
        ":registry",
        "//external:boost",
    ],
)

cc_test(
    name="task_test",
    srcs=["task_test.cpp"],
    deps=[
        ":task",
        "//external:gtest_main",
    ],
)

cc_test(
    name="registry_test",
    srcs=["registry_test.cpp"],
    deps=[
        ":registry",
        "//external:gtest_main",
    ],
)

cc_test(
    name="schedule_test",
    srcs=["schedule_test.cpp"],
    deps=[
        ":schedule",
        "//external:gtest_main",
    ],
)
