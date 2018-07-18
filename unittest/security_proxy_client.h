// Copyright (c) 2017, Formax Inc. All rights reserved.

#ifndef FUTURES_CELL_PROXY_UNITTEST_SECURITY_PROXY_CLIENT_H
#define FUTURES_CELL_PROXY_UNITTEST_SECURITY_PROXY_CLIENT_H

#include <string>

#include "futures_cell_proxy/proto/futures_service.pb.h"
#include "futures_cell_proxy/unittest/common.h"
#include "thirdparty/curl/curl.h"
#include "thirdparty/glog/logging.h"


#define DefineHttpFunc(func_name, req_type, resp_type) \
bool func_name(const req_type& req, resp_type* resp) { \
  std::string req_str; \
  req.SerializeToString(&req_str); \
  \
  std::string resp_str; \
  if (!common::HttpGet(FLAGS_server_ip, FLAGS_server_port, 10, #func_name, req_str, &resp_str)) { \
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

using namespace futures::pb::protocols;
namespace security { namespace pb { namespace protocols {

class SecurityProxyClient {
 public:
  static SecurityProxyClient* Instance() {
    static SecurityProxyClient s_instance;
    return &s_instance;
  }

  DefineHttpFunc(QueryMyFuturesAssets, QueryMyFuturesAssetsRequest, QueryMyFuturesAssetsReturn);
  DefineHttpFunc(FuturesTrade, FuturesTradeRequest, FuturesTradeReturn);
  DefineHttpFunc(QueryUserStatus, QueryUserStatusRequest, QueryUserStatusReturn);
  DefineHttpFunc(QueryPendingOrders, QueryPendingOrdersRequest, QueryPendingOrdersReturn);
  DefineHttpFunc(QueryHoldingOrders, QueryHoldingOrdersRequest, QueryHoldingOrdersReturn);
  DefineHttpFunc(QueryHistoryOrders, QueryHistoryOrderRequest, QueryHistoryOrderReturn);
  DefineHttpFunc(QueryOrderDetail, QueryOrderDetailRequest, QueryOrderDetailReturn);
  DefineHttpFunc(QueryTransactionRecord, QueryTransactionRecordRequest, QueryTransactionRecordReturn);
  DefineHttpFunc(QueryFuturesAccountConfig, QueryFuturesAcconutConfigRequest, QueryFuturesAccountConfigReturn);
  DefineHttpFunc(CancelPendingOrder, CancelPendingOrderRequest, FututesCancelPendingOrderReturn);
  // 搜索相关接口
  DefineHttpFunc(QuerySymbolByName, QuerySymbolByNameRequest, QuerySymbolByNameReturn);
  DefineHttpFunc(TradeConfirmInfo, TradeConfirmInfoRequest, TradeConfirmInfoReturn);

  DefineHttpFunc(QuerySymbolTradeInfo, QuerySymbolTradeInfoRequest, QuerySymbolTradeInfoReturn);
  DefineHttpFunc(QueryFifthOrder, QueryFifthOrderRequest, QueryFifthOrderReturn);

  // selftest 2/2 add interface wrapper here

 private:
  SecurityProxyClient() {
  }
  ~SecurityProxyClient() {
  }
};
} // namespace protocols
} // namespace pb
} // namespace security
#endif // FUTURES_CELL_PROXY_UNITTEST_SECURITY_PROXY_CLIENT_H
