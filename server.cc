#include "common.h"
#include "compute.h"

erpc::Rpc<erpc::CTransport> *rpc;

std::shared_ptr<arrow::RecordBatchReader> reader;

void init_req_handler(erpc::ReqHandle *req_handle, void *) {
  // cp::ExecContext exec_ctx;
  // reader = ScanDataset(exec_ctx, "dataset", "100").ValueOrDie();
  auto &resp = req_handle->pre_resp_msgbuf_;
  rpc->resize_msg_buffer(&resp, kSmallMsgSize);
  sprintf(reinterpret_cast<char *>(resp.buf_), "init");
  rpc->enqueue_response(req_handle, &resp);
}

void next_batch_req_handler(erpc::ReqHandle *req_handle, void *) {
  // std::shared_ptr<arrow::RecordBatch> batch;
  // if (reader->ReadNext(&batch).ok() && batch != nullptr) {
  //   std::shared_ptr<arrow::Array> col_arr = batch->column(0);
  //   arrow::Type::type type = col_arr->type_id();
  //   int64_t null_count = col_arr->null_count();

  //   std::shared_ptr<arrow::Buffer> data_buff =
  //                     std::static_pointer_cast<arrow::PrimitiveArray>(col_arr)->values();
  //   size_t num_bytes = data_buff->size();

  //   auto &resp = req_handle->dyn_resp_msgbuf_;
  //   resp  =  rpc->alloc_msg_buffer(num_bytes);
    
  //   rpc->resize_msg_buffer(&resp, num_bytes);
  //   sprintf(reinterpret_cast<char*>(resp.buf_), (char*)data_buff->data());
  //   rpc->enqueue_response(req_handle, &resp);
  //   std::cout << "ok\n";
  //   rpc->free_msg_buffer(resp);
  // } else {
    auto &resp = req_handle->pre_resp_msgbuf_;
    rpc->resize_msg_buffer(&resp, kSmallMsgSize);
    sprintf(reinterpret_cast<char *>(resp.buf_), "success");
    rpc->enqueue_response(req_handle, &resp);
  // }
}

int main() {
  std::string server_uri = kServerHostname + ":" + std::to_string(kUDPPort);
  erpc::Nexus nexus(server_uri);

  nexus.register_req_func(kInitRpc, init_req_handler);
  nexus.register_req_func(kNextBatchRpc, next_batch_req_handler);

  rpc = new erpc::Rpc<erpc::CTransport>(&nexus, nullptr, 0, nullptr);
  rpc->run_event_loop(100000);
}
