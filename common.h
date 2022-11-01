#include <iostream>

#include <arrow/array/array_base.h>
#include <arrow/array/array_nested.h>
#include <arrow/array/data.h>
#include <arrow/array/util.h>
#include <arrow/api.h>

#include "rpc.h"

static const std::string kServerHostname = "node1";
static const std::string kClientHostname = "node0";

static constexpr uint16_t kUDPPort = 31850;
static constexpr uint8_t kInitRpc = 3;
static constexpr uint8_t kNextBatchRpc = 4;
static constexpr size_t kSmallMsgSize = 1;
static constexpr size_t kLargeMsgSize = 1024*1024;

auto schema = arrow::schema({
    arrow::field("VendorID", arrow::int64()),
    arrow::field("tpep_pickup_datetime", arrow::timestamp(arrow::TimeUnit::MICRO)),
    arrow::field("tpep_dropoff_datetime", arrow::timestamp(arrow::TimeUnit::MICRO)),
    arrow::field("passenger_count", arrow::int64()),
    arrow::field("trip_distance", arrow::float64()),
    arrow::field("RatecodeID", arrow::int64()),
    arrow::field("store_and_fwd_flag", arrow::utf8()),
    arrow::field("PULocationID", arrow::int64()),
    arrow::field("DOLocationID", arrow::int64()),
    arrow::field("payment_type", arrow::int64()),
    arrow::field("fare_amount", arrow::float64()),
    arrow::field("extra", arrow::float64()),
    arrow::field("mta_tax", arrow::float64()),
    arrow::field("tip_amount", arrow::float64()),
    arrow::field("tolls_amount", arrow::float64()),
    arrow::field("improvement_surcharge", arrow::float64()),
    arrow::field("total_amount", arrow::float64())
});
