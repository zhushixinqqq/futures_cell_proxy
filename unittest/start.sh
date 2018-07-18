#!/bin/bash

#APITest.QueryMyFuturesAssets_API


./security_proxy_client \
--gtest_filter=*.QueryMyFuturesAssets_API \
--server_ip=127.0.0.1 \
--server_port=8857 \
--check_contiune_run=true \
