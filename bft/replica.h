#ifndef _BFT_REPLICA_H_
#define _BFT_REPLICA_H_

#include "common/log.h"
#include "common/quorumset.h"
#include "common/replica.h"
#include "lib/configuration.h"
#include <map>
#include <memory>

#include "bft/bft-proto.pb.h"
#include "bft/header.h"

namespace specpaxos {
namespace bft {

class BFTReplica : public Replica {
public:
  BFTReplica(Configuration config, int myIdx, bool initialize,
             Transport *transport, AppReplica *app);
  ~BFTReplica();

  void ReceiveMessage(const TransportAddress &remote, const string &type,
                      const string &data, void *meta_data) override;

private:
  Log log;
  struct Record {
    string data;
    std::unique_ptr<TransportAddress> remote;
    Metadata<4> meta;
  };
  std::map<std::uint32_t, std::unique_ptr<Record>> buffer;
  uint32_t seqNum;

  void HandleRequest(const TransportAddress &remote,
                     const proto::RequestMessage &msg, Metadata<4> *meta);
};

} // namespace bft
} // namespace specpaxos

#endif