cc_binary (
  name = "futures_cell_proxy",
  srcs=[
    "main.cc",
    "config.cc",
    "request_handler.cc",
    "futures_proxy.cc",
    "futures_convert.cc",
    "rpc/sp_trade_info_pool.cc",
    "rpc/sp_trade_info_wrapper.cc",
    "rpc/authen_client_pool.cc",
    "rpc/authen_user_server_wrapper.cc",
  ],
  deps=[
    "//futures_cell_proxy/server_base:server_base",
    "//common/time_cost:time_cost",
    "//futures_cell_proxy/common:futures_common",

    "//futures_cell_proxy/proto:futures_service_proto",
    "//protocols:common",
    "//protocols:sp_trade_info",
    "//protocols:authentication_def",

    "//thirdparty/thrift:thriftnb",
    "//thirdparty/thrift:thrift",
    "#pthread",
    "#rt",
    "#idn",
    "#libz",
  ]
)
