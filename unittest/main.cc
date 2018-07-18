// Copyright (c) 2017, Formax Inc. All rights reserved.

#include "thirdparty/gflags/gflags.h"
#include "thirdparty/glog/logging.h"
#include "thirdparty/gtest/gtest.h"

int main(int argc, char **argv) {
  FLAGS_logbuflevel = -1;
  FLAGS_logtostderr = true;
  testing::InitGoogleTest(&argc, argv);

  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  return RUN_ALL_TESTS();
}
