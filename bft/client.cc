//
#include "common/client.h"
#include "lib/message.h"
#include "lib/transport.h"
#include <cstring>

#include "bft/bft-proto.pb.h"
#include "bft/client.h"
#include "bft/header.h"

namespace specpaxos {
namespace bft {

BFTClient::BFTClient(const Configuration &config, Transport *transport,
                     uint64_t client_id)
    : Client(config, transport, client_id), cont(nullptr), reqid(0), count(0) {}

BFTClient::~BFTClient() {}

void BFTClient::Invoke(const string &request, continuation_t continuation) {
  proto::RequestMessage reqMsg;
  reqMsg.mutable_req()->set_op(request);
  reqMsg.mutable_req()->set_clientid(clientid);
  reqMsg.mutable_req()->set_clientreqid(reqid);

  Debug("Client sending message to all, reqid = %lu", reqid);
  struct Metadata<4> meta;
  std::memset(&meta, 0x0, sizeof(meta));
  meta.type = SwitchType::REQ;
  // payloadlen unused
  // todo: calculate 20 bytes digest, or mock it
  transport->OrderedMulticast2(this, reqMsg, &meta, sizeof(meta));

  // todo: resend timeout

  req = request;
  cont = continuation;
  count = 0;
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
  if (reply.clientreqid() != reqid) {
    Debug("Incorrect reqid, expected %lu get %lu", reqid, reply.clientreqid());
    return;
  }

  count += 1;
  Debug("Client got %d replies for reqid %lu", count, reqid);
  if (count < 3) {
    return;
  }

  reqid += 1;
  Debug("Calling continuation");
  cont(req, reply.reply());
  req.clear();
}

} // namespace bft
} // namespace specpaxos