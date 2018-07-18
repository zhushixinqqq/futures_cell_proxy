// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin
#ifndef FUTURES_CELL_PROXY_SRC_RPC_SP_TRADE_INFO_WRAPPER_H
#define FUTURES_CELL_PROXY_SRC_RPC_SP_TRADE_INFO_WRAPPER_H

#include <string>

#include <boost/noncopyable.hpp>
#include "thrift/transport/TTransport.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "protocols/SPTradeInfoService.h"
#include "protocols/common_types.h"


namespace futures { namespace pb { namespace server {

class SPTradeInfoWrapper : private boost::noncopyable {
  public:
    SPTradeInfoWrapper(const std::string& server_ip, const int32_t server_port);
  public:
    bool GetAccInfo(const futures_trade::protocols::GetAccInfoReq& req,
        futures_trade::protocols::GetAccInfoResp* resp);

    bool AddOrder(const futures_trade::protocols::AddOrderReq& req,
        futures_trade::protocols::AddOrderResp* resp);

    bool GetOrderByNo(const futures_trade::protocols::GetOrderByNoReq& req,
        futures_trade::protocols::GetOrderByNoResp* resp);

    bool GetActiveOrder(const futures_trade::protocols::GetActiveOrderReq& req,
        futures_trade::protocols::GetActiveOrderResp* resp);

    bool GetPosition(const futures_trade::protocols::GetPositionReq& req,
        futures_trade::protocols::GetPositionResp* resp);

    bool DeleteOrder(const futures_trade::protocols::DeleteOrderReq& req,
        futures_trade::protocols::DeleteOrderResp* resp);

    bool GetUserAccountStatus(const futures_trade::protocols::GetUserAccountStatusReq& req,
        futures_trade::protocols::GetUserAccountStatusResp* resp);

    bool GetHisOrder(const futures_trade::protocols::GetHisOrderReq& req,
        futures_trade::protocols::GetHisOrderResp* resp);

    bool GetFundInOutRecord(const futures_trade::protocols::GetFundInOutRecordReq& req,
        futures_trade::protocols::GetFundInOutRecordResp* resp);

    bool GetFuturesQuoteFeeInfo(const futures_trade::protocols::GetFuturesQuoteFeeInfoReq& req,
        futures_trade::protocols::GetFuturesQuoteFeeInfoResp* resp);
  private:
    bool InitClient();
  private:
    const std::string   m_server_ip;
    const int32_t       m_server_port;
    int32_t m_retry_time;
    bool m_is_inited;
    boost::shared_ptr<apache::thrift::transport::TTransport> m_transport;

    boost::shared_ptr<futures_trade::protocols::SPTradeInfoServiceClient> m_client;
};

} // namespace server
} // namespace pb
} // namespace futures
#endif // FUTURES_CELL_PROXY_SRC_RPC_SP_TRADE_INFO_WRAPPER_H



