/*
 *  author liuxiaobing@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 * */
#include <boost/thread.hpp>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "futures_cell_proxy/server_base/server_base.h"

// #include "sp_cell_proxy/server/helper.h"
DEFINE_int32(server_thread_num, 100, "default server num");

namespace http { namespace server_base {

ServerBase::ServerBase(
  const std::string& address, const std::string& port, HandlerPtr handler)
  : m_io_service(), m_stop_signals(m_io_service),
  m_acceptor(m_io_service), m_connection_manager(),
  m_request_handler(handler), m_new_connection() {

    // 注册推出函数
    m_stop_signals.add(SIGINT);
    m_stop_signals.add(SIGTERM);
#if defined(SIGQUIT)
    m_stop_signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    m_stop_signals.async_wait(boost::bind(&ServerBase::ServerStop, this));

    boost::asio::ip::tcp::resolver resolver(m_io_service);
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    m_acceptor.open(endpoint.protocol());
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    m_acceptor.bind(endpoint);
    m_acceptor.listen();
    StartAcceptConnection();
  }

void ServerBase::run() {
  // 启动异步处理服务
  // 启动多个线程 在每个线程上面跑 run
  for (int32_t thread_index = 0; thread_index < FLAGS_server_thread_num; ++thread_index) {
    boost::function0<void>
      client_func = boost::bind(&ServerBase::WorkerThread, this);
    boost::shared_ptr<boost::thread> new_thread(new boost::thread(client_func));
  }
  LOG(INFO) << "ServerBase::run()";
  m_io_service.run();
}

void ServerBase::WorkerThread() {
  LOG(INFO) << "worker thread start";
  m_io_service.run();
  LOG(INFO) << "worker thread end";
}

void ServerBase::StartAcceptConnection() {
  m_new_connection.reset(new Connection(&m_io_service, &m_connection_manager, m_request_handler));
  m_acceptor.async_accept(m_new_connection->Socket(),
    boost::bind(&ServerBase::HandleNewConnection, this, boost::asio::placeholders::error));
}

void ServerBase::HandleNewConnection(const boost::system::error_code& e) {
  if (!m_acceptor.is_open()) {
    return;
  }
  if (!e) {
    // 这里不用加锁
    m_connection_manager.Start(m_new_connection);
    // 继续接受请求
    StartAcceptConnection();
  } else {
    // accept出错？
    LOG(ERROR) << "accept error: " << e;
  }
}

void ServerBase::ServerStop() {
  // 停接收socket
  m_acceptor.close();
  m_connection_manager.StopAll();
}

} // namespace server_base
} // namespace http
