// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin


#include "futures_cell_proxy/src/rpc/authen_user_server_wrapper.h"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "futures_cell_proxy/src/config.h"

namespace futures { namespace pb { namespace server {

using apache::thrift::transport::TSocket;
using apache::thrift::transport::TTransport;
using apache::thrift::transport::TFramedTransport;
using apache::thrift::protocol::TBinaryProtocol;
using apache::thrift::protocol::TProtocol;
using formax_social::protocols::AuthenticationBaseServiceClient;

AuthenServerWrapper::AuthenServerWrapper(const std::string& server_ip,
    const int32_t server_port) : m_server_ip(server_ip), m_server_port(server_port),
  m_retry_time(FLAGS_m_retry_times) {
  m_is_inited = false;
  InitClient();
}
bool AuthenServerWrapper::InitClient() {
  if (m_is_inited) {
    return true;
  }
  try {
    boost::shared_ptr<TSocket> socket(new TSocket(m_server_ip, m_server_port));
    // 设置超时
    socket->setConnTimeout(FLAGS_conn_timeout_ms);
    socket->setRecvTimeout(FLAGS_recv_timeout_ms);
    socket->setSendTimeout(FLAGS_send_timeout_ms);

    boost::shared_ptr<TTransport> transport(new TFramedTransport(socket));
    boost::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
    boost::shared_ptr<AuthenticationBaseServiceClient> client(
        new AuthenticationBaseServiceClient(protocol));

    m_transport  = transport;
    m_transport->open();
    m_client = client;
    m_is_inited = true;
  } catch (const std::exception& e) {
    LOG(INFO) << "AuthenticationBaseServiceClient Inited, ip: " << m_server_ip
      << " port: " << m_server_port << " not ok"
      << " reason: " << e.what();
    return false;
  }
  LOG(INFO) << "AuthenticationBaseServiceClient Inited, ip: " << m_server_ip
    << " port: " << m_server_port;
  return true;
}

bool AuthenServerWrapper::ValidateSession(
    const formax_social::protocols::SessionInfo& session,
    formax_social::protocols::Errno::type *ret) {
  *ret = formax_social::protocols::Errno::UNKNOWN;
  int32_t  retry_time = m_retry_time;
  while (retry_time--) {
    if (!InitClient()) {
      continue;
    }
    try {
      *ret = m_client->ValidateSession(session);
      return true;
    } catch(const std::exception& e) {
      // 超时等网络异常
      LOG(ERROR) << "exception in AuthenServerWrapper::ValidateSession : ["
        << e.what() << "] retry for the: " << retry_time << " times";
      m_is_inited = false;
      continue;
    }
  }
  return false;
}
//
} // namespace server
} // namespace pb
} // namespace futures


