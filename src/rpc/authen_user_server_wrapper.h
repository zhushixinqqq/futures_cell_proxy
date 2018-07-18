// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#ifndef FUTURES_CELL_PROXY_SRC_RPC_AUTHEN_USER_SERVER_WRAPPER_H
#define FUTURES_CELL_PROXY_SRC_RPC_AUTHEN_USER_SERVER_WRAPPER_H

#include <string>
#include "boost/noncopyable.hpp"
#include "boost/thread/locks.hpp"
#include "boost/thread/mutex.hpp"
#include "thrift/protocol/TBinaryProtocol.h"
#include "thrift/transport/TSocket.h"
#include "thrift/transport/TTransportUtils.h"

#include "protocols/authentication_def_types.h"
#include "protocols/AuthenticationBaseService.h"

namespace futures { namespace pb { namespace server {
class AuthenServerWrapper : private boost::noncopyable {
  public:
    AuthenServerWrapper(const std::string& server_ip, const int32_t server_port);
    bool ValidateSession(const formax_social::protocols::SessionInfo& session,
        formax_social::protocols::Errno::type *ret);
  private:
    bool InitClient();
  private:
    const std::string   m_server_ip;
    const int32_t       m_server_port;
    int32_t m_retry_time;
    bool m_is_inited;
    boost::shared_ptr<apache::thrift::transport::TTransport> m_transport;

    boost::shared_ptr<formax_social::protocols::AuthenticationBaseServiceClient> m_client;
};

} // namespace server
} // namespace pb
} // namespace futures
#endif // FUTURES_CELL_PROXY_SRC_RPC_AUTHEN_USER_SERVER_WRAPPER_H
