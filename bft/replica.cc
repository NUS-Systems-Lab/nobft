#include "common/replica.h"
#include "lib/configuration.h"
#include "lib/message.h"
#include "lib/transport.h"
#include <cstring>

#include "bft/bft-proto.pb.h"
#include "bft/header.h"
#include "bft/replica.h"

namespace specpaxos {
namespace bft {
using namespace proto;
using namespace std;

BFTReplica::BFTReplica(Configuration config, int myIdx, bool initialize,
                       Transport *transport, AppReplica *app)
    : Replica(config, 0, myIdx, initialize, transport, app), log(false),
      seqNum(1) {}

BFTReplica::~BFTReplica() {}

void BFTReplica::ReceiveMessage(const TransportAddress &remote,
                                const string &type, const string &data,
                                void *meta_data) {
  static Metadata<4> *header;
  static proto::RequestMessage request;

  if (type != request.GetTypeName()) {
    Panic("Unknown request");
  }

  header = reinterpret_cast<struct Metadata<4> *>(meta_data);
  if (header->seqnum < seqNum) {
    return;
  }
  if (header->seqnum != seqNum) {
    auto record = unique_ptr<Record>(new Record);
    record->data = data;
    record->remote = unique_ptr<TransportAddress>(remote.clone());
    memcpy(&record->meta, meta_data, sizeof(Metadata<4>));
    buffer.insert(make_pair(uint32_t(header->seqnum), move(record)));
    Debug("Reordered packet %u buffered", header->seqnum);
    return;
  }

  Debug("Process in-order packet %u", header->seqnum);
  request.ParseFromString(data);
  HandleRequest(remote, request, reinterpret_cast<Metadata<4> *>(meta_data));

  while (buffer.count(seqNum)) {
    Debug("Process reordered packet %u", seqNum);
    Record &record = *buffer.at(seqNum);
    request.ParseFromString(record.data);
    HandleRequest(*record.remote, request, &record.meta);
    buffer.erase(seqNum - 1);
  }
}

void BFTReplica::HandleRequest(const TransportAddress &remote,
                               const RequestMessage &msg, Metadata<4> *meta) {

  viewstamp_t v;
  v.view = 0;
  v.opnum = seqNum;
  v.sessnum = 0;
  v.msgnum = 0;
  log.Append(v, msg.req(), LOG_STATE_RECEIVED);

  proto::ReplyMessage reply;
  Execute(0, msg.req(), reply);
  seqNum += 1;

  reply.set_clientreqid(msg.req().clientreqid());
  meta->type = SwitchType::REP;
  Debug("Replica sending message");
  transport->SendMessage2(this, remote, reply, meta, sizeof(*meta));
}

} // namespace bft
} // namespace specpaxos