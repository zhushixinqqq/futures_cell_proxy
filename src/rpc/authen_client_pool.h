// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#ifndef FUTURES_CELL_PROXY_SRC_RPC_AUTHEN_CLIENT_POOL_H
#define FUTURES_CELL_PROXY_SRC_RPC_AUTHEN_CLIENT_POOL_H

#include <memory>
#include <string>
#include "futures_cell_proxy/src/rpc/authen_user_server_wrapper.h"
#include "futures_cell_proxy/src/rpc/client_tmplate.h"

namespace futures { namespace pb { namespace server {
class AuthenClientPool : public ClientPool<AuthenServerWrapper, AuthenClientPool, true> {
  protected:
    virtual std::shared_ptr<AuthenServerWrapper> NewClientPtr();
    virtual std::string ClientStr() {
      return "AuthenClientPool";
    }
};

typedef std::shared_ptr<AuthenServerWrapper> AuthenClientPtr;

} // namespace server
} // namespace pb
} // namespace futures
#endif // FUTURES_CELL_PROXY_SRC_RPC_AUTHEN_CLIENT_POOL_H
