//
// request.h
// ~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_REQUEST_H_
#define FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_REQUEST_H_

#include <string>
#include <vector>

#include "boost/shared_ptr.hpp"

#include "futures_cell_proxy/server_base/header.h"

namespace http { namespace server_base {

class Connection;

/// A request received from a client.
struct request
{
  /// The request method, e.g. "GET", "POST".
  std::string method;

  /// The requested URI, such as a path to a file.
  std::string uri;

  /// Major version number, usually 1.
  int http_version_major;

  /// Minor version number, usually 0 or 1.
  int http_version_minor;

  /// The headers included with the request.
  std::vector<header> headers;

  /// The optional content sent with the request.
  std::string content;
  // has parse header
  bool bheader;
  // content lenfth
  size_t content_length_;
  //
public:
  std::string http_req_data;
  std::string req_cmd;
public:
  void Reset() {
    method = "";
    uri = "";
    http_version_major = 0;
    http_version_minor = 0;
    headers.clear();
    content = "";
    bheader = false;
    content_length_ = 0;
    http_req_data = "";
    req_cmd = "";
  }
};

} // namespace server_base
} // namespace http

#endif // FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_REQUEST_H_
