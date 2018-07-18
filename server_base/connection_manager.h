/*
 *  author liuxiaobing@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 * */
#ifndef FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_CONNECTION_MANAGER_H_
#define FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_CONNECTION_MANAGER_H_

#include <set>

#include "boost/thread/mutex.hpp"

#include "futures_cell_proxy/server_base/connection.h"
#include "futures_cell_proxy/server_base/connection_manager_base.h"

namespace http { namespace server_base {

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
class ConnectionManager : public ConnectionManagerBase {
  public:
    ConnectionManager();
    ~ConnectionManager();
  public:
    /// Add the specified connection to the manager and start it.
    virtual void Start(ConnectionPtr c);

    /// Stop the specified connection.
    virtual void Stop(ConnectionPtr c);

    /// Erase the specified connection, i.e., don't manager it
    virtual void Erase(ConnectionPtr c);

    /// Stop all connections.
    virtual void StopAll();

  private:
    void MonitorThread();

  private:
    /// The managed connections.
    std::set<ConnectionPtr> m_connections;
    boost::mutex  m_connection_mutex;
    boost::shared_ptr<boost::thread> m_monitor_thread;
    bool m_is_stopping;
};

} // namespace server_base
} // namespace http

#endif // FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_CONNECTION_MANAGER_H_
