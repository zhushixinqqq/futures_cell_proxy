/*
 *  封装连接相关信息
 *  author liuxiaobing@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 *
 *  */
#include "futures_cell_proxy/server_base/connection.h"

#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/thread.hpp>
#include <glog/logging.h>
#include <vector>

#include "futures_cell_proxy/server_base/connection_manager.h"
#include "futures_cell_proxy/server_base/handler.h"

using formax_social::service_monitor::InfoCollector;

namespace http { namespace server_base {

Connection::Connection(boost::asio::io_service *io_service,
    ConnectionManagerBase *manager, HandlerPtr handler)
  : m_is_long_connection(false),
    m_socket(*io_service),
    m_connection_manager(manager),
    m_request_handler(handler),
    m_request_parser(new request_parser),
    m_io_service(io_service),
    m_conn_strand(*io_service),
    m_active_time(time(NULL)) {
}

boost::asio::ip::tcp::socket& Connection::Socket() {
  return m_socket;
}

bool Connection::IsLongConnection() {
  return m_is_long_connection;
}

void Connection::SetConnectionMgr(ConnectionManagerBase *conn_mgr) {
  m_connection_manager = conn_mgr;
}

ConnectionManagerBase * Connection::GetConnectionMgr() {
  return m_connection_manager;
}

bool Connection::Start() {
  try {
    m_endpoint = m_socket.remote_endpoint();
  } catch (const std::exception& e) {
    VLOG(3) << "[Connection::Start] call remote_endpoint failed: " << e.what();
      return false;
  }
  VLOG(50) << "connection start read " << m_endpoint;
  m_socket.async_read_some(boost::asio::buffer(m_buffer),
    m_conn_strand.wrap(
      boost::bind(&Connection::HandleRead, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred)));
  return true;
}

void Connection::Stop() {
  //LOG(INFO) << (m_is_long_connection? "Long connection " : "") << "m_socket close " << m_endpoint;
  //m_socket.close();
  m_conn_strand.post(
      boost::bind(&Connection::DoClose, shared_from_this()));
}

void Connection::DoClose() {
  VLOG(350) <<__func__<< (m_is_long_connection? " Long connection " : " connection")
    << " m_socket close " << m_endpoint;
  m_socket.close();
}

void Connection::Reply() {
  VLOG(90) <<__func__<< " reply with status: " << m_reply.status << " to endpoint: " << m_endpoint;
  if (m_reply.status != reply::ok) {
    std::string  func("CellProxyService::http_response_code_");
    if (!m_request.uri.empty()) {
      func.append(m_request.uri);
    } else {
      func.append("unknown");
    }
    if (!InfoCollector::ReportReplyCode("security/cell_proxy", func, m_reply.status)) {
      LOG(ERROR) << __func__ << "| ReportReplyCode failed";
    }
  }
  boost::asio::async_write(m_socket, m_reply.to_buffers(),
    m_conn_strand.wrap(
      boost::bind(&Connection::HandleWrite, shared_from_this(),
        boost::asio::placeholders::error)));
}

void Connection::PushMsg(const string& push_msg, PushCallback caller) {
  m_request_handler->BuidlReply(push_msg, &m_reply);
  // std::vector<boost::asio::const_buffer> buffers;
  // VLOG(60) << "push msg ["<< push_msg << "] to endpoint: " << m_endpoint;
  boost::asio::async_write(m_socket, m_reply.to_buffers(), m_conn_strand.wrap(
    boost::bind(caller, boost::asio::placeholders::error)));
  // try {
  //   boost::asio::write(m_socket, m_reply.to_buffers(), boost::asio::transfer_all());
  // } catch (const std::exception& e) {
  //   VLOG(3) << "[Connection::PushMsg] call write to endpoint: " << m_endpoint
  //     << ", reason: " << e.what();
  //   return false;
  // }
  // return true;
}

void Connection::HandleRead(const boost::system::error_code& e, std::size_t bytes_transferred) {
  if (!e) {
    UpdateActivieTime();
    boost::tribool result;
    VLOG(50) << "total read len: " << bytes_transferred << " from endpoint: " << m_endpoint;
    // 处理可能的包一次收取不完整
    // 内部处理采用reenter 方式
    boost::tie(result, boost::tuples::ignore) = m_request_parser->parse(
      m_request, m_buffer.data(), m_buffer.data() + bytes_transferred);
    if (result) {
      m_request_parser.reset(new request_parser);
      if (!m_is_long_connection) {
        m_is_long_connection = RequestKeepAlive();
      }
      m_request_handler->HandleRequest(shared_from_this(), m_request, m_reply);
      VLOG(50) << "handle request: " << m_request.uri;
      Reply();
      m_request.Reset();
      // TODO(xiaobing) 如果是长连接 就可以继续读取
      /*
      m_socket.async_read_some(boost::asio::buffer(m_buffer),
        boost::bind(&Connection::HandleRead, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
          */
    } else if (!result) {
      // 解析失败
      std::string req = m_buffer.data();
      std::size_t pos = req.find("\r\n\r\n");
      if (pos > 0) {
        std::string  httpheader = req.substr(0,pos);
        LOG(ERROR) << "http read fail httpheader:" << httpheader;
      } else {
        LOG(ERROR) << "http read fail";
      }
      m_reply = reply::stock_reply(reply::bad_request);
      Reply();
      m_request.Reset();
    } else {
      // 数据还没收取完整 继续收取
      // buffer可以覆盖， 内部解析采用reenter方式
      LOG(WARNING) << "http read not complete from endpoint " << m_endpoint;
      m_socket.async_read_some(boost::asio::buffer(m_buffer),
        m_conn_strand.wrap(
          boost::bind(&Connection::HandleRead, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred)));
    }
  } else if (e != boost::asio::error::operation_aborted) {
    LOG(ERROR) <<__func__<<  " error: " << e.message() << " from endpoint: " << m_endpoint;
    m_connection_manager->Stop(shared_from_this());
  } else {
    LOG(ERROR) <<__func__<<  " error: " << e.message() << " from endpoint: " << m_endpoint;
    m_connection_manager->Stop(shared_from_this());
  }
}

void Connection::HandleWrite(const boost::system::error_code& e) {
  if (!e) {
    /*
       m_timer = boost::shared_ptr<boost::asio::deadline_timer>(
       new boost::asio::deadline_timer(*m_io_service));
    // t.expires_at(boost::posix_time::microsec_clock::universal_time()
    //  + boost::posix_time::seconds(3));
    m_timer->expires_from_now(boost::posix_time::seconds(5));
    m_timer->async_wait(boost::bind(&Connection::Reply, shared_from_this()));
    boost::asio::ip::address remote_ad = m_socket.remote_endpoint().address();
    LOG(ERROR) << "handle_write, write 5 seconds later, addr: "
    << remote_ad.to_string() << std::endl;
    boost::asio::ip::address remote_ad = m_socket.remote_endpoint().address();
    */
    VLOG(350) <<__func__<< " response addr:" << m_endpoint << (m_is_long_connection ? " long connection ": " complete");
    if (reply::ok != m_reply.status) {
      m_connection_manager->Stop(shared_from_this());
      return;
    }
    if (!m_is_long_connection) {
      m_connection_manager->Stop(shared_from_this());
    } else {
      VLOG(100) <<__func__<< " Find long connection :" << EndPoint() << " wait next read";
      m_socket.async_read_some(boost::asio::buffer(m_buffer),
        m_conn_strand.wrap(
          boost::bind(&Connection::HandleRead, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred)));
    }
  } else if (e != boost::asio::error::operation_aborted) {
    LOG(ERROR) <<__func__<< " response addr:" << m_endpoint << " failed connection closed";
    m_connection_manager->Stop(shared_from_this());
  } else {
    LOG(ERROR) <<__func__<< " response addr:" << m_endpoint << " failed connection closed";
    m_connection_manager->Stop(shared_from_this());
  }
}

bool Connection::RequestKeepAlive() {
  std::vector<header>::const_iterator it = m_request.headers.begin();
  for (; it != m_request.headers.end(); ++it) {
    const string name = boost::algorithm::to_lower_copy(it->name);
    const string value = boost::algorithm::to_lower_copy(it->value);
    if (name == "connection" && value == "keep-alive") {
      return true;
    }
  }

  return false;
}

} // namespace server_base
} // namespace http
