[FUTURES CELL PROXY Framework]
============================

[![Build Status](https://travis-ci.org/gin-gonic/gin.svg)](#html)
 [![codecov](https://codecov.io/gh/gin-gonic/gin/branch/master/graph/badge.svg)](#html)

<img align="right" width="159px" high="140" src="http://static.jrq.com/uploads/image/oa/2017/11/15/5a0bee8b3cd23phpGgOYpe.png">
Futures Cell Proxy is http framework written in C++,but it is difference from the normal web(http) framework. You can feature a interface like API with better perference,thanks to the gin it provides these .svg file,you will love Futures cell proxy.
<br/><br/><br/><br/>
# Repository Providing & Status

futures cell proxy 项目源码目录.

| Language                | Source                              | Remark                         |
|------------------------:|------------------------------------:|-------------------------------:|
| C++                     | [src](src)                          | 主要源码实现                   |
| C++                     | [server_base](server_base)          | 封装的http实现库               |

# Futures Cell Porxy v1. Start using it

- [x] 在protobuf 协议添加服务接口.
- [x] 将接口注册到http监听(REGISTE_HANDLE(QueryMyFuturesAssets))
- [x] RequestHandler,实现相应的接口.
- [x] 调用futures_cell_proxy相应的接口,完成逻辑操作.
- [x] 在thrift协议中完成微服务接口实现，并在XXXPOOL和XXXWrapper中完成接口调用封装.

# Protocol
- [x] The [protobuf protocol](proto) specifies the abstract requirements for communication between
      clients and servers.
- [x] The [thrift protocols](../protocols)  specifies the abstract requirements for communication between
      clients and servers.

# API Examples

```CPP
REGISTE_HANDLE(QueryMyFuturesAssets);
QueryMyFuturesAssetsReturn QueryMyFuturesAssets(const QueryMyFuturesAssetsRequest& req);
ADD_HANDLE(QueryMyFuturesAssets, QueryMyFuturesAssetsRequest, QueryMyFuturesAssetsReturn);

QueryMyFuturesAssetsReturn RequestHandler::QueryMyFuturesAssets(
    const QueryMyFuturesAssetsRequest& req) {
  formax::common::TimeCost ts(__func__);
  VLOG(30) << __func__ << " <<=== req:" << req.ShortDebugString();
  QueryMyFuturesAssetsReturn _return;
  FuturesProxy::getInstance()->QueryMyFuturesAssets(req, &_return);
  VLOG(30) << __func__ << " <<=== resp:" << _return.ShortDebugString();
  return _return;
}

void FuturesProxy::QueryMyFuturesAssets(const QueryMyFuturesAssetsRequest& req,
    QueryMyFuturesAssetsReturn *resp) {
  .........;
  .........;
}

```
# Test Examples

单元测试代码所在的目录unittest.

| Language                | Source                              |
|-------------------------|-------------------------------------|
| C++                     | [unittest](unittest)                |

```CPP
TEST_F(APITest, QueryMyFuturesAssets_API) {
  QueryMyFuturesAssetsRequest req;
  FillLoginSession(req.mutable_login_session());
  FillTerminalInfo(req.mutable_terminal_info());
  LOG(INFO) << "QueryMyFuturesAssetsRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;
  QueryMyFuturesAssetsReturn resp;
  SecurityProxyClient::Instance()->QueryMyFuturesAssets(req, &resp);
  LOG(INFO) << "QueryMyFuturesAssetsReturn:" << resp.Utf8DebugString();
}
```

# Users  [![Sourcegraph](https://sourcegraph.com/github.com/gin-gonic/gin/-/badge.svg)](#html)

Awesome project lists using Futures Cell Proxy http framework.
