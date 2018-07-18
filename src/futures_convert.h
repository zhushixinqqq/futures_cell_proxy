// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin
#ifndef FUTURES_CELL_PROXY_SRC_FUTURES_CONVERT_H
#define FUTURES_CELL_PROXY_SRC_FUTURES_CONVERT_H

#include <string>

#include "glog/logging.h"
#include "protocols/common_types.h"
#include "protocols/SPTradeInfoService.h"
#include "futures_cell_proxy/common/common.h"
#include "futures_cell_proxy/src/config.h"
#include "thirdparty/jsoncpp/json.h"

using namespace futures::pb::server::common;
namespace futures { namespace pb { namespace server {
class FuturesConvert {
  public:
    static void ToProtoMyFuturesAsset(const futures_trade::protocols::GetAccInfoResp& acc_info_resp,
        QueryMyFuturesAssetsReturn* resp);
    static void ToProtoOrderDetail(const futures_trade::protocols::GetOrderByNoResp& order_detail_resp,
       QueryOrderDetailReturn* resp);
    static void ToProtoActiveOrders(const futures_trade::protocols::GetActiveOrderResp& order_detail_resp,
       QueryPendingOrdersReturn* resp);
    static void ToProtoPosition(const futures_trade::protocols::GetPositionResp& position_resp,
       QueryHoldingOrdersReturn* resp);
    static void ToProtoHistoryOrders(const futures_trade::protocols::GetHisOrderResp& his_order_resp,
       QueryHistoryOrderReturn* resp);
    static void ToProtoFuturesAccountConfig(const QueryFuturesAcconutConfigRequest& req,
        QueryFuturesAccountConfigReturn* resp);
    static void ToProtoFundRecord(const futures_trade::protocols::GetFundInOutRecordResp& fund_record_resp,
        QueryTransactionRecordReturn* resp);
    static void ToProtoSymbolByName(const Json::Value& root, QuerySymbolByNameReturn* resp);
    static void ToProtoQuoteFeeInfo(const futures_trade::protocols::GetFuturesQuoteFeeInfoResp& fee_info_resp,
        TradeConfirmInfoReturn* resp);
    static void ToProtoQuerySymbolTradeInfo(const futures_trade::protocols::GetFuturesQuoteFeeInfoResp& fee_info_resp,
       QuerySymbolTradeInfoReturn* resp);

};
} // namespace server
} // namespace pb
} // namespace futures

#endif // FUTURES_CELL_PROXY_SRC_FUTURES_CONVERT_H
