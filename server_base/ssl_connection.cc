#include <vector>
#include <boost/bind.hpp>
#include "boost/thread/thread.hpp"
#include "thirdparty/glog/logging.h"
#include "ssl_connection.h"
#include "connection_manager.h"
#include "request_handler.h"



namespace http { namespace server {

Connection::Connection(boost::asio::io_service& io_service,
    boost::asio::ssl::context& context,
    ConnectionManager& manager, RequestHandler& handler)
  : m_socket(io_service, context),
    m_connection_manager(manager),
    m_request_handler(handler),
    m_io_service(&io_service)
{
}

boost::asio::ip::tcp::socket& Connection::Socket()
{
  return m_socket.lowest_layer();
}

void Connection::Start() {
  // 握手ssl
  m_socket.async_handshake(boost::asio::ssl_stream_base::server,
    boost::bind(&Connection::HandleHandShak, this, boost::asio::placeholders::error));
}

void Connection::HandleHandShak(const boost::system::error_code& error)
{
  if (!error) {
    LOG(INFO) << "connection start read";
    m_socket.async_read_some(boost::asio::buffer(m_buffer),
      boost::bind(&Connection::HandleRead, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
  } else {
    LOG(ERROR) << "ssl hand shake failed!";
    delete this;
  }
}

void Connection::Stop()
{
  m_socket.close();
}

void Connection::Reply() {
  LOG(ERROR) << "reply: " << m_reply.status;
  boost::asio::async_write(m_socket, m_reply.to_buffers(),
    boost::bind(&Connection::HandleWrite, shared_from_this(),
      boost::asio::placeholders::error));
}

void Connection::HandleRead(const boost::system::error_code& e,
  std::size_t bytes_transferred)
{
  if (!e)
  {
    boost::tribool result;
    LOG(ERROR) << std::string(m_buffer.begin(), m_buffer.end()) << std::endl
      << " total len: " << bytes_transferred;


    boost::tie(result, boost::tuples::ignore) = m_request_parser.parse(
      m_request, m_buffer.data(), m_buffer.data() + bytes_transferred);

    if (result)
    {
      m_request.client_ptr = shared_from_this();
      m_request_handler.HandleRequest(m_request, m_reply);
      LOG(INFO) << "handle request: " << m_request.uri;
      Reply();
      m_socket.async_read_some(boost::asio::buffer(m_buffer),
        boost::bind(&Connection::HandleRead, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    }
    else if (!result)
    {
      LOG(ERROR) << "http read fail";
      m_reply = reply::stock_reply(reply::bad_request);
      boost::asio::async_write(m_socket, m_reply.to_buffers(),
        boost::bind(&Connection::HandleWrite, shared_from_this(),
          boost::asio::placeholders::error));
    }
    else
    {
      LOG(ERROR) << "http read not complete";
      m_socket.async_read_some(boost::asio::buffer(m_buffer),
        boost::bind(&Connection::HandleRead, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    }
  }
  else if (e != boost::asio::error::operation_aborted)
  {
    m_connection_manager.Stop(shared_from_this());
  }
  else {
    m_connection_manager.Stop(shared_from_this());
  }
}

void Connection::HandleWrite(const boost::system::error_code& e)
{
  if (!e)
  {
    m_timer = boost::shared_ptr<boost::asio::deadline_timer>(
      new boost::asio::deadline_timer(*m_io_service));
    // t.expires_at(boost::posix_time::microsec_clock::universal_time()
    //  + boost::posix_time::seconds(3));
    m_timer->expires_from_now(boost::posix_time::seconds(5));
    m_timer->async_wait(boost::bind(&Connection::Reply, shared_from_this()));
    boost::asio::ip::address remote_ad = m_socket.remote_endpoint().address();
    LOG(ERROR) << "handle_write, write 5 seconds later, addr: "
      << remote_ad.to_string() << std::endl;
    /*
       boost::asio::ip::address remote_ad = m_socket.remote_endpoint().address();
       LOG(INFO) << "handle_write, addr:" << remote_ad.to_string() << std::endl;
     */
  } else if (e != boost::asio::error::operation_aborted) {
    m_connection_manager.Stop(shared_from_this());
    std::cerr << "handle_write, connection close" ;
  } else {
    m_connection_manager.Stop(shared_from_this());
    std::cerr << "handle_write, connection close";
  }
}

} // namespace server
}
