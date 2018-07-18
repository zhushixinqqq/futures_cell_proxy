// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#ifndef FUTURES_CELL_PROXY_COMMON_DEFINE_H
#define FUTURES_CELL_PROXY_COMMON_DEFINE_H

#include <string>

#include "futures_cell_proxy/proto/cell_proxy_common.pb.h"
#include "futures_cell_proxy/proto/futures_service.pb.h"


using cell_proxy::protocols::StatusInfo;
using cell_proxy::protocols::ErrInfo;
using namespace futures::pb::protocols;

/////////////////////////////// 宏定义开始 ////////////////////////////////
#define ADD_HANDLE(fun_name, in_type, out_type) \
  inline void base##fun_name(const std::string& in_data, std::string* out) { \
    in_type in; \
    in.ParseFromString(in_data);\
    out_type out_proto; \
    out_proto = fun_name(in); \
    out_proto.SerializeToString(out); \
  }
// define ADD_HANDLE END

#define REGISTE_HANDLE(fun_name) \
  LOG(INFO) << "add handle name: " << #fun_name; \
  m_handle_map[#fun_name] \
    = boost::bind(&RequestHandler::base##fun_name, this, _1, _2); \
  m_handle_map["/"#fun_name] \
    = boost::bind(&RequestHandler::base##fun_name, this, _1, _2);
// define REGISTE_HANDLE END


#define QPS_RETURN(res_msg, ret_class)  \
  ret_class ret; \
  auto refl = ret.GetReflection();  \
  auto des = ret.descriptor();  \
  \
  if (auto fdes = des->FindFieldByName("status_info")) {  \
    StatusInfo* status_info \
      = static_cast<StatusInfo*>(refl->MutableMessage(&ret, fdes));  \
    status_info->set_status_no(-1);  \
    status_info->set_message(res_msg);  \
  \
    ret.SerializeToString(out); \
    return; \
  } else if (auto fdes = des->FindFieldByName("err_info")) {  \
    ErrInfo* err_info \
      = static_cast<ErrInfo*>(refl->MutableMessage(&ret, fdes));  \
    err_info->set_err_no(cell_proxy::protocols::FAILED);  \
    err_info->set_err_str(res_msg); \
  \
    ret.SerializeToString(out); \
    return; \
  } else {  \
    LOG(ERROR) << "[" << __func__ << "] can not find status_info or err_info " \
      << "in response class[" << #ret_class << "]."; \
  }
// define QPS_RETURN END
//
//////////////////////////////// 宏定义结束 //////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 重载流输出运算符，可以直接输出pb协议的结构体到流
//////////////////////////////////////////////////////////////////////////
inline std::ostream& operator << (std::ostream& o, const
  ::google::protobuf::Message& msg) {
  std::string s = msg.Utf8DebugString();
  size_t pos = s.find_first_of('\n');
  for ( ; pos != std::string::npos; ) {
    s.at(pos) = ' ';
    pos = s.find_first_of('\n', pos);
  }
  return o << s;
}
//////////////////////////////////////////////////////////////////////////

#endif  // FUTURES_CELL_PROXY_COMMON_DEFINE_H
