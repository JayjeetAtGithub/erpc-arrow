#include "common.h"

erpc::Rpc<erpc::CTransport> *rpc;
erpc::MsgBuffer init_req;
erpc::MsgBuffer init_resp;

erpc::MsgBuffer nb_req;
erpc::MsgBuffer nb_resp;

void init_func(void *, void *) { std::cout << "Scan Started" << std::endl; }
void nb_func(void *, void *) { 
  if (nb_resp.get_data_size() == kSmallMsgSize) {
    std::cout << "Scan Finished" << std::endl;
    delete rpc;
    exit(0);
  } else {
    std::shared_ptr<arrow::Buffer> buf = std::make_shared<arrow::Buffer>(nb_resp.buf_, nb_resp.get_data_size());
    std::shared_ptr<arrow::DataType> type = schema->field(3)->type();
    std::shared_ptr<arrow::Array> col_arr = 
      std::make_shared<arrow::PrimitiveArray>(type, 131072, std::move(buf));
    std::cout << nb_resp.get_data_size() << std::endl;
    std::cout << col_arr->ToString() << std::endl;
  }
}

void sm_handler(int, erpc::SmEventType, erpc::SmErrType, void *) {}

int main() {
  std::string client_uri = kClientHostname + ":" + std::to_string(kUDPPort);
  erpc::Nexus nexus(client_uri);

  rpc = new erpc::Rpc<erpc::CTransport>(&nexus, nullptr, 0, sm_handler);

  std::string server_uri = kServerHostname + ":" + std::to_string(kUDPPort);
  int session_num = rpc->create_session(server_uri, 0);

  while (!rpc->is_connected(session_num)) rpc->run_event_loop_once();

  init_req = rpc->alloc_msg_buffer(kSmallMsgSize);
  init_resp = rpc->alloc_msg_buffer(kSmallMsgSize);
  rpc->enqueue_request(session_num, kInitRpc, &init_req, &init_resp, init_func, nullptr);
  rpc->free_msg_buffer(init_req);
  rpc->free_msg_buffer(init_resp);

  for (int i = 0; i < 30; i++) {  
  	nb_req = rpc->alloc_msg_buffer(kLargeMsgSize);
  	nb_resp = rpc->alloc_msg_buffer(kLargeMsgSize);
  	rpc->enqueue_request(session_num, kNextBatchRpc, &nb_req, &nb_resp, nb_func, nullptr);
    rpc->free_msg_buffer(nb_req);
    rpc->free_msg_buffer(nb_resp);
  }

  while (true) rpc->run_event_loop(100);
}
