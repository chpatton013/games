workspace(name="gamez")

new_git_repository(
    name="assimp_git",
    remote="https://github.com/assimp/assimp.git",
    tag="v3.3.1",
    build_file="third_party/BUILD.assimp",
)
bind(name="assimp", actual="@assimp_git//:assimp")

new_http_archive(
    name="boost_http",
    url="https://sourceforge.net/projects/boost/files/boost/1.62.0/boost_1_62_0.tar.gz",
    sha256="440a59f8bc4023dbe6285c9998b0f7fa288468b889746b1ef00e8b36c559dce1",
    build_file="third_party/BUILD.boost",
    strip_prefix="boost_1_62_0/",
)
bind(name="boost", actual="@boost_http//:boost")

new_git_repository(
    name="gflags_git",
    remote="https://github.com/gflags/gflags.git",
    tag="v2.1.2",
    build_file="third_party/BUILD.gflags",
)
bind(name="gflags", actual="@gflags_git//:gflags")

new_git_repository(
    name="glfw_git",
    remote="https://github.com/glfw/glfw.git",
    tag="3.2.1",
    build_file="third_party/BUILD.glfw",
)
bind(name="glfw", actual="@glfw_git//:glfw")

new_git_repository(
    name="glm_git",
    remote="https://github.com/g-truc/glm.git",
    tag="0.9.8.1",
    build_file="third_party/BUILD.glm",
)
bind(name="glm", actual="@glm_git//:glm")

new_git_repository(
    name="glog_git",
    remote="https://github.com/google/glog",
    tag="v0.3.4",
    build_file="third_party/BUILD.glog",
)
bind(name="glog", actual="@glog_git//:glog")

new_git_repository(
    name="gtest_git",
    remote="https://github.com/google/googletest.git",
    tag="release-1.8.0",
    build_file="third_party/BUILD.gtest",
)
bind(name="gtest", actual="@gtest_git//:gtest")
bind(name="gtest_main", actual="@gtest_git//:gtest_main")
bind(name="gmock", actual="@gtest_git//:gmock")
bind(name="gmock_main", actual="@gtest_git//:gmock_main")
