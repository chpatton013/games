def path_to_name(path):
    return "glog_" + path.replace("/", "_").replace(".", "_")

def path_to_out(path):
    return path.split("external/glog_git/glog_build/", 1)[1]
