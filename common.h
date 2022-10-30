#include <stdio.h>
#include "rpc.h"

static const std::string kServerHostname = "node1";
static const std::string kClientHostname = "node0";

static constexpr uint16_t kUDPPort = 31850;
static constexpr uint8_t kInitRpc = 3;
static constexpr uint8_t kNextBatchRpc = 4;
static constexpr size_t kSmallMsgSize = 16;
static constexpr size_t kLargeMsgSize = 1024*1024;
