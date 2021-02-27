#ifndef _BFT_REPLICA_H_
#define _BFT_REPLICA_H_

#include "bft/bft-proto.pb.h"
#include "common/log.h"
#include "common/quorumset.h"
#include "common/replica.h"
#include "lib/configuration.h"

#include <list>
#include <map>
#include <memory>

namespace specpaxos {
namespace bft {

class BFTReplica : public Replica {
public:
  BFTReplica(Configuration config, int myIdx, bool initialize,
             Transport *transport, AppReplica *app);
  ~BFTReplica();

  void ReceiveMessage(const TransportAddress &remote, const string &type,
                      const string &data, void *meta_data) override;
};

} // namespace bft
} // namespace specpaxos

#endif