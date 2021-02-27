#include "common/replica.h"
#include "lib/configuration.h"
#include "lib/message.h"
#include "lib/transport.h"

#include "bft/bft-proto.pb.h"
#include "bft/replica.h"

namespace specpaxos {
namespace bft {
using namespace proto;

BFTReplica::BFTReplica(Configuration config, int myIdx, bool initialize,
                       Transport *transport, AppReplica *app)
    : Replica(config, 0, myIdx, initialize, transport, app) {}

BFTReplica::~BFTReplica() {}

void BFTReplica::ReceiveMessage(const TransportAddress &remote,
                                const string &type, const string &data,
                                void *meta_data) {
  static proto::RequestMessage request;

  if (type != request.GetTypeName()) {
    Panic("Unknown request");
  }

  request.ParseFromString(data);
  proto::ReplyMessage reply;
  Execute(0, request.req(), reply);
  reply.set_clientreqid(request.req().clientreqid());

  Debug("Replica sending message");
  transport->SendMessage(this, remote, reply);
}
} // namespace bft
} // namespace specpaxos