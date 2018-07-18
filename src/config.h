// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#ifndef FUTURES_CELL_PROXY_SRC_CONFIG_H
#define FUTURES_CELL_PROXY_SRC_CONFIG_H

#include <string>
#include "thirdparty/gflags/gflags.h"

DECLARE_string(listen_ip);
DECLARE_string(listen_port);

DECLARE_int32(m_retry_times);
DECLARE_int32(rpc_call_times);

DECLARE_int32(conn_timeout_ms);
DECLARE_int32(send_timeout_ms);
DECLARE_int32(recv_timeout_ms);
// sp_trade_info ip:port
DECLARE_string(sp_trade_info_server_ip);
DECLARE_int32(sp_trade_info_server_port);
// 期货开户状态URL
DECLARE_string(futures_account_open_url);
// 搜索URL
DECLARE_string(futures_search_url);

DECLARE_string(buy_icon_url);
DECLARE_string(sell_icon_url);
DECLARE_string(fund_in_icon_url);
DECLARE_string(fund_out_icon_url);

// authen_user_server ip:port
DECLARE_string(authen_server_ip);
DECLARE_int32(authen_server_port);
DECLARE_string(futures_fund_in_url);
DECLARE_string(futures_fund_out_url);
#endif // FUTURES_CELL_PROXY_SRC_CONFIG_H

