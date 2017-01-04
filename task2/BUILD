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
    name="schedule",
    srcs=["schedule.h"],
    hdrs=["schedule.h"],
    deps=[
        ":task",
        "//external:boost",
    ],
)
