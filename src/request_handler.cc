// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#include "futures_cell_proxy/common/common.h"
#include "futures_cell_proxy/common/define.h"
#include "futures_cell_proxy/src/futures_proxy.h"
#include "futures_cell_proxy/src/request_handler.h"

// #include "security/cell_proxy/security/src/security_proxy_mgr.h"

using namespace ::futures::pb::protocols;

namespace futures { namespace pb { namespace server {

RequestHandler::RequestHandler(const std::string& doc_root) : Handler(doc_root) {
  Init();
}

void RequestHandler::Init() {
  // 注册到http监听索引
  REGISTE_HANDLE(QueryMyFuturesAssets);
  REGISTE_HANDLE(QueryUserStatus);
  REGISTE_HANDLE(QueryFuturesAccountConfig);
  REGISTE_HANDLE(QueryPendingOrders);
  REGISTE_HANDLE(QueryHoldingOrders);
  REGISTE_HANDLE(QueryOrderDetail);
  REGISTE_HANDLE(QueryTransactionRecord);
  REGISTE_HANDLE(FuturesTrade);
  REGISTE_HANDLE(CancelPendingOrder);
  REGISTE_HANDLE(QueryHistoryOrders);
  // 搜索
  REGISTE_HANDLE(SymbolTypeRecommend);
  REGISTE_HANDLE(SymbolListByType);
  REGISTE_HANDLE(QuerySymbolByName);
  REGISTE_HANDLE(TradeConfirmInfo);
  REGISTE_HANDLE(QuerySymbolTradeInfo);
  REGISTE_HANDLE(QueryFifthOrder);

  REGISTE_HANDLE(QuerySymbolQuotations);
  REGISTE_HANDLE(FuturesGraph);
  REGISTE_HANDLE(FuturesKLine);
  // addapi step 2/3: REGISTE_HANDLE 关联uri->func_name
}

QueryMyFuturesAssetsReturn RequestHandler::QueryMyFuturesAssets(
    const QueryMyFuturesAssetsRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QueryMyFuturesAssetsReturn _return;
  FuturesProxy::getInstance()->QueryMyFuturesAssets(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

QueryUserStatusReturn RequestHandler::QueryUserStatus(
    const QueryUserStatusRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QueryUserStatusReturn _return;
  FuturesProxy::getInstance()->QueryUserStatus(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

QueryFuturesAccountConfigReturn RequestHandler::QueryFuturesAccountConfig(
    const QueryFuturesAcconutConfigRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QueryFuturesAccountConfigReturn _return;
  FuturesProxy::getInstance()->QueryFuturesAccountConfig(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

QueryPendingOrdersReturn RequestHandler::QueryPendingOrders(
    const QueryPendingOrdersRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QueryPendingOrdersReturn _return;
  FuturesProxy::getInstance()->QueryPendingOrders(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

QueryHoldingOrdersReturn RequestHandler::QueryHoldingOrders(
    const QueryHoldingOrdersRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QueryHoldingOrdersReturn _return;
  FuturesProxy::getInstance()->QueryHoldingOrders(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

QueryOrderDetailReturn RequestHandler::QueryOrderDetail(
    const QueryOrderDetailRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QueryOrderDetailReturn _return;
  FuturesProxy::getInstance()->QueryOrderDetail(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

// 获取资金流水
QueryTransactionRecordReturn RequestHandler::QueryTransactionRecord(
    const QueryTransactionRecordRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QueryTransactionRecordReturn _return;
  FuturesProxy::getInstance()->QueryTransactionRecord(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

// 下单交易
FuturesTradeReturn RequestHandler::FuturesTrade(
    const FuturesTradeRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  FuturesTradeReturn _return;
  FuturesProxy::getInstance()->FuturesTrade(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

// 撤单
FututesCancelPendingOrderReturn RequestHandler::CancelPendingOrder(
    const CancelPendingOrderRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  FututesCancelPendingOrderReturn _return;
  FuturesProxy::getInstance()->CancelPendingOrder(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

// 历史订单
QueryHistoryOrderReturn RequestHandler::QueryHistoryOrders(
    const QueryHistoryOrderRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QueryHistoryOrderReturn _return;
  FuturesProxy::getInstance()->QueryHistoryOrders(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

// 搜索
SymbolTypeRecommendReturn RequestHandler::SymbolTypeRecommend(
    const SymbolTypeRecommendRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  SymbolTypeRecommendReturn _return;
  FuturesProxy::getInstance()->SymbolTypeRecommend(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

SymbolListByTypeReturn RequestHandler::SymbolListByType(
    const SymbolListByTypeRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  SymbolListByTypeReturn _return;
  FuturesProxy::getInstance()->SymbolListByType(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

QuerySymbolByNameReturn RequestHandler::QuerySymbolByName(
    const QuerySymbolByNameRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QuerySymbolByNameReturn _return;
  FuturesProxy::getInstance()->QuerySymbolByName(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

TradeConfirmInfoReturn RequestHandler::TradeConfirmInfo(
    const TradeConfirmInfoRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  TradeConfirmInfoReturn _return;
  FuturesProxy::getInstance()->TradeConfirmInfo(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

// 获取期货品种报价等详细信息  ---交易界面
QuerySymbolTradeInfoReturn RequestHandler::QuerySymbolTradeInfo(
    const QuerySymbolTradeInfoRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QuerySymbolTradeInfoReturn _return;
  FuturesProxy::getInstance()->QuerySymbolTradeInfo(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

// 买卖五档
QueryFifthOrderReturn RequestHandler::QueryFifthOrder(
    const QueryFifthOrderRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QueryFifthOrderReturn _return;
  FuturesProxy::getInstance()->QueryFifthOrder(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

// 期货报价
QuerySymbolQuotationsReturn RequestHandler::QuerySymbolQuotations(
    const QuerySymbolQuotationsRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QuerySymbolQuotationsReturn _return;
  FuturesProxy::getInstance()->QuerySymbolQuotations(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

// 分时
FuturesGraphReturn RequestHandler::FuturesGraph(
    const FuturesGraphRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  FuturesGraphReturn _return;
  FuturesProxy::getInstance()->FuturesGraph(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

// k线
FuturesKLineReturn RequestHandler::FuturesKLine(
    const FuturesKLineRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  FuturesKLineReturn _return;
  FuturesProxy::getInstance()->FuturesKLine(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}
// addapi step 3/3: 函数实现,

} // namespace server
} // namespace pb
} // namespace futures
