// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#ifndef FUTURES_CELL_PROXY_SRC_REQUEST_HANDLER_H
#define FUTURES_CELL_PROXY_SRC_REQUEST_HANDLER_H
#include <string>

#include "common/time_cost/time_cost.h"
#include "futures_cell_proxy/common/define.h"
#include "futures_cell_proxy/proto/futures_service.pb.h"
#include "futures_cell_proxy/server_base/connection.h"
#include "futures_cell_proxy/server_base/handler.h"
#include "futures_cell_proxy/server_base/reply.h"
#include "futures_cell_proxy/server_base/request.h"


using http::server_base::ConnectionPtr;
using http::server_base::reply;
using http::server_base::request;
using namespace futures::pb::protocols;

namespace futures { namespace pb { namespace server {

class RequestHandler : public http::server_base::Handler {
 public:
  explicit RequestHandler(const std::string& doc_root);

  virtual void HandleRequest(ConnectionPtr conn, const request& req,
    reply& rep) {
    LOG(INFO) << __func__ << "request: " << req.uri;

    std::string request_path;
    if (!UrlDecode(req.uri, request_path)) {
      rep = reply::stock_reply(reply::bad_request);
      return;
    }

    ProccessReqFromHandle(conn, request_path, req, &rep);
  }

  // 用于测试echo
  inline void baseecho(const std::string& in_data, std::string* out) {
    *out = in_data;
  }

 public:
  // addapi step 1/3: 添加入口函数及ADD_HANDLE
  // 获取期货账户资产信息
  QueryMyFuturesAssetsReturn QueryMyFuturesAssets(const QueryMyFuturesAssetsRequest& req);
  ADD_HANDLE(QueryMyFuturesAssets, QueryMyFuturesAssetsRequest, QueryMyFuturesAssetsReturn);
  // 获取账户开户状态
  QueryUserStatusReturn QueryUserStatus(const QueryUserStatusRequest& req);
  ADD_HANDLE(QueryUserStatus, QueryUserStatusRequest, QueryUserStatusReturn);
  // 账户中心配置项
  QueryFuturesAccountConfigReturn QueryFuturesAccountConfig(const QueryFuturesAcconutConfigRequest& req);
  ADD_HANDLE(QueryFuturesAccountConfig, QueryFuturesAcconutConfigRequest, QueryFuturesAccountConfigReturn);
  // 获取委托订单
  QueryPendingOrdersReturn QueryPendingOrders(const QueryPendingOrdersRequest& req);
  ADD_HANDLE(QueryPendingOrders, QueryPendingOrdersRequest, QueryPendingOrdersReturn);
  // 获取持仓中订单
  QueryHoldingOrdersReturn QueryHoldingOrders(const QueryHoldingOrdersRequest& req);
  ADD_HANDLE(QueryHoldingOrders, QueryHoldingOrdersRequest, QueryHoldingOrdersReturn);
  // 获取订单详情
  QueryOrderDetailReturn QueryOrderDetail(const QueryOrderDetailRequest& req);
  ADD_HANDLE(QueryOrderDetail, QueryOrderDetailRequest, QueryOrderDetailReturn);
  // 获取资金流水
  QueryTransactionRecordReturn QueryTransactionRecord(const QueryTransactionRecordRequest& req);
  ADD_HANDLE(QueryTransactionRecord, QueryTransactionRecordRequest, QueryTransactionRecordReturn);
  // 下单交易
  FuturesTradeReturn FuturesTrade(const FuturesTradeRequest& req);
  ADD_HANDLE(FuturesTrade, FuturesTradeRequest, FuturesTradeReturn);
  // 撤单
  FututesCancelPendingOrderReturn CancelPendingOrder(const CancelPendingOrderRequest& req);
  ADD_HANDLE(CancelPendingOrder, CancelPendingOrderRequest, FututesCancelPendingOrderReturn);
  // 历史订单
  QueryHistoryOrderReturn QueryHistoryOrders(const QueryHistoryOrderRequest& req);
  ADD_HANDLE(QueryHistoryOrders, QueryHistoryOrderRequest, QueryHistoryOrderReturn);
  // 期货搜索相关接口
  // @1期货搜索界面，按类选择推荐 ---搜索界面
  SymbolTypeRecommendReturn SymbolTypeRecommend(const SymbolTypeRecommendRequest& req);
  ADD_HANDLE(SymbolTypeRecommend, SymbolTypeRecommendRequest, SymbolTypeRecommendReturn);
  // @2按类别查询期货列表 ---搜索界面更多
  SymbolListByTypeReturn SymbolListByType(const SymbolListByTypeRequest& req);
  ADD_HANDLE(SymbolListByType, SymbolListByTypeRequest, SymbolListByTypeReturn);
  // @3通过期货名称查询期货 ---搜索期货
  QuerySymbolByNameReturn QuerySymbolByName(const QuerySymbolByNameRequest& req);
  ADD_HANDLE(QuerySymbolByName, QuerySymbolByNameRequest, QuerySymbolByNameReturn);
  // 交易确认接口
  TradeConfirmInfoReturn TradeConfirmInfo(const TradeConfirmInfoRequest& req);
  ADD_HANDLE(TradeConfirmInfo, TradeConfirmInfoRequest, TradeConfirmInfoReturn);
  // 交易界面
  QuerySymbolTradeInfoReturn QuerySymbolTradeInfo(const QuerySymbolTradeInfoRequest& req);
  ADD_HANDLE(QuerySymbolTradeInfo, QuerySymbolTradeInfoRequest, QuerySymbolTradeInfoReturn);
  // 买卖五档
  QueryFifthOrderReturn QueryFifthOrder(const QueryFifthOrderRequest& req);
  ADD_HANDLE(QueryFifthOrder, QueryFifthOrderRequest, QueryFifthOrderReturn);
  // 期货报价
  QuerySymbolQuotationsReturn QuerySymbolQuotations(const QuerySymbolQuotationsRequest& req);
  ADD_HANDLE(QuerySymbolQuotations, QuerySymbolQuotationsRequest, QuerySymbolQuotationsReturn);
  // 分时
  FuturesGraphReturn FuturesGraph(const FuturesGraphRequest& req);
  ADD_HANDLE(FuturesGraph, FuturesGraphRequest, FuturesGraphReturn);
  // K线
  FuturesKLineReturn FuturesKLine(const FuturesKLineRequest& req);
  ADD_HANDLE(FuturesKLine, FuturesKLineRequest, FuturesKLineReturn);
 private:
  void Init();
};
} // namespace server
} // namespace pb
} // namespace futures
#endif // FUTURES_CELL_PROXY_SRC_REQUEST_HANDLER_H
