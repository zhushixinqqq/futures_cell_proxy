/*
 *  author liuxiaobing@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 * */
#ifndef FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_HANDLER_H_
#define FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_HANDLER_H_

#include <list>
#include <map>
#include <string>

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/noncopyable.hpp"
#include "boost/shared_ptr.hpp"
#include "futures_cell_proxy/server_base/connection.h"


namespace http { namespace server_base {

using std::string;

struct reply;
struct request;
// class Connection;

// handler base class
class Handler : private boost::noncopyable {
public:
  /// Construct with a directory containing files to be served.
  explicit Handler(const std::string& doc_root);
  virtual ~Handler();

  /// Handle a request and produce a reply.
  virtual void HandleRequest(ConnectionPtr conn, const request& req, reply& rep);

  void BuidlReply(const string& reply_msg, reply* rep);

protected:
  typedef std::map<std::string,
    boost::function<void (const string& in_data, string* out_data)> > HandleMap;
  HandleMap m_handle_map;

  void ProccessReqFromHandle(ConnectionPtr conn, const string& req_path,  const request& req,
      reply* rep);

  /// Perform URL-decoding on a string. Returns false if the encoding was
  /// invalid.
  static bool UrlDecode(const std::string& in, std::string& out);

protected:
  /// The directory containing the files to be served.
  std::string m_doc_root;

public:
  boost::asio::ip::tcp::endpoint  m_endpoint;
};

typedef boost::shared_ptr<Handler> HandlerPtr;

} // namespace server_base
} // namespace http

#endif // FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_HANDLER_H_
