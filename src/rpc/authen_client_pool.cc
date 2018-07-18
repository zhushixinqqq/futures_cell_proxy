// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#include <string>
#include "futures_cell_proxy/src/config.h"
#include "futures_cell_proxy/src/rpc/authen_client_pool.h"

namespace futures { namespace pb { namespace server {

std::shared_ptr<AuthenServerWrapper> AuthenClientPool::NewClientPtr() {
  std::shared_ptr<AuthenServerWrapper> client_ptr=
    std::shared_ptr<AuthenServerWrapper> (new AuthenServerWrapper(FLAGS_authen_server_ip,
          FLAGS_authen_server_port));
  return client_ptr;
}
} // namesapce serve
} // namespace pb
} // namespace futures


