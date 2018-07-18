// Copyright (c) 2017, Formax Inc. All rights reserved.

#include "futures_cell_proxy/unittest/common.h"

#include "thirdparty/curl/curl.h"
#include "thirdparty/glog/logging.h"

using namespace cell_proxy::protocols;
namespace futures { namespace pb { namespace protocols {
namespace common {
static size_t WriteData(void *ptr, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  std::string* str_ptr = (std::string*) userp;
  str_ptr->append((char*)ptr, realsize);
  return realsize;
}

bool HttpGet(const std::string& server_ip, int32_t server_port,
  int32_t timeout_s, const std::string& func_name,
  const std::string& http_req_body, const std::string* http_resp_body) {
  try {
    // 组包
    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, 1.1);

    std::stringstream url;
    url << server_ip << ":" << server_port;
    url << "/" << func_name;
    curl_easy_setopt(curl, CURLOPT_URL, url.str().c_str());

    struct curl_slist *chunk = NULL;
    std::stringstream host;
    host << "Host: " << server_ip << ":" << server_port;
    chunk = curl_slist_append(chunk, host.str().c_str());

    std::stringstream len;
    len << "Content-Length: " << http_req_body.size();
    chunk = curl_slist_append(chunk, len.str().c_str());

    std::stringstream cmd;
    cmd << "User-cmd: " << func_name;
    chunk = curl_slist_append(chunk, cmd.str().c_str());

    chunk = curl_slist_append(chunk, "Accept:");
    chunk = curl_slist_append(chunk, "Content-Type:");

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout_s);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)http_resp_body);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, http_req_body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, http_req_body.size());
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    // 发包
    CURLcode result;
    result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    // 结果
    if (CURLE_OK == result) {
      return true;
    } else {
      LOG(ERROR) << func_name << " Failed! Curl error code = " << result
          << " error_string = " << curl_easy_strerror(result)
          << " func_name:" << func_name;
      return false;
    }
  } catch (const std::exception& e) {
    LOG(ERROR) << func_name << "Failed. exception[" << e.what() << "]";
    return false;
  }
}
} // namespace common
} // namespace protocols
} // namespace pb
} // namespace futures
