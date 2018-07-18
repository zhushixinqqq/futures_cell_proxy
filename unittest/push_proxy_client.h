// Copyright (c) 2017, Formax Inc. All rights reserved.

#ifndef FUTURES_CELL_PROXY_UNITTEST_PUSH_PROXY_CLIENT_H
#define FUTURES_CELL_PROXY_UNITTEST_PUSH_PROXY_CLIENT_H

#include <string>

#include "thirdparty/cryptopp/include/aes.h"
#include "thirdparty/cryptopp/include/base64.h"
#include "thirdparty/cryptopp/include/modes.h"
#include "thirdparty/curl/curl.h"
#include "thirdparty/glog/logging.h"

#include "futures_cell_proxy/proto/push_service.pb.h"
#include "futures_cell_proxy/unittest/common.h"

using namespace cell_proxy::protocols;
using namespace futures::pb::protocols;

#define DefineHttpFunc(func_name, req_type, resp_type) \
bool func_name(const req_type& req, resp_type* resp) { \
  std::string req_str; \
  req.SerializeToString(&req_str); \
  \
  std::string resp_str; \
  if (!common::HttpGet(FLAGS_push_server_ip, FLAGS_push_server_port, 10, #func_name, req_str, &resp_str)) { \
    LOG(ERROR) << FUNC_NAME << " http get failed." << req_str; \
    return false; \
  } \
  if (!resp->ParseFromString(resp_str)) { \
    LOG(ERROR) << FUNC_NAME << " ParseFromString failed. req_str:" << req_str \
        << " resp_str:" << resp_str; \
    return false; \
  } \
  return true; \
}

// using namespace ::cell_proxy::protocols;
namespace security { namespace pb { namespace protocols {

class PushProxyClient {
 public:
  static PushProxyClient* Instance() {
    static PushProxyClient s_instance;
    return &s_instance;
  }

  DefineHttpFunc(Login, LoginInfoProto, LoginReturn);

 private:
  PushProxyClient() {
  }
  ~PushProxyClient() {
  }
};
} // namespace protocols
} // namespace pb
} // namespace security
#endif // FUTURES_CELL_PROXY_UNITTEST_PUSH_PROXY_CLIENT_H
