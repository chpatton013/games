EXTERNAL_PATH = "external/glog"
BUILD_DIR = "glog_build"
BUILD_PATH = "%s/%s" % (EXTERNAL_PATH, BUILD_DIR)

def path_to_name(path):
    return "glog_" + path.replace("/", "_").replace(".", "_")

def path_to_out(path):
    return path.partition(BUILD_PATH)[2].strip("/")
