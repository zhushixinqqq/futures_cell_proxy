cc_test (
  name = "futures_cell_proxy_client",
  srcs = [
  "api_test.cc",
  "common.cc",
  "config.cc",
  "main.cc",
  ],
  deps = [
  "//thirdparty/cryptopp:cryptopp",
  '//thirdparty/curl:curl',
  "//thirdparty/glog:glog",
  "//thirdparty/gflags:gflags",

  "//futures_cell_proxy/proto:futures_service_proto",
  "//futures_cell_proxy/proto:push_service_proto",
  "//futures_cell_proxy/server_base:server_base",

  '#idn',
  "#rt",
  "#z",
  ]
)
