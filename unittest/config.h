// Copyright (c) 2017, Formax Inc. All rights reserved.

#ifndef FUTURES_CELL_PROXY_UNITTEST_CONFIG_H
#define FUTURES_CELL_PROXY_UNITTEST_CONFIG_H

#include "thirdparty/gflags/gflags.h"

DECLARE_string(server_ip);
DECLARE_int32(server_port);
DECLARE_bool(check_contiune_run);
DECLARE_string(push_server_ip);
DECLARE_int32(push_server_port);

DECLARE_int64(uid);
DECLARE_int64(mt4_id);

DECLARE_string(phone_number);
DECLARE_string(password);
DECLARE_string(password);
DECLARE_string(futures_id);
DECLARE_int32(order_id);

DECLARE_int32(tradeType);
DECLARE_string(symbol);
DECLARE_double(volume);
DECLARE_double(price);
DECLARE_string(search_key);
DECLARE_int32(start_index);
DECLARE_int32(batch_num);

bool ContinueRun();

#endif  // FUTURES_CELL_PROXY_UNITTEST_CONFIG_H
