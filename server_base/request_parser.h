//
// request_parser.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_REQUEST_PARSER_H_
#define FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_REQUEST_PARSER_H_

#include <string>

#include "boost/asio/coroutine.hpp"
#include "boost/logic/tribool.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/algorithm/string.hpp"

#include "common/time_cost/time_cost.h"

namespace http {
namespace server_base {

struct request;


/// Parser for incoming requests.
class request_parser : boost::asio::coroutine
{
public:
  /// Parse some data. The tribool return value is true when a complete request
  /// has been parsed, false if the data is invalid, indeterminate when more
  /// data is required. The InputIterator return value indicates how much of the
  /// input has been consumed.
 /*
  template <typename InputIterator>
  boost::tuple<boost::tribool, InputIterator> parse(request& req,
  InputIterator begin, InputIterator end)
  {
  formax::common::TimeCost ts("request parser parse:");
  while (begin != end)
  {
  boost::tribool result = consume(req, *begin++);
  if (result || !result)
  return boost::make_tuple(result, begin);
  }
  boost::tribool result = boost::indeterminate;
  return boost::make_tuple(result, begin);
  }
  */
  template <typename InputIterator>
  boost::tuple<boost::tribool, InputIterator> parse(request& req,
      InputIterator begin, InputIterator end)
  {
    formax::common::TimeCost ts("request parser parse:");
    boost::tribool result;
    try  {
      req.http_req_data.append(begin, end-begin);
      result = prase_http(req, begin, end-begin);
      if (result || !result)
        return boost::make_tuple(result, end);
      result = boost::indeterminate;
      return boost::make_tuple(result, end);
    } catch (const std::exception& e) {
      LOG(ERROR) << __func__ << "| " << " reason: " << e.what();
      return boost::make_tuple(result, end);
    }
  }

private:
  /// The name of the content length header.
  static std::string content_length_name_;
  static std::string http_end;

  /// Content length as decoded from headers. Defaults to 0.
  std::size_t content_length_;
  std::string m_httpheader;
  char* m_phead;
  char* m_pend;
  std::string linebuf;
  int m_ret;
  int m_offset = 0;
  std::size_t m_content_size;

  /// Handle the next character of input.
  boost::tribool consume(request& req, char input);

  /// Check if a byte is an HTTP character.
  static bool is_char(int c);

  /// Check if a byte is an HTTP control character.
  static bool is_ctl(int c);

  /// Check if a byte is defined as an HTTP tspecial character.
  static bool is_tspecial(int c);

  /// Check if a byte is a digit.
  static bool is_digit(int c);

  /// Check if two characters are equal, without regard to case.
  static bool tolower_compare(char a, char b);

  /// Check whether the two request header names match.
  static bool headers_equal(const std::string& a, const std::string& b);

  static bool is_white_space(char * chunk);
  static void trim(char * chunk, int * trimmed);
  static int  getline_from_buf(const char* src, char* line, int size, int& offset);
  static int  getline_from_buf2(const char* src, std::string& linebuf, int& offset);
  static bool ncmp(const char * string1, const char * string2, int bytes);
  static bool get_http_uri(request& req, const char* line, const int len);
  static bool get_mime_header(request& req, const char * buffer, int buffer_size);
  boost::tribool prase_http(request& req, char*& a, size_t len);
};
} // namespace server_base
} // namespace http

#endif // FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_REQUEST_PARSER_H_
