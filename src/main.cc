// Copyright [2013-2017] <xxxxx.Inc>
//
// Author: zhushixin

#include <iostream>
#include <string>

#include "boost/asio.hpp"
#include "boost/bind.hpp"
#include "glog/logging.h"

#include "futures_cell_proxy/server_base/server_base.h"
#include "futures_cell_proxy/src/config.h"
#include "futures_cell_proxy/src/request_handler.h"

int main(int argc, char* argv[]) {
  try {
    FLAGS_logtostderr = false;
    FLAGS_alsologtostderr = false;
    FLAGS_max_log_size = 200;
    google::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    LOG(INFO) << argv[0];

    http::server_base::HandlerPtr handler(new futures::pb::server::RequestHandler("."));
    // // Initialise the server.
    http::server_base::ServerBase s(FLAGS_listen_ip, FLAGS_listen_port, handler);

    std::cerr << "server start";
    // Run the server until stopped.
    s.run();
  } catch (const std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
  }
  return 0;
}
