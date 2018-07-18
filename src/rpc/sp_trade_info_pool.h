// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#ifndef FUTURES_CELL_PROXY_SRC_RPC_SP_TRADE_INFO_POOL_H
#define FUTURES_CELL_PROXY_SRC_RPC_SP_TRADE_INFO_POOL_H

#include <memory>
#include <string>
#include "futures_cell_proxy/src/rpc/client_tmplate.h"
#include "futures_cell_proxy/src/rpc/sp_trade_info_wrapper.h"

namespace futures { namespace pb { namespace server {

class SPTradeInfoClientPool : public ClientPool<SPTradeInfoWrapper, SPTradeInfoClientPool, true> {
  protected:
    virtual std::shared_ptr<SPTradeInfoWrapper> NewClientPtr();
    virtual std::string ClientStr() {
      return "SPTradeInfoClientPool";
    }
};

typedef std::shared_ptr<SPTradeInfoWrapper> SPTradeInfoClientPtr;

} // namespace server
} // namespace pb
} // namespace futures
#endif // FUTURES_CELL_PROXY_SRC_RPC_SP_TRADE_INFO_POOL_H


