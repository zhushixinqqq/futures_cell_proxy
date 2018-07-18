// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin
#include "futures_cell_proxy/src/futures_proxy.h"

// #include <string>

#include "futures_cell_proxy/src/futures_convert.h"
#include "futures_cell_proxy/src/rpc/sp_trade_info_pool.h"
#include "futures_cell_proxy/src/rpc/sp_trade_info_wrapper.h"
#include "futures_cell_proxy/src/rpc/authen_client_pool.h"
#include "futures_cell_proxy/src/rpc/authen_user_server_wrapper.h"

#include "futures_cell_proxy/src/config.h"

#include "protocols/SPTradeInfoService.h"
#include "protocols/common_types.h"

namespace futures { namespace pb { namespace server {

FuturesProxy::FuturesProxy() {
};

FuturesProxy::~FuturesProxy() {
};

// 获取期货账户资产信息
void FuturesProxy::QueryMyFuturesAssets(const QueryMyFuturesAssetsRequest& req,
    QueryMyFuturesAssetsReturn *resp) {
  //TODO
  if (req.has_login_session()) {
    formax_social::protocols::Errno::type thrift_ret;
    ValidateSession(req.login_session(), &thrift_ret);
    if (formax_social::protocols::Errno::SUCCESS != thrift_ret) {
      common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_SESSION_ERR, "SESSION ERROR");
      LOG(ERROR) << __func__ << " REQ SESSION ERROR:";
      return;
    }

    futures_trade::protocols::GetAccInfoReq acc_info_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    acc_info_req.__set_access_info(access_info);
    acc_info_req.__set_user_id(req.login_session().uid());
    acc_info_req.__set_AccNo(req.futures_id());
    // 1 需要今日盈亏;0不需要
    acc_info_req.__set_NeedProfit(1);

    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
        = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::GetAccInfoResp acc_info_resp;
    bool rpc_ret=false;
    VLOG(90) << __func__ << " rpc GetAccInfo with req:" << acc_info_req;
    rpc_ret = sp_trade_info_client->GetAccInfo(acc_info_req, &acc_info_resp);
    VLOG(90) << __func__ << " rpc GetAccInfo with resp:" << acc_info_resp;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc GetAccInfo failed:";
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (acc_info_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
         FuturesConvert::ToProtoMyFuturesAsset(acc_info_resp, resp);
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_RETURN_ERR, "RETURN ERROR");
        LOG(ERROR) << __func__ << " rpc GetAccInfo return errno acc_info_resp:" << acc_info_resp;
      }
    }
  } else {
    LOG(ERROR) << __func__ << " QueryMyFuturesAssetsRequest: has login_session err with req:" << req.ShortDebugString();
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

// 获取账户开户状态
void FuturesProxy::QueryUserStatus(const QueryUserStatusRequest& req,
    QueryUserStatusReturn *resp) {
  // TODO(this)
  if (req.has_login_session()) {
    futures_trade::protocols::GetUserAccountStatusReq acc_status_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    acc_status_req.__set_access_info(access_info);
    acc_status_req.__set_user_id(req.login_session().uid());

    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
        = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::GetUserAccountStatusResp acc_status_resp;
    bool rpc_ret=false;
    VLOG(90) << __func__ << " rpc GetUserAccountStatus with req:" << acc_status_req;
    rpc_ret = sp_trade_info_client->GetUserAccountStatus(acc_status_req, &acc_status_resp);
    VLOG(90) << __func__ << " rpc GetUserAccountStatus with resp:" << acc_status_resp;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc GetUserAccountStatus failed:";
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (acc_status_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
         // FuturesConvert::ToProtoAccountStatus(acc_info_resp, resp);
        if (acc_status_resp.account_status == 5 || acc_status_resp.account_status == 4) {
        } else {
          resp->set_status_url(FLAGS_futures_account_open_url);
        }
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_RETURN_ERR, "RETURN ERROR");
        LOG(ERROR) << __func__ << " rpc GetUserAccountStatus return errno acc_info_resp:" << acc_status_resp;
      }
    }
  } else {
    resp->set_status_url(FLAGS_futures_account_open_url);
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

// 账户中心配置项
void FuturesProxy::QueryFuturesAccountConfig(const QueryFuturesAcconutConfigRequest& req,
    QueryFuturesAccountConfigReturn *resp) {
  // TODO(this)
  if (req.has_login_session()) {
    FuturesConvert::ToProtoFuturesAccountConfig(req,resp);
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
  } else {
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

// 获取委托订单
void FuturesProxy::QueryPendingOrders(const QueryPendingOrdersRequest& req,
    QueryPendingOrdersReturn *resp) {
  // TODO(this)
  if (req.has_login_session()) {
    formax_social::protocols::Errno::type thrift_ret;
    ValidateSession(req.login_session(), &thrift_ret);
    if (formax_social::protocols::Errno::SUCCESS != thrift_ret) {
      common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_SESSION_ERR, "SESSION ERROR");
      LOG(ERROR) << __func__ << " REQ SESSION ERROR:";
      return;
    }

    futures_trade::protocols::GetActiveOrderReq active_order_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    active_order_req.__set_access_info(access_info);
    active_order_req.__set_AccNo(req.futures_id());
    active_order_req.__set_user_id(req.login_session().uid());
    active_order_req.__set_start_index(req.start_index());
    active_order_req.__set_batch_num(req.batch_num());

    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
      = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::GetActiveOrderResp active_order_resp;
    bool rpc_ret=false;
    VLOG(90) << __func__ << " rpc GetActiveOrder with req:" << active_order_req;
    rpc_ret = sp_trade_info_client->GetActiveOrder(active_order_req, &active_order_resp);
    VLOG(90) << __func__ << " rpc GetActiveOrder with resp:" << active_order_resp;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc GetActiveOrder failed:";
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (active_order_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
        FuturesConvert::ToProtoActiveOrders(active_order_resp, resp);
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_RETURN_ERR, "RETURN ERROR");
        LOG(ERROR) << __func__ << " rpc GetActiveOrder return errno acc_info_resp:" << active_order_resp;
      }
    }
  } else {
    LOG(ERROR) << __func__ << " GetActiveOrder: has login_session err with req:" << req.ShortDebugString();
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

// 获取持仓中订单
void FuturesProxy::QueryHoldingOrders(const QueryHoldingOrdersRequest& req,
    QueryHoldingOrdersReturn *resp) {
  // TODO(this)
  if (req.has_login_session()) {
    formax_social::protocols::Errno::type thrift_ret;
    ValidateSession(req.login_session(), &thrift_ret);
    if (formax_social::protocols::Errno::SUCCESS != thrift_ret) {
      common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_SESSION_ERR, "SESSION ERROR");
      LOG(ERROR) << __func__ << " REQ SESSION ERROR:";
      return;
    }
    futures_trade::protocols::GetPositionReq position_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    position_req.__set_access_info(access_info);
    position_req.__set_user_id(req.login_session().uid());
    position_req.__set_AccNo(req.futures_id());
    // int8_t byte = 0;
    position_req.__set_Type(0);//TODO
    position_req.__set_start_index(req.start_index());
    position_req.__set_batch_num(req.batch_num());

    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
      = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::GetPositionResp position_resp;
    bool rpc_ret=false;
    VLOG(90) << __func__ << " rpc GetPosition with req:" << position_req;
    rpc_ret = sp_trade_info_client->GetPosition(position_req, &position_resp);
    VLOG(90) << __func__ << " rpc GetPosition with resp:" << position_resp;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc GetPosition failed rpc_ret: " << rpc_ret;
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (position_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
         FuturesConvert::ToProtoPosition(position_resp, resp);
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_RETURN_ERR, "RETURN ERROR");
        LOG(ERROR) << __func__ << " rpc GetPosition return errno position_resp:" << position_resp;
      }
    }
  } else {
    LOG(ERROR) << __func__ << " QueryHoldingOrdersRequest: has login_session err with req:" << req.ShortDebugString();
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

// 获取订单详情
void FuturesProxy::QueryOrderDetail(const QueryOrderDetailRequest& req,
    QueryOrderDetailReturn *resp) {
  // TODO(this)
  if (req.has_login_session()) {
    formax_social::protocols::Errno::type thrift_ret;
    ValidateSession(req.login_session(), &thrift_ret);
    if (formax_social::protocols::Errno::SUCCESS != thrift_ret) {
      common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_SESSION_ERR, "SESSION ERROR");
      LOG(ERROR) << __func__ << " REQ SESSION ERROR:";
      return;
    }
    futures_trade::protocols::GetOrderByNoReq order_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    order_req.__set_access_info(access_info);
    order_req.__set_user_id(req.login_session().uid());
    order_req.__set_AccNo(req.futures_id());
    order_req.__set_IntOrderNo(req.order_id());

    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
      = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::GetOrderByNoResp order_detail_resp;
    bool rpc_ret=false;
    VLOG(90) << __func__ << " rpc GetOrderByNo with req:" << order_req;
    rpc_ret = sp_trade_info_client->GetOrderByNo(order_req, &order_detail_resp);
    VLOG(90) << __func__ << " rpc GetOrderByNo with resp:" << order_detail_resp;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc GetOrderByNo failed rpc_ret: " << rpc_ret;
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (order_detail_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
         FuturesConvert::ToProtoOrderDetail(order_detail_resp, resp);
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_RETURN_ERR, "RETURN ERROR");
        LOG(ERROR) << __func__ << " rpc GetOrderByNo return errno add_order_resp:" << order_detail_resp;
      }
    }
  } else {
    LOG(ERROR) << __func__ << " QueryOrderDetailRequest: has login_session err with req:" << req.ShortDebugString();
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

// 获取资金流水
void FuturesProxy::QueryTransactionRecord(const QueryTransactionRecordRequest& req,
    QueryTransactionRecordReturn *resp) {
  // TODO(this)
  if (req.has_login_session()) {
    formax_social::protocols::Errno::type thrift_ret;
    ValidateSession(req.login_session(), &thrift_ret);
    if (formax_social::protocols::Errno::SUCCESS != thrift_ret) {
      common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_SESSION_ERR, "SESSION ERROR");
      LOG(ERROR) << __func__ << " REQ SESSION ERROR:";
      return;
    }

    futures_trade::protocols::GetFundInOutRecordReq fund_record_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    fund_record_req.__set_access_info(access_info);
    fund_record_req.__set_user_id(req.login_session().uid());
    fund_record_req.__set_AccNo(req.futures_id());
    fund_record_req.__set_index(req.index());
    fund_record_req.__set_batch_num(req.batch_num());

    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
      = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::GetFundInOutRecordResp fund_record_resp;
    bool rpc_ret=false;
    VLOG(90) << __func__ << " rpc GetFundInOutRecord with req:" << fund_record_req;
    rpc_ret = sp_trade_info_client->GetFundInOutRecord(fund_record_req, &fund_record_resp);
    VLOG(90) << __func__ << " rpc GetFundInOutRecord with resp:" << fund_record_resp;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc GetFundInOutRecord failed rpc_ret: " << rpc_ret;
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (fund_record_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
         FuturesConvert::ToProtoFundRecord(fund_record_resp, resp);
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_RETURN_ERR, "RETURN ERROR");
        LOG(ERROR) << __func__ << " rpc GetFundInOutRecord return errno add_order_resp:" << fund_record_resp;
      }
    }
  } else {
    LOG(ERROR) << __func__ << " QueryTransactionRecordRequest: has login_session err with req:" << req.ShortDebugString();
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

// 下单交易
void FuturesProxy::FuturesTrade(const FuturesTradeRequest& req,
    FuturesTradeReturn *resp) {
  // TODO(this)
  if (req.has_session()) {
    std::ostringstream oss;
    oss << req.futures_id() << req.symbol() << req.volume() << req.type() << req.price();
    MD5 md5(oss.str());
    std::string aes_string = AESEncrypt(md5.toString());
    VLOG(90) <<"oss:" << oss.str() << "MD5:"<< md5.toString() << "AES:" << aes_string;
    if (aes_string!=req.signature()) {
      common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_SESSION_ERR, "VALIDATE ERROR");
      LOG(ERROR) << __func__ << " the req.signature validate was not correct(AES(MD5)):";
      return;
    }
    double price=strtod(req.price().c_str(),NULL);

    formax_social::protocols::Errno::type thrift_ret;
    ValidateSession(req.session(), &thrift_ret);
    if (formax_social::protocols::Errno::SUCCESS != thrift_ret) {
      common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_SESSION_ERR, "SESSION ERROR");
      LOG(ERROR) << __func__ << " REQ SESSION ERROR:";
      return;
    }
    futures_trade::protocols::AddOrderReq add_order_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    add_order_req.__set_access_info(access_info);
    add_order_req.__set_AccNo(req.futures_id());
    add_order_req.__set_user_id(req.session().uid());
    add_order_req.__set_ProdCode(req.symbol());
    add_order_req.__set_Qty(atof(req.volume().c_str()));

    nanoClock_type tp = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
    oss.str("");
    oss << tp.time_since_epoch().count();
    add_order_req.__set_ClOrderId(oss.str());
    add_order_req.__set_OrderOptions(0);
    add_order_req.__set_CondType(futures_trade::protocols::OrderCondType::COND_NONE);
    add_order_req.__set_OrderType(futures_trade::protocols::OrderPriceType::ORD_LIMIT);

    add_order_req.__set_ValidType(futures_trade::protocols::VLDType::VLD_REST_OF_DAY);
    add_order_req.__set_DecInPrice(4);
    add_order_req.__set_OrderAction(1);

    if (req.type() == 1) {
      add_order_req.__set_BuySell('B');
    } else {
      add_order_req.__set_BuySell('S');
    }
    add_order_req.__set_Price(price);

    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
      = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::AddOrderResp add_order_resp;
    bool rpc_ret=false;
    VLOG(90) << __func__ << " rpc AddOrder with req:" <<add_order_req ;
    rpc_ret = sp_trade_info_client->AddOrder(add_order_req, &add_order_resp);
    VLOG(90) << __func__ << " rpc AddOrder with resp:" << add_order_resp;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc AddOrder failed rpc_ret: " << rpc_ret;
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (add_order_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_TRADE_ERR, "TRADE ERROR");
        LOG(ERROR) << __func__ << " rpc AddOrder return errno add_order_resp:" << add_order_resp;
      }
    }
  } else {
    LOG(ERROR) << __func__ << " FuturesTrade: has login_session err with req:" << req.ShortDebugString();
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

// 撤单
void FuturesProxy::CancelPendingOrder(const CancelPendingOrderRequest& req,
    FututesCancelPendingOrderReturn *resp) {
  // TODO(this)
  if (req.has_session()) {
    formax_social::protocols::Errno::type thrift_ret;
    ValidateSession(req.session(), &thrift_ret);
    if (formax_social::protocols::Errno::SUCCESS != thrift_ret) {
      common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_SESSION_ERR, "SESSION ERROR");
      LOG(ERROR) << __func__ << " REQ SESSION ERROR:";
      return;
    }
    futures_trade::protocols::DeleteOrderReq delete_order_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    delete_order_req.__set_access_info(access_info);
    delete_order_req.__set_user_id(req.session().uid());
    delete_order_req.__set_AccNo(req.futures_id());
    delete_order_req.__set_IntOrderNo(atoi(req.order_id().c_str()));
    delete_order_req.__set_DeleteMode(1);//TODO
    delete_order_req.__set_ProdCode(req.symbol());

    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
      = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::DeleteOrderResp delete_order_resp;
    bool rpc_ret=false;
    VLOG(90) << __func__ << " rpc DeleteOrder with req:" << delete_order_req ;
    rpc_ret = sp_trade_info_client->DeleteOrder(delete_order_req, &delete_order_resp);
    VLOG(90) << __func__ << " rpc DeleteOrder with resp:" << delete_order_resp ;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc DeleteOrder failed rpc_ret: " << rpc_ret;
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (delete_order_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
        //TODO
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_RETURN_ERR, "DELETE ERROR");
        LOG(ERROR) << __func__ << " rpc DeleteOrder eturn errno delete_order_resp:" << delete_order_resp;
      }
    }
  } else {
    LOG(ERROR) << __func__ << " FuturesTrade: has login_session err with req:" << req.ShortDebugString();
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

// 历史订单
void FuturesProxy::QueryHistoryOrders(const QueryHistoryOrderRequest& req,
    QueryHistoryOrderReturn *resp) {
  // TODO(this)
  if (req.has_login_session()) {
    formax_social::protocols::Errno::type thrift_ret;
    ValidateSession(req.login_session(), &thrift_ret);
    if (formax_social::protocols::Errno::SUCCESS != thrift_ret) {
      common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_SESSION_ERR, "SESSION ERROR");
      LOG(ERROR) << __func__ << " REQ SESSION ERROR:";
      return;
    }

    futures_trade::protocols::GetHisOrderReq his_order_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    his_order_req.__set_access_info(access_info);
    his_order_req.__set_user_id(req.login_session().uid());
    his_order_req.__set_AccNo(req.futures_id());
    his_order_req.__set_start_index(req.start_index());
    his_order_req.__set_batch_num(req.batch_num());

    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
      = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::GetHisOrderResp his_order_resp;
    bool rpc_ret = false;
    VLOG(90) << __func__ << " rpc GetHisOrder with req:" << his_order_req;
    rpc_ret = sp_trade_info_client->GetHisOrder(his_order_req, &his_order_resp);
    VLOG(90) << __func__ << " rpc GetHisOrder with resp:" <<his_order_resp ;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc GetHisOrder failed rpc_ret: " << rpc_ret;
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (his_order_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
        //TODO
        FuturesConvert::ToProtoHistoryOrders(his_order_resp, resp);
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_RETURN_ERR, "DELETE ERROR");
        LOG(ERROR) << __func__ << " rpc GetHisOrde return errno his_order_resp:" << his_order_resp;
      }
    }
  } else {
    LOG(ERROR) << __func__ << " FuturesTrade: has login_session err with req:" << req.ShortDebugString();
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

// 期货搜索推荐页
void FuturesProxy::SymbolTypeRecommend(const SymbolTypeRecommendRequest& req,
    SymbolTypeRecommendReturn *resp) {
  //TODO
  common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
}

// 期货分类搜索
void FuturesProxy::SymbolListByType(const SymbolListByTypeRequest& req,
    SymbolListByTypeReturn *resp) {
  //TODO
  common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
}

// 期货名称搜索
void FuturesProxy::QuerySymbolByName(const QuerySymbolByNameRequest& req,
    QuerySymbolByNameReturn *resp) {
  //TODO
  std::string request_url = FLAGS_futures_search_url + "futures/" + req.key();
  LOG(INFO) << "fndocurl request_url:" << request_url;
  std::string ret_out;
  int ret = fnDoCurl(request_url, ret_out);
  if (ret != 0) {
    LOG(ERROR) << "fnDoCurl fail,ret code:" << ret;
    common::FillStatusInfo(resp->mutable_status_info(),
        FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
  } else {
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(ret_out, root);
    if (!parsingSuccessful) {
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_UNKNOWN, "UNKNOWN ERROR");
      LOG(ERROR) << "parse json fail, json content::[" << ret_out << "]";
    } else {
      FuturesConvert::ToProtoSymbolByName(root, resp);
      common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
    }
  }
}

void FuturesProxy::ValidateSession(const LoginSession& login_session,
     formax_social::protocols::Errno::type* thrift_ret) {
  formax_social::protocols::SessionInfo thrift_req;
  thrift_req.m_session_id = login_session.session_str();
  thrift_req.m_uid = login_session.uid();
  AuthenClientPtr authen_client = AuthenClientPool::Instance()->GetClientPtr();
  bool rpc_valided = false;
  // formax_social::protocols::Errno::type thrift_ret;
  rpc_valided = authen_client->ValidateSession(thrift_req, thrift_ret);
  VLOG(90) << __func__ << " ValidateSession==>resp(success:0):" << *thrift_ret;
  if (rpc_valided) {
    AuthenClientPool::Instance()->FreeClientPtr(authen_client, true);
  }
}

void FuturesProxy::TradeConfirmInfo(const TradeConfirmInfoRequest& req,
    TradeConfirmInfoReturn *resp) {
  //TODO 单笔保证金，与行情有关;也可能是数据库表
  if (req.has_session()) {
    futures_trade::protocols::GetFuturesQuoteFeeInfoReq fee_info_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    fee_info_req.__set_quote_id("HI"); //TODO 品种
    fee_info_req.__set_access_info(access_info);
    fee_info_req.__set_user_id(req.session().uid());
    fee_info_req.__set_AccNo(req.futures_id());

    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
      = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::GetFuturesQuoteFeeInfoResp fee_info_resp;
    bool rpc_ret = false;
    VLOG(90) << __func__ << " rpc GetFuturesQuoteFeeInfo with req:" << fee_info_req;
    rpc_ret = sp_trade_info_client->GetFuturesQuoteFeeInfo(fee_info_req, &fee_info_resp);
    VLOG(90) << __func__ << " rpc GetFuturesQuoteFeeInfo with resp:" << fee_info_resp;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc GetFuturesQuoteFeeInfo failed rpc_ret: " << rpc_ret;
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (fee_info_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
        //TODO
        FuturesConvert::ToProtoQuoteFeeInfo(fee_info_resp, resp);
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_RETURN_ERR, "RETURN ERROR");
        LOG(ERROR) << __func__ << " rpc GetFuturesQuoteFeeInfo return ERROR";
      }
    }
  } else {
    LOG(ERROR) << __func__ << " TradeConfirmInfoRequest: has login_session err with req:" << req.ShortDebugString();
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

void FuturesProxy::QuerySymbolTradeInfo(const QuerySymbolTradeInfoRequest& req,
    QuerySymbolTradeInfoReturn *resp) {
  if (req.has_session()) {
    futures_trade::protocols::GetFuturesQuoteFeeInfoReq fee_info_req;
    futures_trade::protocols::AccessInfo access_info;
    common::GenerateUniqueID("futures_cell_proxy",
        futures_trade::protocols::AccessType::CELL_PROXY, &access_info);
    fee_info_req.__set_quote_id("HI"); //TODO 品种
    fee_info_req.__set_access_info(access_info);
    fee_info_req.__set_user_id(req.session().uid());
    fee_info_req.__set_AccNo(req.futures_id());
    // TODO
    std::shared_ptr<SPTradeInfoWrapper> sp_trade_info_client
      = SPTradeInfoClientPool::Instance()->GetClientPtr();
    futures_trade::protocols::GetFuturesQuoteFeeInfoResp fee_info_resp;
    bool rpc_ret = false;
    VLOG(90) << __func__ << " rpc GetFuturesQuoteFeeInfo with req:" << fee_info_req;
    rpc_ret = sp_trade_info_client->GetFuturesQuoteFeeInfo(fee_info_req, &fee_info_resp);
    VLOG(90) << __func__ << " rpc GetFuturesQuoteFeeInfo with resp:" << fee_info_resp;
    if (!rpc_ret) {
      LOG(ERROR) << __func__ << " rpc GetFuturesQuoteFeeInfo failed rpc_ret: " << rpc_ret;
      common::FillStatusInfo(resp->mutable_status_info(),
          FuturesError::ErrCode_FUTURES_INFO_BUSY, "FUTURES INFO BUSY");
    } else {
      SPTradeInfoClientPool::Instance()->FreeClientPtr(sp_trade_info_client, true);
      if (fee_info_resp.err.err_no == futures_trade::protocols::Errno::SUCCESS) {
        //TODO
        FuturesConvert::ToProtoQuerySymbolTradeInfo(fee_info_resp, resp);
        FuturesSymbolItem* symbol_item=resp->mutable_symbol_info()->mutable_base_info();
        symbol_item->set_symbol(req.symbol());
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
      } else {
        common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_RETURN_ERR, "RETURN ERROR");
        LOG(ERROR) << __func__ << " rpc GetFuturesQuoteFeeInfo return ERROR";
      }
    }
  } else {
    LOG(ERROR) << __func__ << " QuerySymbolTradeInfoRequest: has login_session err with req:" << req.ShortDebugString();
    common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_FUTURES_REQ_ERR, "REQ ERROR");
  }
}

void FuturesProxy::QueryFifthOrder(const QueryFifthOrderRequest& req,
    QueryFifthOrderReturn *resp) {
  //TODO
  common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
}

// 报价
void FuturesProxy::QuerySymbolQuotations(const QuerySymbolQuotationsRequest& req,
    QuerySymbolQuotationsReturn *resp) {
  //TODO
  common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
}

// 分时
void FuturesProxy::FuturesGraph(const FuturesGraphRequest& req,
    FuturesGraphReturn *resp) {
  //TODO
  common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
}

// k线
void FuturesProxy::FuturesKLine(const FuturesKLineRequest& req,
    FuturesKLineReturn *resp) {
  //TODO
  common::FillStatusInfo(resp->mutable_status_info(), FuturesError::ErrCode_SUCCESS, "SUCCESS");
}
} // namespace server
} // namespace pb
} // namespace futures
