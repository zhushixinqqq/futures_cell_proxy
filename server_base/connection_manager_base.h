/*
 *  author zhaoxiaoli@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-06-30
 * */
#ifndef FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_CONNECTION_MANAGER_BASE_H_
#define FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_CONNECTION_MANAGER_BASE_H_

#include "boost/noncopyable.hpp"

namespace http { namespace server_base {

/// Manages open connections so that they may be cleanly stopped when the server
/// needs to shut down.
class ConnectionManagerBase : private boost::noncopyable {
 public:
  virtual ~ConnectionManagerBase() {}

  /// Add the specified connection to the manager and start it.
  virtual void Start(ConnectionPtr c) = 0;

  /// Stop the specified connection.
  virtual void Stop(ConnectionPtr c) = 0;

  /// Erase the specified connection, i.e., don't manager it
  virtual void Erase(ConnectionPtr c) = 0;

  /// Stop all connections.
  virtual void StopAll() = 0;
};

} // namespace server_base
} // namespace http

#endif // FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_CONNECTION_MANAGER_BASE_H_
