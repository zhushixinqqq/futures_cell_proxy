/*
 *  封装连接相关信息
 *  author liuxiaobing@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 *
 *  */
#ifndef FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_CONNECTION_H_
#define FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_CONNECTION_H_

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <gflags/gflags.h>
#include <string>

#include "futures_cell_proxy/server_base/reply.h"
#include "futures_cell_proxy/server_base/request.h"
#include "futures_cell_proxy/server_base/request_parser.h"
#include "thirdparty/service_monitor/sender/info_collector.h"

namespace http { namespace server_base {

using std::string;
class ConnectionManagerBase;
class Handler;
typedef boost::shared_ptr<Handler> HandlerPtr;
typedef boost::shared_ptr<Connection> ConnectionPtr;
typedef boost::function<void (const boost::system::error_code&)> PushCallback;

class Connection
  : public boost::enable_shared_from_this<Connection>,
  private boost::noncopyable {
 public:
  explicit Connection(boost::asio::io_service *io_service,
    ConnectionManagerBase *manager, HandlerPtr handler);
  // 在连接上开始接受请求
  bool Start();
  // 断掉连接上的请求
  void Stop();

  void DoClose();
  const boost::asio::ip::tcp::endpoint& EndPoint() {
    return m_endpoint;
  }
  boost::asio::ip::tcp::socket& Socket();

  bool IsLongConnection();
  void SetConnectionMgr(ConnectionManagerBase *conn_mgr);
  ConnectionManagerBase * GetConnectionMgr();

  void PushMsg(const string& push_msg, PushCallback caller);
  const boost::asio::ip::tcp::endpoint& Getendpoint() {return m_endpoint;}

  bool IsTimeOut() { return time(NULL) - m_active_time > 330;}

 private:
  // 接收数据完成回调
  void HandleRead(const boost::system::error_code& e,
    std::size_t bytes_transferred);
  // 写入完成回调
  void HandleWrite(const boost::system::error_code& e);
  void Reply();

  bool RequestKeepAlive();

  void UpdateActivieTime() {m_active_time = time(0);};

 private:
  bool m_is_long_connection;
  // 连接套接字
  boost::asio::ip::tcp::socket m_socket;
  ConnectionManagerBase *m_connection_manager;
  // 请求处理类
  HandlerPtr m_request_handler;
  // 接收buffer
  enum {
    s_http_recv_buf = 4096
  };
  boost::array<char, s_http_recv_buf> m_buffer;
  // 解析出来的请求对象
  request m_request;
  boost::shared_ptr<request_parser> m_request_parser;
  reply m_reply;
  boost::asio::io_service *m_io_service;
  boost::shared_ptr<boost::asio::deadline_timer> m_timer;
  boost::asio::ip::tcp::endpoint  m_endpoint;
  boost::asio::strand m_conn_strand;

  time_t m_active_time;

};

} // namespace server_base
} // namespace http

#endif // FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_CONNECTION_H_
