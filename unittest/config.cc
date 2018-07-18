// Copyright (c) 2017, Formax Inc. All rights reserved.

#include "futures_cell_proxy/unittest/config.h"

#include <iostream>

#include "thirdparty/curl/curl.h"

DEFINE_string(server_ip, "127.0.0.1", "server ip");
DEFINE_int32(server_port, 0, "server port");
DEFINE_bool(check_contiune_run, true, "check_contiune_run");
DEFINE_string(push_server_ip, "127.0.0.1", "push server ip");
DEFINE_int32(push_server_port, 0, "push server port");

DEFINE_int64(uid, 0, "uid");
DEFINE_int64(mt4_id, 0, "mt4_id");

DEFINE_string(phone_number, "", "phone_number");
DEFINE_string(password, "123456", "password");
DEFINE_string(futures_id, "000001", "期货ID");
DEFINE_int32(order_id, 100001, "订单号");

DEFINE_string(symbol, "100001", "code");
DEFINE_double(volume, 10, "数量");
DEFINE_double(price, 9.2, "价格");
DEFINE_int32(tradeType, 1, "交易类型");
DEFINE_string(search_key, "C", "搜索关键字");
DEFINE_int32(start_index, 0, "开始");
DEFINE_int32(batch_num, 20, "batch数量");

bool ContinueRun() {
  if (!FLAGS_check_contiune_run) {
    return true;
  }
  std::string confirm;
  while (true) {
    std::cout << "enter Y/N:";
    std::cin >> confirm;
    if (confirm == "N") return false;
    if (confirm == "Y") return true;
  }
}
