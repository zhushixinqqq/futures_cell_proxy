// Copyright (c) 2017, Formax Inc. All rights reserved.

#include <string>

#include "futures_cell_proxy/proto/futures_service.pb.h"
#include "futures_cell_proxy/server_base/reply.h"
#include "futures_cell_proxy/server_base/request.h"
#include "thirdparty/glog/logging.h"
#include "thirdparty/gtest/gtest.h"

#include "futures_cell_proxy/unittest/config.h"
#include "futures_cell_proxy/unittest/push_proxy_client.h"
#include "futures_cell_proxy/unittest/security_proxy_client.h"


using namespace futures::pb::protocols;
using namespace security::pb::protocols;
using namespace cell_proxy::protocols;

class APITest : public testing::Test {
protected:
  void SetUp() {
  }

  void TearDown() {
  }
protected:
  void FillLoginSession(LoginSession* login_session) {
    login_session->set_uid(FLAGS_uid);
    // required 字段没有会导致core
    // login_session->set_session_str("futures_cell_proxy");
    LoginInfoProto login_info;
    login_info.set_phonenumber_countrycode("+86");
    login_info.set_phonenumber(FLAGS_phone_number);
    login_info.set_session_expire(3000);
    login_info.set_need_detail(true);
    login_info.set_version(131072);
    login_info.set_login_way(2);
    login_info.set_md5_pass(AESEncrypt(FLAGS_password));
    login_info.set_mt4_id(0);
    PushProxyClient::Instance()->Login(login_info, &m_login_resp);
    login_session->CopyFrom(m_login_resp.login_session());
  }

  std::string AESEncrypt(const std::string& str_in,
      const std::string& key = "FormaxmarketInternetBusinessDept", const std::string& iv = "0123456789123456") {
    std::string str_out;
    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
    CryptoPP::StringSource encryptor(str_in, true, new CryptoPP::StreamTransformationFilter(encryption,
          new CryptoPP::Base64Encoder(new CryptoPP::StringSink(str_out),false) // do not append a newline
          ));
    return str_out;
  }

  void FillTerminalInfo(TerminalInfo* terminal_info) {
    CHECK_NOTNULL(terminal_info);
    terminal_info->set_version(4321);
    terminal_info->set_device_type(IOS);
    terminal_info->set_lang("ch");
    terminal_info->set_app_type(Security);
    terminal_info->set_debug(true);
  }
  bool CheckLogin() {
    return true;
  }
public:
  LoginReturn m_login_resp;
};

TEST_F(APITest, QueryMyFuturesAssets_API) {
  QueryMyFuturesAssetsRequest req;
  req.set_futures_id(FLAGS_futures_id);
  FillLoginSession(req.mutable_login_session());
  FillTerminalInfo(req.mutable_terminal_info());
  LOG(INFO) << "QueryMyFuturesAssetsRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;
  QueryMyFuturesAssetsReturn resp;
  SecurityProxyClient::Instance()->QueryMyFuturesAssets(req, &resp);

  LOG(INFO) << "QueryMyFuturesAssetsReturn:" << resp.Utf8DebugString();
}

// 下单交易
TEST_F(APITest, FuturesTrade_API) {
  FuturesTradeRequest req;
  req.set_futures_id(FLAGS_futures_id);
  req.set_type(FLAGS_tradeType);
  req.set_symbol(FLAGS_symbol);
  req.set_volume(std::to_string(FLAGS_volume));
  req.set_price(std::to_string(FLAGS_price));

  FillLoginSession(req.mutable_session());
  FillTerminalInfo(req.mutable_terminal_info());
  LOG(INFO) << "FuturesTradeRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  FuturesTradeReturn resp;
  SecurityProxyClient::Instance()->FuturesTrade(req, &resp);

  LOG(INFO) << "FuturesTradeReturn:" << resp.Utf8DebugString();
}

// 开户状态
TEST_F(APITest,QueryUserStatus_API ) {
  QueryUserStatusRequest req;
  req.set_futures_id(FLAGS_futures_id);
  FillLoginSession(req.mutable_login_session());
  FillTerminalInfo(req.mutable_terminal_info());
  LOG(INFO) << "QueryUserStatusRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  QueryUserStatusReturn resp;
  SecurityProxyClient::Instance()->QueryUserStatus(req, &resp);

  LOG(INFO) << "QueryUserStatusReturn:" << resp.Utf8DebugString();
}

// 获取委托订单
TEST_F(APITest,QueryPendingOrders_API ) {
  QueryPendingOrdersRequest req;
  req.set_futures_id(FLAGS_futures_id);
  FillLoginSession(req.mutable_login_session());
  FillTerminalInfo(req.mutable_terminal_info());
  req.set_start_index(FLAGS_start_index);
  req.set_batch_num(FLAGS_batch_num);
  LOG(INFO) << "QueryPendingOrdersRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  QueryPendingOrdersReturn resp;
  SecurityProxyClient::Instance()->QueryPendingOrders(req, &resp);

  LOG(INFO) << "QueryPendingOrdersReturn:" << resp.Utf8DebugString();
}

// 获取持仓中订单
TEST_F(APITest,QueryHoldingOrders_API) {
  QueryHoldingOrdersRequest req;
  req.set_futures_id(FLAGS_futures_id);
  FillLoginSession(req.mutable_login_session());
  FillTerminalInfo(req.mutable_terminal_info());
  req.set_start_index(FLAGS_start_index);
  req.set_batch_num(FLAGS_batch_num);
  LOG(INFO) << "QueryHoldingOrdersRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  QueryHoldingOrdersReturn resp;
  SecurityProxyClient::Instance()->QueryHoldingOrders(req, &resp);

  LOG(INFO) << "QueryHoldingOrdersReturn:" << resp.Utf8DebugString();
}

