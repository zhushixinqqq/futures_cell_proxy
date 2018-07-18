// Copyright (c) 2017, Formax Inc. All rights reserved.

#ifndef FUTURES_CELL_PROXY_UNITTEST_COMMON_H
#define FUTURES_CELL_PROXY_UNITTEST_COMMON_H

#include <string>

#include "futures_cell_proxy/proto/futures_service.pb.h"

/////////////////////////////// 宏定义开始 ////////////////////////////////
//
#ifndef FUNC_NAME
#define FUNC_NAME \
  "[" << __func__ << "] " \

#endif  // FUNC_NAME

namespace futures { namespace pb { namespace protocols {
namespace common {
bool HttpGet(const std::string& server_ip, int32_t server_port,
  int32_t timeout_s, const std::string& func_name,
  const std::string& http_req_body, const std::string* http_resp_body);
} // namespace common
} // namespace protocols
} // namespace pb
} // namespace futures
#endif // FUTURES_CELL_PROXY_UNITTEST_COMMON_H
