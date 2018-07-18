/*
 *  author liuxiaobing@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 *
 *  */
#ifndef FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_SSL_CONNECTION_H_
#define FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_SSL_CONNECTION_H_

#include "boost/array.hpp"
#include "boost/asio.hpp"
#include "boost/asio/ssl.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/noncopyable.hpp"
#include "boost/shared_ptr.hpp"

#include "sp_cell_proxy/server/server_base/reply.h"
#include "sp_cell_proxy/server/server_base/request.h"
#include "sp_cell_proxy/server/server_base/request_handler.h"
#include "sp_cell_proxy/server/server_base/request_parser.h"

namespace http { namespace server {

class ConnectionManager;
class Connection
  : public boost::enable_shared_from_this<Connection>,
  private boost::noncopyable {

   public:
    explicit Connection(boost::asio::io_service& io_service,
      boost::Asio::ssl::context& context,
      ConnectionManager& manager, RequestHandler& handler);
    // 在连接上开始接受请求
    void Start();

    // 断掉连接上的请求
    void Stop();

    boost::asio::ip::tcp::socket& Socket();

   private:
    // 接收数据完成回调
    void HandleRead(const boost::system::error_code& e,
      std::size_t bytes_transferred);

    // 写入完成回调
    void HandleWrite(const boost::system::error_code& e);

    void Reply();

   private:
    // 连接套接字
    // boost::asio::ip::tcp::socket m_socket;

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> m_socket;
    //
    ConnectionManager& m_connection_manager;

    // 请求处理类
    RequestHandler& m_request_handler;

    // 接收buffer
    boost::array<char, 8192> m_buffer;
    // 解析出来的请求对象
    request m_request;
    request_parser m_request_parser;
    reply m_reply;
    boost::asio::io_service* m_io_service;
    boost::shared_ptr<boost::asio::deadline_timer> m_timer;
  };
typedef boost::shared_ptr<Connection> ConnectionPtr;

} // namespace server
} // namespace http
#endif // FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_SSL_CONNECTION_H_