// 获取订单历史
TEST_F(APITest,QueryHistoryOrders_API ) {
  QueryHistoryOrderRequest req;
  req.set_futures_id(FLAGS_futures_id);
  FillLoginSession(req.mutable_login_session());
  FillTerminalInfo(req.mutable_terminal_info());
  LOG(INFO) << "QueryHistoryOrderRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  QueryHistoryOrderReturn resp;
  SecurityProxyClient::Instance()->QueryHistoryOrders(req, &resp);

  LOG(INFO) << "QueryHistoryOrderReturn:" << resp.Utf8DebugString();
}

// 订单详情
TEST_F(APITest,QueryOrderDetail_API ) {
  QueryOrderDetailRequest req;
  req.set_futures_id(FLAGS_futures_id);
  FillLoginSession(req.mutable_login_session());
  FillTerminalInfo(req.mutable_terminal_info());
  LOG(INFO) << "QueryOrderDetailRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  QueryOrderDetailReturn resp;
  SecurityProxyClient::Instance()->QueryOrderDetail(req, &resp);

  LOG(INFO) << "QueryOrderDetailReturn:" << resp.Utf8DebugString();
}

// 资金流水
TEST_F(APITest,QueryTransactionRecord_API ) {
  QueryTransactionRecordRequest req;
  req.set_futures_id(FLAGS_futures_id);
  FillLoginSession(req.mutable_login_session());
  FillTerminalInfo(req.mutable_terminal_info());
  LOG(INFO) << "QueryTransactionRecordRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  QueryTransactionRecordReturn resp;
  SecurityProxyClient::Instance()->QueryTransactionRecord(req, &resp);

  LOG(INFO) << "QueryTransactionRecordReturn:" << resp.Utf8DebugString();
}

// 账户中心配置项
TEST_F(APITest,QueryFuturesAccountConfig_API ) {
  QueryFuturesAcconutConfigRequest req;
  req.set_futures_id(FLAGS_futures_id);
  FillLoginSession(req.mutable_login_session());
  FillTerminalInfo(req.mutable_terminal_info());
  LOG(INFO) << "QueryFuturesAcconutConfigRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  QueryFuturesAccountConfigReturn resp;
  SecurityProxyClient::Instance()->QueryFuturesAccountConfig(req, &resp);

  LOG(INFO) << "QueryFuturesAccountConfigReturn:" << resp.Utf8DebugString();
}

// 账户中心配置项
TEST_F(APITest,CancelPendingOrder) {
  CancelPendingOrderRequest req;
  req.set_futures_id(FLAGS_futures_id);
  req.set_order_id(std::to_string(FLAGS_order_id));
  FillLoginSession(req.mutable_session());
  LOG(INFO) << "CancelPendingOrderRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  FututesCancelPendingOrderReturn resp;
  SecurityProxyClient::Instance()->CancelPendingOrder(req, &resp);

  LOG(INFO) << "CancelPendingOrderReturn:" << resp.Utf8DebugString();
}

//  按关键字搜索
TEST_F(APITest, QuerySymbolByName) {
  QuerySymbolByNameRequest req;
  req.set_key(FLAGS_search_key);
  LOG(INFO) << "QuerySymbolByNameRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  QuerySymbolByNameReturn resp;
  SecurityProxyClient::Instance()->QuerySymbolByName(req, &resp);

  LOG(INFO) << "QuerySymbolByNameReturn:" << resp.Utf8DebugString();
}

// 交易确认
TEST_F(APITest, TradeConfirmInfo) {
  TradeConfirmInfoRequest req;
  FillLoginSession(req.mutable_session());
  FillTerminalInfo(req.mutable_terminal_info());
  req.set_futures_id(FLAGS_futures_id);
  req.set_trade_type(FLAGS_tradeType);
  req.set_symbol(FLAGS_symbol);
  req.set_price(FLAGS_price);
  req.set_volume(FLAGS_volume);

  LOG(INFO) << "ConfirmInfoRequestreq:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  TradeConfirmInfoReturn resp;
  SecurityProxyClient::Instance()->TradeConfirmInfo(req, &resp);

  LOG(INFO) << "TradeConfirmInfoReturn:" << resp.Utf8DebugString();
}

TEST_F(APITest, QuerySymbolTradeInfo) {
  QuerySymbolTradeInfoRequest req;
  FillLoginSession(req.mutable_session());
  FillTerminalInfo(req.mutable_terminal_info());
  req.set_futures_id(FLAGS_futures_id);
  req.set_symbol(FLAGS_symbol);

  LOG(INFO) << "QuerySymbolTradeInfoRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  QuerySymbolTradeInfoReturn resp;
  SecurityProxyClient::Instance()->QuerySymbolTradeInfo(req, &resp);

  LOG(INFO) << "QuerySymbolTradeInfoReturn:" << resp.Utf8DebugString();
}

// 买卖五档
TEST_F(APITest, QueryFifthOrder) {
  QueryFifthOrderRequest req;
  FillLoginSession(req.mutable_login_session());
  FillTerminalInfo(req.mutable_terminal_info());
  req.set_futures_id(FLAGS_futures_id);
  req.set_symbol(FLAGS_symbol);

  LOG(INFO) << "QueryFifthOrderRequest:" << req.Utf8DebugString();
  if (!ContinueRun()) return;

  QueryFifthOrderReturn resp;
  SecurityProxyClient::Instance()->QueryFifthOrder(req, &resp);

  LOG(INFO) << "QueryFifthOrderReturn:" << resp.Utf8DebugString();
}
// selftest 1/2 add api interface
