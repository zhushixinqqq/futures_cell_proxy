// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin
#include <string>
#include <iostream>
#include "futures_cell_proxy/common/common.h"
#include "futures_cell_proxy/proto/futures_service.pb.h"
#include "protocols/common_types.h"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "thirdparty/boost/lexical_cast.hpp"
#include "thirdparty/curl/curl.h"

namespace futures {namespace pb {namespace server {
namespace common {
  void FillStatusInfo(StatusInfo* status_info,
      int32_t status_no, const std::string& message/* = "" */) {
    status_info->set_status_no(status_no);
    status_info->set_message(message);
  }
  // 生成唯一iD 精确到纳妙
  void GenerateUniqueID(const std::string& access_name,
      const futures_trade::protocols::AccessType::type& access_type,
      futures_trade::protocols::AccessInfo* access_info) {
    nanoClock_type tp = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now());
    std::ostringstream oss;
    oss << tp.time_since_epoch().count();
    access_info->__set_request_id(oss.str());
    access_info->__set_access_name(access_name);
    access_info->__set_access_type(access_type);
  }
  int curlStroreRst(char *data, size_t size, size_t nmemb, std::string *result) {
    for(unsigned int i=0; i<size*nmemb; i++)
      result->push_back(*(data++));
    return size * nmemb;
  }

  // http调用
  int32_t fnDoCurl(const std::string &strURL,std::string &strRst) {
    CURL *curl = NULL;
    try {
      curl = curl_easy_init();
      if (curl == NULL) {
        LOG(ERROR) << "curl_easy_init() failed";
        return CURLE_FAILED_INIT;
      }
      CURLcode curlRetCode;
      curlRetCode = curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
      curlRetCode = curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 3000); // ms
      curlRetCode = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlStroreRst);
      curlRetCode = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &strRst);
      curlRetCode = curl_easy_setopt(curl, CURLOPT_IPRESOLVE,CURL_IPRESOLVE_V4); //  only resolve ipv4 address
      curlRetCode = curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, 60*60*24); //  cache DNS for 1 day
      curlRetCode = curl_easy_setopt(curl, CURLOPT_DNS_USE_GLOBAL_CACHE, 1); // share cached DNS across sessions
      curlRetCode = curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L); //modify the mulit_phtread clash cause by timeout

      // char szErrBuf[1024] = {0};
      // curl_easy_setopt ( curl, CURLOPT_ERRORBUFFER, szErrBuf);
      // CURLcode res = curl_easy_perform(curl);

      int try_nums = 2;
      CURLcode res;
      while (try_nums-- > 0) {
        char errbuf[1024];
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        if (CURLE_OK == (res = curl_easy_perform(curl))) {
          break;
        }
      }
      if (try_nums < 0) {
        LOG(ERROR) << "call http server failed,errno : " << res;
      }
      //...........begin 打印超时日志...................................................
      CURLcode return_code;
      double namelookuptime = 0;
      // DNS解析时间,从请求开始到DNS解析完毕所用时间
      return_code = curl_easy_getinfo(curl, CURLINFO_NAMELOOKUP_TIME, &namelookuptime);
      if((CURLE_OK == return_code) && namelookuptime) {
        VLOG(100)  << "dns resolve time cost:" << namelookuptime;
      }
      // TCP 建立连接的耗时
      double connect_time = 0;
      return_code = curl_easy_getinfo(curl, CURLINFO_CONNECT_TIME, &connect_time);
      if (CURLE_OK == return_code) {
        VLOG(100) << "connect cost time " << connect_time;
      }
      // 从开始计算,到应用层（SSL，在TCP之上的应用层）连接/握手完成的耗时
      double appconnect_time = 0;
      return_code = curl_easy_getinfo(curl, CURLINFO_APPCONNECT_TIME, &appconnect_time);
      if (CURLE_OK == return_code) {
        VLOG(100) << "appconnect cost time " << appconnect_time;
      }
      // 从开始到准备传输的时间
      double prestransfer_time = 0;
      return_code = curl_easy_getinfo(curl, CURLINFO_PRETRANSFER_TIME, &prestransfer_time);
      if (CURLE_OK == return_code) {
        VLOG(100) << "prestransfer cost time " << prestransfer_time;
      }
      // 开始传输时间,在client发出请求之后,服务器返回数据的第一个字节所用的时间
      double starttransfer_time = 0;
      return_code = curl_easy_getinfo(curl, CURLINFO_STARTTRANSFER_TIME, &starttransfer_time);
      if (CURLE_OK == return_code) {
        VLOG(100) << "starttransfer cost time " << starttransfer_time;
      }
      // 传输的总耗时(单位为秒),这个总的时间里包括了域名解析,以及TCP连接过程中所需要的时间
      double totaltime = 0;
      return_code = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &totaltime);
      if ((CURLE_OK == return_code) && totaltime) {
        VLOG(100) << "http cost total time " << totaltime;
      }
      //...........end 打印超时日志...................................................
      if (res != CURLE_OK) {
        LOG(ERROR) << "curl_easy_perform failed,errmsg:" << curl_easy_strerror(res);
        curl_easy_cleanup(curl);
        return res;
      }
      if (strRst.empty()) {
        int32_t nRspCode = 0;
        curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE, &nRspCode);
        if (nRspCode != 200)
        {
          LOG(ERROR) << "curl http response not 200,is:" << nRspCode;
          curl_easy_cleanup(curl);
          return res;
        }
      }
      if (curl !=NULL )
        curl_easy_cleanup(curl);
      return 0;
    } catch (const std::exception &e) {
      LOG(ERROR) << "curl catch exp:" << e.what();
      if (curl !=NULL )
        curl_easy_cleanup(curl);
      return -__LINE__;
    }
    return -__LINE__;
  }

  void FillLableValue(LableValue* label_value, const std::string& lable, const std::string& value) {
    label_value->set_label(lable);
    label_value->set_value(value);
  }

  std::string DoubleToString(const double& value, int32_t digits/* = 2 */) {
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "%.2f", value);
    return buffer;
  }

  std::string AESEncrypt(const std::string& str_in) {
    std::string key = "FormaxmarketInternetBusinessDept";
    std::string iv = "0123456789123456";
    std::string str_out;
    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
    CryptoPP::StringSource encryptor(str_in, true, new CryptoPP::StreamTransformationFilter(encryption,
          new CryptoPP::Base64Encoder(new CryptoPP::StringSink(str_out),false) // do not append a newline
          ));
    return str_out;
  }
} // namespace common
} // namespace server
} // namespace pb
} // namespace futures
