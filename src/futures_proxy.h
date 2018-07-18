// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#ifndef FUTURES_CELL_PROXY_SRC_FUTURES_PROXY_H
#define FUTURES_CELL_PROXY_SRC_FUTURES_PROXY_H

#include <string>
#include "futures_cell_proxy/common/common.h"
#include "futures_cell_proxy/common/md5.h"
#include "protocols/authentication_def_types.h"

using namespace futures::pb::server::common;

namespace futures { namespace pb { namespace server {

class FuturesProxy : public Singleton<FuturesProxy> {
  friend class Singleton<FuturesProxy>; //基类在构造函数中需要调用FuturesProxy构造函数
  private:
    FuturesProxy();
    ~FuturesProxy();
  public:
    // 获取期货账户资产信息
    void QueryMyFuturesAssets(const QueryMyFuturesAssetsRequest& req,
        QueryMyFuturesAssetsReturn *resp);
    // 获取账户开户状态
    void QueryUserStatus(const QueryUserStatusRequest& req,
        QueryUserStatusReturn *resp);
    // 账户中心配置项
    void QueryFuturesAccountConfig(const QueryFuturesAcconutConfigRequest& req,
        QueryFuturesAccountConfigReturn *resp);
    // 获取委托订单
    void QueryPendingOrders(const QueryPendingOrdersRequest& req,
        QueryPendingOrdersReturn *resp);
    // 获取持仓中订单
    void QueryHoldingOrders(const QueryHoldingOrdersRequest& req,
        QueryHoldingOrdersReturn *resp);
    // 获取订单详情
    void QueryOrderDetail(const QueryOrderDetailRequest& req,
        QueryOrderDetailReturn *resp);
    // 获取资金流水
    void QueryTransactionRecord(const QueryTransactionRecordRequest& req,
        QueryTransactionRecordReturn *resp);
    // 下单交易
    void FuturesTrade(const FuturesTradeRequest& req,
        FuturesTradeReturn *resp);
    // 撤单
    void CancelPendingOrder(const CancelPendingOrderRequest& req,
        FututesCancelPendingOrderReturn *resp);
    // 历史订单
    void QueryHistoryOrders(const QueryHistoryOrderRequest& req,
        QueryHistoryOrderReturn *resp);
    // 期货搜索界面，按类选择推荐
    void SymbolTypeRecommend(const SymbolTypeRecommendRequest& req,
        SymbolTypeRecommendReturn *resp);
    // ---搜索界面更多
    void SymbolListByType(const SymbolListByTypeRequest& req,
        SymbolListByTypeReturn *resp);
    // ---搜索期货
    void QuerySymbolByName(const QuerySymbolByNameRequest& req,
        QuerySymbolByNameReturn *resp);
    // SESSION 验证接口
    void ValidateSession(const LoginSession& login_session,
        formax_social::protocols::Errno::type* thrift_ret);
    // 交易确认接口
    void TradeConfirmInfo(const TradeConfirmInfoRequest& req,
        TradeConfirmInfoReturn* resp);
    //--交易界面
    void QuerySymbolTradeInfo(const QuerySymbolTradeInfoRequest& req,
        QuerySymbolTradeInfoReturn* resp);
    //--买卖五档
    void QueryFifthOrder(const QueryFifthOrderRequest& req,
        QueryFifthOrderReturn* resp);
    //--期货报价
    void QuerySymbolQuotations(const QuerySymbolQuotationsRequest& req,
        QuerySymbolQuotationsReturn* resp);
    //--分时
    void FuturesGraph(const FuturesGraphRequest& req,
        FuturesGraphReturn* resp);
    //--K线
    void FuturesKLine(const FuturesKLineRequest& req,
        FuturesKLineReturn* resp);
};
} // namespace server
} // namespace pb
} // namespace futures

#endif // FUTURES_CELL_PROXY_SRC_FUTURES_PROXY_H

