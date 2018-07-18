//
// request_parser.cpp
// ~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request.h"
#include "request_parser.h"

#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <cctype>
#include "glog/logging.h"

namespace http {
namespace server_base {
#include <boost/asio/yield.hpp>

// Enable the pseudo-keywords reenter, yield and fork.

std::string request_parser::content_length_name_ = "Content-Length";
std::string request_parser::http_end = "\r\n\r\n";

boost::tribool request_parser::consume(request& req, char c)
{
  reenter(this)
  {
    req.method.clear();
    req.uri.clear();
    req.http_version_major = 0;
    req.http_version_minor = 0;
    req.headers.clear();
    req.content.clear();
    content_length_ = 0;

    // Request method.
    while (is_char(c) && !is_ctl(c) && !is_tspecial(c) && c != ' ')
    {
      req.method.push_back(c);
      yield return boost::indeterminate;
    }
    if (req.method.empty())
      return false;

    // Space.
    if (c != ' ') return false;
    yield return boost::indeterminate;

    // URI.
    while (!is_ctl(c) && c != ' ')
    {
      req.uri.push_back(c);
      yield return boost::indeterminate;
    }
    if (req.uri.empty()) return false;

    // Space.
    if (c != ' ') return false;
    yield return boost::indeterminate;

    // HTTP protocol identifier.
    if (c != 'H') return false;
    yield return boost::indeterminate;
    if (c != 'T') return false;
    yield return boost::indeterminate;
    if (c != 'T') return false;
    yield return boost::indeterminate;
    if (c != 'P') return false;
    yield return boost::indeterminate;

    // Slash.
    if (c != '/') return false;
    yield return boost::indeterminate;

    // Major version number.
    if (!is_digit(c)) return false;
    while (is_digit(c))
    {
      req.http_version_major = req.http_version_major * 10 + c - '0';
      yield return boost::indeterminate;
    }

    // Dot.
    if (c != '.') return false;
    yield return boost::indeterminate;

    // Minor version number.
    if (!is_digit(c)) return false;
    while (is_digit(c))
    {
      req.http_version_minor = req.http_version_minor * 10 + c - '0';
      yield return boost::indeterminate;
    }

    // CRLF.
    if (c != '\r') return false;
    yield return boost::indeterminate;
    if (c != '\n') return false;
    yield return boost::indeterminate;

    // Headers.
    while ((is_char(c) && !is_ctl(c) && !is_tspecial(c) && c != '\r')
        || (c == ' ' || c == '\t'))
    {
      if (c == ' ' || c == '\t')
      {
        // Leading whitespace. Must be continuation of previous header's value.
        if (req.headers.empty()) return false;
        while (c == ' ' || c == '\t')
          yield return boost::indeterminate;
      }
      else
      {
        // Start the next header.
        req.headers.push_back(header());

        // Header name.
        while (is_char(c) && !is_ctl(c) && !is_tspecial(c) && c != ':')
        {
          req.headers.back().name.push_back(c);
          VLOG(100) << "header: " << req.headers.back().name;
          yield return boost::indeterminate;
        }

        // Colon and space separates the header name from the header value.
        if (c != ':') return false;
        yield return boost::indeterminate;
        if (c != ' ') return false;
        yield return boost::indeterminate;
      }

      // Header value.
      while (is_char(c) && !is_ctl(c) && c != '\r')
      {
        req.headers.back().value.push_back(c);
        VLOG(100) << "header: " << req.headers.back().name
          << " value: " << req.headers.back().value;

        yield return boost::indeterminate;
      }

      // CRLF.
      if (c != '\r') return false;
      yield return boost::indeterminate;
      if (c != '\n') return false;
      yield return boost::indeterminate;
    }

    // CRLF.
    if (c != '\r') return false;
    yield return boost::indeterminate;
    if (c != '\n') return false;

    // Check for optional Content-Length header.
    for (std::size_t i = 0; i < req.headers.size(); ++i)
    {
      if (headers_equal(req.headers[i].name, content_length_name_))
      {
        try
        {
          content_length_ =
            boost::lexical_cast<std::size_t>(req.headers[i].value);
          VLOG(80) << "content length: " << content_length_;
        }
        catch (boost::bad_lexical_cast&)
        {
          return false;
        }
      }
    }

    // Content.
    while (req.content.size() < content_length_)
    {
      yield return boost::indeterminate;
      req.content.push_back(c);
      VLOG(100) << "content: " << req.content;
    }
  }
  VLOG(60) << "http parse complete!";
  return true;
}
boost::tribool request_parser::prase_http(request& req, char*& pdata, size_t len) {

  reenter(this) {
    m_phead = NULL;
    m_pend = NULL;
    linebuf.clear();
    m_ret = 0;
    m_offset = 0;
    m_content_size = 0;

    // 接收http包头
    if ((m_pend = strstr(pdata, http_end.c_str())) != NULL && ((m_pend - pdata) < (int32_t)len)) {
      m_phead = pdata;
      m_pend += 4;
    } else {
      do {
        m_httpheader.append(pdata, len);
        if ((m_pend = strstr((char*)m_httpheader.c_str(), http_end.c_str())) != NULL) {
          m_phead = (char*)m_httpheader.c_str();
          m_pend += 4;
          len = m_httpheader.size();
          // LOG(INFO) << "recv complete http:" << m_httpheader;
          break;
        }
        VLOG(80) << "recv Incomplete http:" << m_httpheader;
        yield return boost::indeterminate;
      } while (1);
    }
    // 解析http包头
    while ((m_ret = getline_from_buf2(m_phead, linebuf, m_offset)) > 0 &&
        m_offset < static_cast<int>(len)) {
      if (m_ret == 2 && ncmp(linebuf.c_str(), "\r\n", 2)) break;
      if (ncmp(linebuf.c_str(), "POST ", 5)) {
        if (!get_http_uri(req, linebuf.c_str(), m_ret)) {
          LOG(WARNING) << "get_http_uri failed line:" << linebuf.c_str();
          return false;
        }
        continue;
      }
      if (req.uri.empty()) {
        LOG(WARNING) << "Err not find uri http req:" << linebuf.c_str();
        return false;
      }
      if (!get_mime_header(req, linebuf.c_str(), m_ret)) {
        LOG(WARNING) << "get_mime_header failed line:" << linebuf.c_str();
        return false;
      }
    }
    // 解析mime content length
    for (std::size_t i = 0; i < req.headers.size(); ++i) {
      if (headers_equal(req.headers[i].name, content_length_name_)) {
        try
        {
          VLOG(80) << "get header name:" << req.headers[i].name << "|value:"
            << req.headers[i].value;
          req.content_length_ =
            boost::lexical_cast<std::size_t>(req.headers[i].value);
          req.bheader = true;
          // LOG(INFO) << "content length: " << req.content_length_;
        }
        catch (boost::bad_lexical_cast&)
        {
          VLOG(80) << "Error change content_length for http header:"<< m_phead;
          return false;
        }
      }
    }
    if (!req.bheader) {
      LOG(WARNING) << "Error not find content_length for http header:"<< m_phead;
      return false;
    }
    // 接收content
    m_content_size = len - (m_pend-m_phead);
    req.content.append(m_pend, m_content_size);
    if (req.content.size() >= req.content_length_) {
      VLOG(80) << "http parse complete length:" << req.content.size();
      return true;
    }
    yield return boost::indeterminate;
    // 继续接收content
    while (1) {
      if (len < (req.content_length_-req.content.size())) {
        req.content.append(pdata, len);
        pdata +=  len;
      } else {
        req.content.append(pdata, req.content_length_-req.content.size());
        pdata +=  req.content_length_-req.content.size();
      }
      if (req.content.size() >= req.content_length_) {
        VLOG(80) << "http parse complete length:" << req.content.size();
        return true;
      }
      LOG(WARNING) << "http recv Incomplete content_length:" << req.content.size();
      yield return boost::indeterminate;
    }
  }
  return true;
}
// Disable the pseudo-keywords reenter, yield and fork.
#include <boost/asio/unyield.hpp>

bool request_parser::is_char(int c)
{
  return c >= 0 && c <= 127;
}

bool request_parser::is_ctl(int c)
{
  return (c >= 0 && c <= 31) || (c == 127);
}

bool request_parser::is_tspecial(int c)
{
  switch (c)
  {
    case '(': case ')': case '<': case '>': case '@':
    case ',': case ';': case ':': case '\\': case '"':
    case '/': case '[': case ']': case '?': case '=':
    case '{': case '}': case ' ': case '\t':
      return true;
    default:
      return false;
  }
}

bool request_parser::is_digit(int c)
{
  return c >= '0' && c <= '9';
}

bool request_parser::tolower_compare(char a, char b)
{
  return std::tolower(a) == std::tolower(b);
}

bool request_parser::headers_equal(const std::string& a, const std::string& b)
{
  if (a.length() != b.length())
    return false;

  return std::equal(a.begin(), a.end(), b.begin(),
      &request_parser::tolower_compare);
}

int request_parser::getline_from_buf(const char* src, char* line, int size, int& offset) {
  char* pend = NULL;
  if ((pend = strstr((char*)(src + offset), "\r\n")) == NULL) {
    return -1;
  }
  pend += 2;
  int n = pend - (src + offset);
  n = n < size ? n : size;
  strncpy(line, src + offset, n);
  offset += n;
  return n;
}

int request_parser::getline_from_buf2(const char* src, std::string& linebuf, int& offset) {
  char* pend = NULL;
  if ((pend = strstr((char*)(src + offset), "\r\n")) == NULL) {
    return -1;
  }
  pend += 2;
  int n = pend - (src + offset);
  linebuf.clear();
  linebuf.assign(src + offset, n);
  offset += n;
  return n;
}

bool request_parser::is_white_space(char * chunk) {
  /* do not complain about receive a null refernce chunk */
  if (chunk == NULL)
    return false;
  if (chunk[0] == ' ')
    return true;
  if (chunk[0] == '\n')
    return true;
  if (chunk[0] == '\t')
    return true;
  if (chunk[0] == '\r')
    return true;
  /* no white space was found */
  return false;
}

void request_parser::trim(char * chunk, int * trimmed) {
  int    iterator;
  int    iterator2;
  int    end;
  int    total;
  /* perform some environment check */
  if (chunk == NULL)
    return;
  /* check empty string received */
  if (strlen(chunk) == 0) {
    if (trimmed)
      *trimmed = 0;
    return;
  }
  /* check the amount of white spaces to remove from the
   * begin */
  iterator = 0;
  while (chunk[iterator] != 0) {
    /* check that the iterator is not pointing to a white
     * space */
    if (!is_white_space (chunk + iterator))
      break;
    /* update the iterator */
    iterator++;
  }
  /* check for the really basic case where an empty string is found */
  if (iterator == (int)strlen(chunk)) {
    /* an empty string, trim it all */
    chunk[0] = 0;
    if (trimmed)
      *trimmed = iterator;
    return;
  } /* end if */

  /* now get the position for the last valid character in the
   * chunk */
  total   = strlen(chunk) - 1;
  end     = total;
  while (chunk[end] != 0) {
    /* stop if a white space is found */
    if (!is_white_space(chunk + end)) {
      break;
    }
    /* update the iterator to eat the next white space */
    end--;
  }
  /* the number of items trimmed */
  total -= end;
  total += iterator;
  /* copy the exact amount of non white spaces items */
  iterator2 = 0;
  while (iterator2 < (end - iterator + 1)) {
    /* copy the content */
    chunk[iterator2] = chunk[iterator + iterator2];
    /* update the iterator */
    iterator2++;
  }
  chunk[end - iterator + 1] = 0;
  if (trimmed != NULL)
    *trimmed = total;
  /* return the result reference */
  return;
}

bool request_parser::ncmp(const char * string1, const char * string2, int bytes) {
  int iterator;
  if (bytes <= 0)
    return false;
  if (string1 == NULL && string2 == NULL)
    return true;
  if (string1 == NULL || string2 == NULL)
    return false;
  /* next position */
  iterator = 0;
  while (string1[iterator] && string1[iterator] && iterator < bytes) {
    if (string1[iterator] != string2[iterator])
      return false;
    iterator++;
  } /* end while */
  /* last check, ensure both ends with 0 */
  return iterator == bytes;
}

std::string  TrimUri(std::string & uri) {
  boost::algorithm::replace_all(uri, "http://", "");
  boost::algorithm::replace_all(uri, "HTTP://", "");
  std::size_t found = uri.find("?");
  if (found != std::string::npos) {
    uri = uri.substr (0, found);
  }
  found = uri.find("/");
  if (found != std::string::npos && found > 0) {
    uri = uri.substr (0, found);
  }
  return uri;
}

bool request_parser::get_http_uri(request& req, const char* line,
    const int len) {
  int buffsize = len;
  int it = 0;
  while (it <  buffsize && line[it] != ' ' ) it++;
  req.method.append(line, it);

  while (it <  buffsize && line[it] == ' ' ) it++;
  if (line[it] == '/')it++;

  int it2 = it+1;
  while (it2 < len && line[it2] != ' ') it2++;
  if (it2 == len) return false;
  std::string temp_uri;
  temp_uri.append(line+it, it2-it);

  it = it2+1;
  while (it <  buffsize && line[it] == ' ' ) it++;
  // not parse http ver
  req.uri = TrimUri(temp_uri);
  return true;
}

bool request_parser::get_mime_header(request& req, const char * buffer,
    const int buffer_size) {
  int iterator = 0;
  int iterator2 = 0;
  /* ok, find the first : */
  while (iterator < buffer_size && buffer[iterator] && buffer[iterator] != ':')
    iterator++;
  if (buffer[iterator] != ':') {
    LOG(WARNING) << __func__ << "| failed, not find line:" << buffer << "len:"
      << buffer_size;
    return false;
  }
  std::string tmpvalue;
  header httpheader;
  /* copy the header value */

  tmpvalue.assign(buffer, iterator);
  boost::trim(tmpvalue);
  httpheader.name = tmpvalue;
  /* now get the mime header value */
  iterator2 = iterator + 1;
  while (iterator2 < buffer_size && buffer[iterator2] && buffer[iterator2] != '\n')
    iterator2++;
  if (buffer[iterator2] != '\n') {
    LOG(WARNING) << __func__ <<"| failed, not find line:" << buffer << "len:"
      << buffer_size;
    return false;
  }
  /* copy the value */
  tmpvalue.clear();
  tmpvalue.assign(buffer+ iterator + 1, iterator2-iterator);
  boost::trim(tmpvalue);

  httpheader.value = tmpvalue;
  req.headers.push_back(httpheader);
  if (httpheader.name == "User-cmd") {
    VLOG(90) <<__func__<< " recv User-Cmd:" << tmpvalue;
    req.req_cmd = tmpvalue;
  }
  VLOG(150) << "Recv http head name:" << httpheader.name << " value:"
    <<httpheader.value;
  return true;
}
} // namespace server_base
} // namespace http
