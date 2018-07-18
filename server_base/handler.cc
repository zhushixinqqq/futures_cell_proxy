/*
 *  author liuxiaobing@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 *
 *  */

#include <boost/lexical_cast.hpp>
#include <glog/logging.h>
#include <fstream>
#include <sstream>
#include <string>

#include "futures_cell_proxy/server_base/handler.h"
#include "futures_cell_proxy/server_base/reply.h"
#include "futures_cell_proxy/server_base/request.h"
#include "thirdparty/service_monitor/sender/info_collector.h"


using formax_social::service_monitor::InfoCollector;

namespace http { namespace server_base {

Handler::Handler(const std::string& doc_root) : m_doc_root(doc_root) {
}

Handler::~Handler() {
}

void Handler::ProccessReqFromHandle(ConnectionPtr conn,
    const string& req_path, const request& req, reply *rep) {
  HandleMap::iterator handle_iter = m_handle_map.find(req_path);
  HandleMap::iterator handle_iter1;
  if (handle_iter == m_handle_map.end()) {
    LOG(ERROR) << "no handle register for request: " << req_path;
    if (!req.req_cmd.empty() &&
        (handle_iter1 = m_handle_map.find(req.req_cmd)) != m_handle_map.end()) {
      handle_iter = handle_iter1;
      LOG(INFO) <<__func__<< " find cmd from http parameter:" << req.req_cmd <<
        " uri:" << req_path;
    } else {
      rep->status = reply::not_found;
      return;
    }
  }

  std::string out_data;
  {
    std::string  sfunc = "CellProxyService::" + req_path;
    InfoCollector infoc(sfunc.c_str());
    infoc.SetRemoteHost(conn->Getendpoint().address().to_string());
    infoc.SetRemotePort(conn->Getendpoint().port());
    (handle_iter->second)(req.content, &out_data);
  }
  VLOG(50) << "func:" << handle_iter->first << " result data return from handle size: "
    << out_data.length() << "client:" << conn->Getendpoint();
  // 通过注册的函数处理了
  if ("echo" == handle_iter->first ||
      "/echo" == handle_iter->first) {
    LOG(INFO) <<__func__<< " recv echo cmd req:" << req.content;
    BuidlReply(req.http_req_data, rep);
  } else {
    BuidlReply(out_data, rep);
  }
}

void Handler::BuidlReply(const string& reply_msg, reply* rep) {
  rep->status = reply::ok;
  rep->content = reply_msg;
  rep->headers.resize(2);
  rep->headers[0].name = "Content-Length";
  rep->headers[0].value = boost::lexical_cast<std::string>(rep->content.size());
  rep->headers[1].name = "Content-Type";
  rep->headers[1].value = "text/plain";
  VLOG(50) << "reply data: " << rep->content.length();
}

void Handler::HandleRequest(ConnectionPtr conn, const request& req, reply& rep) {
  // Decode url to path.
  std::string request_path;
  LOG(INFO) << "request: " << req.uri;
  if (!UrlDecode(req.uri, request_path)) {
    rep = reply::stock_reply(reply::bad_request);
    return;
  }
  ProccessReqFromHandle(conn, request_path, req, &rep);
}

bool Handler::UrlDecode(const std::string& in, std::string& out)
{
  out.clear();
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

} // namespace server_base
} // namespace http
