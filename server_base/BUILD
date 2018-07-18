cc_library (
  name = "server_base",
  srcs = [
  "connection.cc",
  "connection_manager.cc",
  "handler.cc",
  "reply.cpp",
  "request_parser.cpp",
  "server_base.cc",
  "helper.cc",
  ],
  deps = [
    "//common/time_cost:time_cost",
    "//thirdparty/boost:boost_system",
    "//thirdparty/boost:boost_thread",
    "//thirdparty/glog:glog",
    "//thirdparty/gflags:gflags",
    "//thirdparty/service_monitor/sender:info_collector",
    "#pthread",
    "#rt",
  ],
)
