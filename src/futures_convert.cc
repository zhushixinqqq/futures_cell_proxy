// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin
#include "futures_cell_proxy/src/futures_convert.h"
#include "thirdparty/gflags/gflags.h"


namespace futures { namespace pb { namespace server {

void FuturesConvert::ToProtoSymbolByName(const Json::Value& root, QuerySymbolByNameReturn* resp) {
  for (unsigned int i =0 ; i < static_cast<unsigned int>(root.size()); ++i) {
    FuturesSymbolItem* futureItem = resp->add_symbols();
    futureItem->set_symbol(root[i]["futureId"].asString());
    futureItem->set_name(root[i]["futureName"].asString());
  }
}

void FuturesConvert::ToProtoMyFuturesAsset(const futures_trade::protocols::GetAccInfoResp& acc_info_resp,
    QueryMyFuturesAssetsReturn* resp) {
  resp->set_total_net_worth(DoubleToString(acc_info_resp.accInfo.NAV,2));
  resp->set_real_buy_pwr(DoubleToString(acc_info_resp.accInfo.BuyingPower,2));
  resp->set_basic_deposit(DoubleToString(acc_info_resp.accInfo.IMargin,2));
  resp->set_pre_deposit(DoubleToString(acc_info_resp.accInfo.MMargin,2));
  // TODO
  std::string profit_today;
  char buffer[128];
  // 持仓盈亏是否正常 1 正常 0 不正常
  if (acc_info_resp.accInfo.TodayProfitFlag == 1 ) {
    profit_today = acc_info_resp.accInfo.TodayProfit;
    // snprintf(buffer, sizeof(buffer), "%.2s", acc_info_resp.accInfo.TodayProfit);
  } else {
    profit_today="--";
  }
  resp->set_profit_today(profit_today);
}

void FuturesConvert::ToProtoOrderDetail(const futures_trade::protocols::GetOrderByNoResp& order_detail_resp,
    QueryOrderDetailReturn* resp) {
  if (order_detail_resp.__isset.orderInfo) {
    FututesOrderInfo* order_info= resp->mutable_order_detail();
    order_info->set_order_id(order_detail_resp.orderInfo.IntOrderNo);
    if (order_detail_resp.orderInfo.BuySell == 'B') {
      order_info->set_trade_cmd(1);
      std::string order_status;
      if(order_detail_resp.orderInfo.Status == futures_trade::protocols::OrderStatus::ORDSTAT_TRADED) {
        order_status = "已成交";
      } else if (order_detail_resp.orderInfo.Status == futures_trade::protocols::OrderStatus::ORDSTAT_PARTTRD_WRK) {
        order_status = "部分已成交";
      } else {
        order_status = "未成交";
      }
      order_info->set_state(order_status);
    } else {
      order_info->set_trade_cmd(2);
    }
    order_info->set_trade_time(order_detail_resp.orderInfo.ValidTime);
    FillLableValue(order_info->add_atts(), "委托数量", std::to_string(order_detail_resp.orderInfo.TotalQty));
    FillLableValue(order_info->add_atts(), "成交数量", std::to_string(order_detail_resp.orderInfo.TradedQty));
    FillLableValue(order_info->add_atts(), "委托价格", std::to_string(order_detail_resp.orderInfo.EntrustPrice)); // TODO
    FillLableValue(order_info->add_atts(), "成交价格", std::to_string(order_detail_resp.orderInfo.Price));
    FillLableValue(order_info->add_atts(), "委托时间", std::to_string(order_detail_resp.orderInfo.TimeStamp));
    FillLableValue(order_info->add_atts(), "成交时间", std::to_string(order_detail_resp.orderInfo.ValidTime));
    FillLableValue(order_info->add_atts(), "佣金以及市场费用", std::to_string(order_detail_resp.orderInfo.Fees));
    if (order_detail_resp.orderInfo.BuySell == 'B') {
      FillLableValue(order_info->add_atts(), "持仓盈亏", std::to_string(order_detail_resp.orderInfo.Profit));
      FillLableValue(order_info->add_atts(), "保证金", std::to_string(order_detail_resp.orderInfo.Deposit)); // TODO
    } else {
      FillLableValue(order_info->add_atts(), "卖出盈亏", std::to_string(order_detail_resp.orderInfo.Profit));
    }
  }
}

void FuturesConvert::ToProtoActiveOrders(const futures_trade::protocols::GetActiveOrderResp& order_detail_resp,
    QueryPendingOrdersReturn* resp) {
  for (auto& item : order_detail_resp.ActiveOrders) {
    FututesOrderInfo* order_info= resp->add_orders();
    order_info->set_order_id(item.IntOrderNo);
    FuturesSymbolItem* symbol_item=order_info->mutable_symbol_item();

    symbol_item->set_symbol(item.ProdCode);
    if (item.BuySell == 'B') {
      order_info->set_trade_cmd(1);
    } else {
      order_info->set_trade_cmd(2);
    }
    order_info->set_trade_time(item.ValidTime);
    FillLableValue(order_info->add_atts(), "委托价", std::to_string(item.Price));
    FillLableValue(order_info->add_atts(), "手数", std::to_string(item.Qty));
    FillLableValue(order_info->add_atts(), "现价", std::to_string(item.CurrentPrice));
  }
  if (order_detail_resp.has_more == true) {
    resp->set_has_more(true);
  } else {
    resp->set_has_more(false);
  }
}

void FuturesConvert::ToProtoPosition(const futures_trade::protocols::GetPositionResp& position_resp,
    QueryHoldingOrdersReturn* resp){
  for (auto& item : position_resp.PositionList) {
    FututesOrderInfo* order_info= resp->add_orders();
    //TODO
    order_info->set_order_id(0);
    FuturesSymbolItem* symbol_item=order_info->mutable_symbol_item();
    symbol_item->set_symbol(item.ProdCode);
    if (item.LongShort == 'B') {
      order_info->set_trade_cmd(1);
    } else {
      order_info->set_trade_cmd(2);
    }
    order_info->set_trade_time(0);
    // LableValue* label_value=order_info->add_atts();
    double avg_price=0.00;
    if (item.Qty+item.LongQty-item.ShortQty > 0.00001 || item.Qty+item.LongQty-item.ShortQty < -0.00001) {
      avg_price=(item.DepTotalAmt+item.LongTotalAmt-item.ShortTotalAmt)/(item.Qty+item.LongQty-item.ShortQty);
    }
    FillLableValue(order_info->add_atts(), "平均价", DoubleToString(avg_price,1));
    FillLableValue(order_info->add_atts(), "净仓", DoubleToString(item.Qty+item.LongQty-item.ShortQty,1));
    FillLableValue(order_info->add_atts(), "市价", DoubleToString(item.MarketPrice,1));
    FillLableValue(order_info->add_atts(), "盈亏", DoubleToString(item.PL,1));
  }
  if (position_resp.has_more == true) {
    resp->set_has_more(true);
  } else {
    resp->set_has_more(false);
  }
}

void FuturesConvert::ToProtoHistoryOrders(const futures_trade::protocols::GetHisOrderResp& his_order_resp,
    QueryHistoryOrderReturn* resp) {
  for (auto& item : his_order_resp.orders) {
    FututesOrderInfo* order_info = resp->add_orders();
    order_info->set_order_id(item.IntOrderNo);
    FuturesSymbolItem* symbol_item=order_info->mutable_symbol_item();
    symbol_item->set_symbol(item.ProdCode);
    if (item.BuySell == 'B') {
      order_info->set_trade_cmd(1);
      std::string order_status;
      if(item.Status == futures_trade::protocols::OrderStatus::ORDSTAT_TRADED) {
        order_status = "已成交";
      } else if (item.Status == futures_trade::protocols::OrderStatus::ORDSTAT_PARTTRD_WRK) {
        order_status = "部分已成交";
      } else {
        order_status = "未成交";
      }
      order_info->set_state(order_status);
    } else {
      order_info->set_trade_cmd(2);
    }
    order_info->set_trade_time(item.ValidTime);
  }
}

// 期货配置列表
void FuturesConvert::ToProtoFuturesAccountConfig(const QueryFuturesAcconutConfigRequest& req,
    QueryFuturesAccountConfigReturn * resp) {
  // type:0 表示账户中心配置项;1表示更多(全部)配置项
  if (req.type() == 0 ) {
    FuturesAccountConfigInfoList* config_infos=resp->add_config_info_plate_list();
    // FuturesSubAccountConfigInfoList* sub_config_infos=config_infos->add_config_info_list();

    FuturesSubAccountConfigInfoList* buy_info =config_infos->add_config_info_list();
    buy_info->set_caption_des("买入");
    buy_info->set_scheme("formaxsecurity://futuresTrade");//TODO
    buy_info->set_item_icon(FLAGS_buy_icon_url);//TODO

    FuturesSubAccountConfigInfoList* sell_info = config_infos->add_config_info_list();
    sell_info->set_caption_des("卖出");
    sell_info->set_scheme("formaxsecurity://futuresTrade");//TODO
    sell_info->set_item_icon(FLAGS_sell_icon_url);//TODO

    FuturesSubAccountConfigInfoList* fund_in = config_infos->add_config_info_list();
    fund_in->set_caption_des("入金");
    fund_in->set_scheme(FLAGS_futures_fund_in_url);//TODO
    fund_in->set_item_icon(FLAGS_fund_in_icon_url);//TODO

    FuturesSubAccountConfigInfoList* fund_out = config_infos->add_config_info_list();
    fund_out->set_caption_des("出金");
    fund_out->set_scheme(FLAGS_futures_fund_out_url);//TODO
    fund_out->set_item_icon(FLAGS_fund_out_icon_url);//TODO

    // AccountConfigInfo* more_info = resp->add_config_info_list();
    // more_info->set_caption_des("更多");
    // more_info->set_scheme("TODO");//TODO
    // more_info->set_item_icon("TODO");//TODO
  } else {
    FuturesAccountConfigInfoList* config_infos=resp->add_config_info_plate_list();

    FuturesSubAccountConfigInfoList* fund_in = config_infos->add_config_info_list();
    fund_in->set_caption_des("入金");
    fund_in->set_scheme(FLAGS_futures_fund_in_url);//TODO
    fund_in->set_item_icon(FLAGS_fund_in_icon_url);//TODO

    FuturesSubAccountConfigInfoList* fund_out = config_infos->add_config_info_list();
    // AccountConfigInfo* fund_out = fund_out_node->add_config_info_list();
    fund_in->set_caption_des("出金");
    fund_in->set_scheme(FLAGS_futures_fund_out_url);//TODO
    fund_in->set_item_icon(FLAGS_fund_out_icon_url);//TODO

    FuturesSubAccountConfigInfoList* change_passwd = config_infos->add_config_info_list();
    // AccountConfigInfo* change_passwd = change_passwd_node->add_config_info_list();
    change_passwd->set_caption_des("重置期货交易密码");
    change_passwd->set_scheme("www.baidu.com");//TODO
    change_passwd->set_item_icon("TODO");//TODO
  }
}

// 期货出入金明细
void FuturesConvert::ToProtoFundRecord(const futures_trade::protocols::GetFundInOutRecordResp& fund_record_resp,
    QueryTransactionRecordReturn * resp) {
  for (auto& item : fund_record_resp.fund_in_out_list) {
    TransactionRecord* trans_record = resp->add_records();
    if (item.description == 1) {
      trans_record->set_description("出金");
    } else {
      trans_record->set_description("入金");
    }
    trans_record->set_money(item.money);
    trans_record->set_create_time(item.create_time);
  }
  if (fund_record_resp.has_more == true) {
    resp->set_has_more(true);
  } else {
    resp->set_has_more(false);
  }
}

void FuturesConvert::ToProtoQuoteFeeInfo(const futures_trade::protocols::GetFuturesQuoteFeeInfoResp& fee_info_resp,
    TradeConfirmInfoReturn * resp) {
  double commission = fee_info_resp.quote_fee_info.trade_cost + fee_info_resp.quote_fee_info.exchange_cost +
    fee_info_resp.quote_fee_info.securities_cost;
  resp->set_commission(std::to_string(commission));
  resp->set_security_deposit(std::to_string(fee_info_resp.quote_fee_info.IMargin)); // TODO
}

void FuturesConvert::ToProtoQuerySymbolTradeInfo(const futures_trade::protocols::GetFuturesQuoteFeeInfoResp& fee_info_resp,
    QuerySymbolTradeInfoReturn * resp){
  double commission = fee_info_resp.quote_fee_info.trade_cost + fee_info_resp.quote_fee_info.exchange_cost +
    fee_info_resp.quote_fee_info.securities_cost;
  FuturesSymbolTradeInfo* symbol_trade_info=resp->mutable_symbol_info();
  symbol_trade_info->set_lots_step(1);
  symbol_trade_info->set_price_digits(4);//TODO
  symbol_trade_info->set_margin_peer_lot(fee_info_resp.quote_fee_info.IMargin);
  symbol_trade_info->set_trade_fee(commission);

}
} // namepsace server
} // namespace pb
} // namespace futures
