/*
 *  author zhushixin@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 * */
#ifndef FUTURES_CELL_PROXY_SRC_RPC_CLIENT_TMPLATE_H
#define FUTURES_CELL_PROXY_SRC_RPC_CLIENT_TMPLATE_H

#include <glog/logging.h>
// #include "thirdparty/boost/shared_ptr.hpp"
#include <memory>
#include <mutex>
#include <set>

namespace futures { namespace pb { namespace server {

// 模版参数:  ClientType 操作对象
//        2:  PoolType 对象池名字
template <class ClientType, class PoolType, bool client_reuse>
class ClientPool {
 public:
  typedef std::shared_ptr<ClientType> ShrClientPtr;

  // 获取对象池
  static PoolType* Instance() {
    return &ClientPool::this_pool;
  }

  // 获取对象
  ShrClientPtr GetClientPtr() {
    std::lock_guard<std::mutex> lock(m_clients_mutex);
    auto it = m_client_set.begin();
    if ( it != m_client_set.end() ) {
      ShrClientPtr client = *it;
      m_client_set.erase(it);
      return client;
    }
    return NewClientPtr();
  }

  // 释放对象 usable标识对像是否还可用
  void FreeClientPtr(ShrClientPtr client, bool usable) {
    std::lock_guard<std::mutex> lock(m_clients_mutex);
    if (usable) {
      m_client_set.insert(client);
    } else {
      return;
    }
  }

 protected:
  ClientPool() {}
  virtual ~ClientPool() {}
  // 需要重写 如何构造对象
  virtual ShrClientPtr NewClientPtr() = 0;

 private:
  ClientPool(const ClientPool&);
  ClientPool& operator=(const ClientPool&);

  static PoolType           this_pool;
  std::set<ShrClientPtr>  m_client_set;
  std::mutex              m_clients_mutex;
};

template <typename ClientType, typename PoolType, bool client_reuse>
PoolType ClientPool<ClientType, PoolType, client_reuse>::this_pool;

} // namespace server
} // namepspace pb
} // namespace cell_proxy

#endif // FUTURES_CELL_PROXY_SRC_RPC_CLIENT_TMPLATE_H
