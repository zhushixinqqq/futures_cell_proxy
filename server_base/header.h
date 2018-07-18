/*
 *  author liuxiaobing@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 *
 *  */

#ifndef FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_HEADER_H_
#define FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_HEADER_H_

#include <string>

namespace http {
namespace server_base {

struct header
{
  std::string name;
  std::string value;
};

} // namespace server_base
} // namespace http

#endif // FORMAX_SOCIAL_CELL_PROXY_SERVER_SERVER_BASE_HEADER_H_
