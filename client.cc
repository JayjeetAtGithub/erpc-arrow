#include "common.h"

erpc::Rpc<erpc::CTransport> *rpc;
erpc::MsgBuffer init_req;
erpc::MsgBuffer init_resp;

erpc::MsgBuffer nb_req;
erpc::MsgBuffer nb_resp;

void init_func(void *, void *) { printf("%s\n", init_resp.buf_); }
void nb_func(void *, void *) { printf("%s\n", nb_resp.buf_); }

void sm_handler(int, erpc::SmEventType, erpc::SmErrType, void *) {}

int main() {
  std::string client_uri = kClientHostname + ":" + std::to_string(kUDPPort);
  erpc::Nexus nexus(client_uri);

  rpc = new erpc::Rpc<erpc::CTransport>(&nexus, nullptr, 0, sm_handler);

  std::string server_uri = kServerHostname + ":" + std::to_string(kUDPPort);
  int session_num = rpc->create_session(server_uri, 0);

  while (!rpc->is_connected(session_num)) rpc->run_event_loop_once();

  init_req = rpc->alloc_msg_buffer_or_die(kSmallMsgSize);
  init_resp = rpc->alloc_msg_buffer_or_die(kSmallMsgSize);
  rpc->enqueue_request(session_num, kInitRpc, &init_req, &init_resp, init_func, nullptr);
  
  for (int i = 0; i < 10; i++) {  
  	nb_req = rpc->alloc_msg_buffer_or_die(kLargeMsgSize);
  	nb_resp = rpc->alloc_msg_buffer_or_die(kLargeMsgSize);
  	rpc->enqueue_request(session_num, kNextBatchRpc, &nb_req, &nb_resp, nb_func, nullptr);
  }
  rpc->run_event_loop(2000);

  delete rpc;
}
