// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#include "futures_cell_proxy/src/config.h"
#include "thirdparty/gflags/gflags.h"

DEFINE_string(listen_ip, "0.0.0.0", "futures_cell_proxy 服务ip");
DEFINE_string(listen_port, "8877", "futures_cell_proxy 服务port");

DEFINE_int32(m_retry_times, 3, "client retry times");
DEFINE_int32(rpc_call_times, 1, "rpc call times");

DEFINE_int32(conn_timeout_ms, 10000, "default conn timeout for ecn proxy");
DEFINE_int32(send_timeout_ms, 10000, "default send timeout for ecn proxy");
DEFINE_int32(recv_timeout_ms, 10000, "default recv timeout for ecn proxy");
// sp_trade_info ip:port
DEFINE_string(sp_trade_info_server_ip, "please checkout the config file", "sp_trade_info 服务ip");
DEFINE_int32(sp_trade_info_server_port, 0, "sp_trade_info 服务port");
// 期货开户状态URL
DEFINE_string(futures_account_open_url, "please checkout the config file", "期货开户状态URL");
DEFINE_string(futures_search_url, "please checkout the config file", "期货搜索http服务URL");
// 配置ICON
DEFINE_string(buy_icon_url, "please checkout the config file", "买入的 ICON URL");
DEFINE_string(sell_icon_url, "please checkout the config file", "卖出的 ICON URL");
DEFINE_string(fund_in_icon_url, "please checkout the config file", "入金的 ICON URL");
DEFINE_string(fund_out_icon_url, "please checkout the config file", "出金的 ICON URL");

DEFINE_string(authen_server_ip, "please checkout the config file", "authen_user_server 服务ip");
DEFINE_int32(authen_server_port, 0, "authen_user_server 服务port");
DEFINE_string(futures_fund_in_url, "please checkout the config file", "入金的URL");
DEFINE_string(futures_fund_out_url, "please checkout the config file", "出金的URL");
