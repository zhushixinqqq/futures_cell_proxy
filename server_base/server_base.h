/*
 *  author liuxiaobing@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 * */
#ifndef FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_SERVER_BASE_H
#define FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_SERVER_BASE_H

#include <map>
#include <string>

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/noncopyable.hpp"

#include "futures_cell_proxy/server_base/connection.h"
#include "futures_cell_proxy/server_base/connection_manager.h"
#include "futures_cell_proxy/server_base/handler.h"

namespace http { namespace server_base {

class ServerBase : private boost::noncopyable {
 public:
  ServerBase(const std::string& address, const std::string& port, HandlerPtr handler);
  // void RegisterInterface(
  //   const std::string& interface_name,
  //   boost::function<std::string (std::string&)> func) {
  //   server_interfaces[interface_name] = func;
  // }

  void run();

 private:
  void StartAcceptConnection();
  void HandleNewConnection(const boost::system::error_code& e);
  void ServerStop();
  void WorkerThread();

 private:
  // std::map<std::string, boost::function<std::string (std::string&)> > server_interfaces;

  boost::asio::io_service m_io_service;
  boost::asio::signal_set m_stop_signals;
  boost::asio::ip::tcp::acceptor m_acceptor;
  ConnectionManager m_connection_manager;
  HandlerPtr m_request_handler;
  ConnectionPtr m_new_connection;
};

/*
class MyProtoServer : public ServerBase {
 public:
  MyProtoServer() {
    RegisterInterface("TestInterface",
      boost::bind(&MyProtoServer::base_TestInterface, this, _1));
  }
  TestResponse TestInterface(TestRequest& req);
  std::string base_TestInterface(const std::string& in_data);
};
*/

} // namespace server_base
} // namespace http

#endif // FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_SERVER_BASE_H
