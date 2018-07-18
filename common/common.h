// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#ifndef FUTURES_CELL_PROXY_COMMON_COMMON_H
#define FUTURES_CELL_PROXY_COMMON_COMMON_H

#include <sstream>
#include <chrono>
#include <string>

#include "thirdparty/cryptopp/include/aes.h"
#include "thirdparty/cryptopp/include/base64.h"
#include "thirdparty/cryptopp/include/modes.h"
#include "thirdparty/gflags/gflags.h"

#include "futures_cell_proxy/proto/futures_service.pb.h"
#include "protocols/common_types.h"

using namespace futures::pb::protocols;
using namespace cell_proxy::protocols;

namespace futures {namespace pb {namespace server {
namespace common {

template <class T>
class Singleton {
  private:
    Singleton(const Singleton& src){};
    Singleton& operator=(const Singleton& src){};
  protected:
    Singleton(){};
    ~Singleton(){};
  public:
    static T* getInstance() {
      static T instance;
      return &instance;
    }
};

typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> nanoClock_type;

void FillStatusInfo(StatusInfo* status_info,
    int32_t status_no, const std::string& message = "");

void GenerateUniqueID(const std::string& access_name,
    const futures_trade::protocols::AccessType::type& access_type,
    futures_trade::protocols::AccessInfo* access_info);

int curlStroreRst(char *data, size_t size, size_t nmemb, std::string *result);

int32_t fnDoCurl(const std::string &strURL, std::string &strRst);

void FillLableValue(LableValue* label_value, const std::string& lable, const std::string& value);

std::string DoubleToString(const double& value, int32_t digits/* = 2 */);

std::string AESEncrypt(const std::string& str_in);

} // namespace common
} // namespace server
} // namespace pb
} // namespace futures
#endif // FUTURES_CELL_PROXY_COMMON_COMMON_H
