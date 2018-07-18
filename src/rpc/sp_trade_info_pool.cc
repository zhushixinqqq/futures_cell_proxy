// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#include <string>

#include "futures_cell_proxy/src/rpc/sp_trade_info_pool.h"
#include "futures_cell_proxy/src/config.h"

namespace futures { namespace pb { namespace server {

std::shared_ptr<SPTradeInfoWrapper> SPTradeInfoClientPool::NewClientPtr() {
  std::shared_ptr<SPTradeInfoWrapper> client_ptr=
    std::shared_ptr<SPTradeInfoWrapper> (new SPTradeInfoWrapper(FLAGS_sp_trade_info_server_ip,
          FLAGS_sp_trade_info_server_port));
  return client_ptr;
}
} // namesapce server
} // namespace pb
} // namespace futures

