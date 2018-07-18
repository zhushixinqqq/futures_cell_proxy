// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin
#include "futures_cell_proxy/src/rpc/sp_trade_info_wrapper.h"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "futures_cell_proxy/src/config.h"

namespace futures { namespace pb { namespace server {

using apache::thrift::transport::TSocket;
using apache::thrift::transport::TTransport;
using apache::thrift::transport::TFramedTransport;
using apache::thrift::protocol::TBinaryProtocol;
using apache::thrift::protocol::TProtocol;
using futures_trade::protocols::SPTradeInfoServiceClient;

SPTradeInfoWrapper::SPTradeInfoWrapper(const std::string& server_ip,
    const int32_t server_port):m_server_ip(server_ip), m_server_port(server_port),
m_retry_time(FLAGS_m_retry_times) {
  m_is_inited = false;
  InitClient();
}

bool SPTradeInfoWrapper::InitClient() {
  if(m_is_inited) {
    return true;
  }
  m_client.reset();
  try {
    boost::shared_ptr<TSocket> socket(new TSocket(m_server_ip, m_server_port));
    socket->setConnTimeout(FLAGS_conn_timeout_ms);
    socket->setRecvTimeout(FLAGS_recv_timeout_ms);
    socket->setSendTimeout(FLAGS_send_timeout_ms);

    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    boost::shared_ptr<SPTradeInfoServiceClient> client(new SPTradeInfoServiceClient(protocol));

    m_transport  = transport;
    m_transport->open();
    m_client = client;
    m_is_inited = true;
  } catch (const std::exception& e) {
    LOG(ERROR) << "SPTradeInfoServiceClient inited ip:" << m_server_ip
      << " port:" << m_server_port << "failed reason is:" << e.what();
    m_is_inited = false;
    return false;
  }
  LOG(INFO) << "SPTradeInfoServiceClient inited ip:" << m_server_ip
    << " port:" << m_server_port;
  return true;
}

bool SPTradeInfoWrapper::GetAccInfo(const futures_trade::protocols::GetAccInfoReq& req,
    futures_trade::protocols::GetAccInfoResp* resp) {
  int32_t  retry_time = m_retry_time;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      m_client->GetAccInfo(*resp, req);
      return true;
    } catch (const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in SPTradeInfoWrapper::GetAccInfo "
        << e.what() << " retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}

bool SPTradeInfoWrapper::AddOrder(const futures_trade::protocols::AddOrderReq& req,
    futures_trade::protocols::AddOrderResp* resp) {
  // 防止重复下单调用一次
  int32_t  retry_time = FLAGS_rpc_call_times;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      m_client->AddOrder(*resp, req);
      return true;
    } catch (const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in SPTradeInfoWrapper::AddOrder "
        << e.what() << " retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}

bool SPTradeInfoWrapper::GetOrderByNo(const futures_trade::protocols::GetOrderByNoReq& req,
    futures_trade::protocols::GetOrderByNoResp* resp) {
  int32_t  retry_time = m_retry_time;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      m_client->GetOrderByNo(*resp, req);
      return true;
    } catch (const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in SPTradeInfoWrapper::GetOrderByNo "
        << e.what() << " retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}

bool SPTradeInfoWrapper::GetActiveOrder(const futures_trade::protocols::GetActiveOrderReq& req,
    futures_trade::protocols::GetActiveOrderResp* resp) {
  int32_t  retry_time = m_retry_time;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      m_client->GetActiveOrder(*resp, req);
      return true;
    } catch (const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in SPTradeInfoWrapper::GetActiveOrder "
        << e.what() << " retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}

bool SPTradeInfoWrapper::GetPosition(const futures_trade::protocols::GetPositionReq& req,
    futures_trade::protocols::GetPositionResp* resp) {
  int32_t  retry_time = m_retry_time;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      m_client->GetPosition(*resp, req);
      return true;
    } catch (const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in SPTradeInfoWrapper::GetPositionResp "
        << e.what() << " retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}

// 删除订单
bool SPTradeInfoWrapper::DeleteOrder(const futures_trade::protocols::DeleteOrderReq& req,
    futures_trade::protocols::DeleteOrderResp* resp) {
  int32_t  retry_time = FLAGS_rpc_call_times;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      m_client->DeleteOrder(*resp, req);
      return true;
    } catch (const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in SPTradeInfoWrapper::GetPositionResp "
        << e.what() << " retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}

// 获取开户状态
bool SPTradeInfoWrapper::GetUserAccountStatus(const futures_trade::protocols::GetUserAccountStatusReq& req,
    futures_trade::protocols::GetUserAccountStatusResp* resp) {
  int32_t  retry_time = m_retry_time;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      m_client->GetUserAccountStatus(*resp, req);
      return true;
    } catch (const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in SPTradeInfoWrapper::GetUserAccountStatus "
        << e.what() << " retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}

// 获取历史订单
bool SPTradeInfoWrapper::GetHisOrder(const futures_trade::protocols::GetHisOrderReq& req,
    futures_trade::protocols::GetHisOrderResp* resp) {
  int32_t  retry_time = m_retry_time;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      m_client->GetHisOrder(*resp, req);
      return true;
    } catch (const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in SPTradeInfoWrapper::GetHisOrder "
        << e.what() << " retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}

// 出入金记录
bool SPTradeInfoWrapper::GetFundInOutRecord(const futures_trade::protocols::GetFundInOutRecordReq& req,
    futures_trade::protocols::GetFundInOutRecordResp* resp) {
  int32_t  retry_time = m_retry_time;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      m_client->GetFundInOutRecord(*resp, req);
      return true;
    } catch (const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in SPTradeInfoWrapper::GetFundInOutRecord "
        << e.what() << " retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}

// 佣金费用
bool SPTradeInfoWrapper::GetFuturesQuoteFeeInfo(const futures_trade::protocols::GetFuturesQuoteFeeInfoReq& req,
    futures_trade::protocols::GetFuturesQuoteFeeInfoResp* resp) {
  int32_t  retry_time = m_retry_time;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      m_client->GetFuturesQuoteFeeInfo(*resp, req);
      return true;
    } catch (const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in SPTradeInfoWrapper::GetFuturesQuoteFeeInfo "
        << e.what() << " retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}
} //nampespace server
} //namespace pb
} // namespace futures



