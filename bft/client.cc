//

#include "common/client.h"
#include "lib/message.h"
#include "lib/transport.h"

#include "bft/bft-proto.pb.h"
#include "bft/client.h"

namespace specpaxos {
namespace bft {

BFTClient::BFTClient(const Configuration &config, Transport *transport,
                     uint64_t client_id)
    : Client(config, transport, client_id), cont(nullptr), req_id(0) {}

BFTClient::~BFTClient() {}

void BFTClient::Invoke(const string &request, continuation_t continuation) {
  proto::RequestMessage reqMsg;
  reqMsg.mutable_req()->set_op(request);
  reqMsg.mutable_req()->set_clientid(clientid);
  reqMsg.mutable_req()->set_clientreqid(req_id);
  Debug("Client sending message to all, req_id = %lu", req_id);
  transport->SendMessageToAll(this, reqMsg);
  // todo: resend timeout

  req = request;
  cont = continuation;
}

void BFTClient::ReceiveMessage(const TransportAddress &remote,
                               const string &type, const string &data,
                               void *meta_data) {
  static proto::ReplyMessage reply;
  Debug("Client received message");
  if (type != reply.GetTypeName()) {
    Debug("Incorrect type");
    return Client::ReceiveMessage(remote, type, data, meta_data);
  }
  reply.ParseFromString(data);
  if (reply.clientreqid() != req_id) {
    Debug("Incorrect req_id, expected %lu get %lu", req_id, reply.clientreqid());
    return;
  }
  req_id += 1;
  Debug("Calling continuation");
  cont(req, reply.reply());
  req.clear();
}

} // namespace bft
} // namespace specpaxos