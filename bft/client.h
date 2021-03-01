//

#ifndef _BFT_CLIENT_H
#define _BFT_CLIENT_H

#include "common/client.h"
#include "common/quorumset.h"
#include "lib/configuration.h"
#include "lib/message.h"
#include <map>

namespace specpaxos {
namespace bft {

class BFTClient : public Client {
public:
  BFTClient(const Configuration &config, Transport *transport,
            uint64_t client_id = 0);
  virtual ~BFTClient();
  virtual void Invoke(const string &request, continuation_t continuation);
  virtual void ReceiveMessage(const TransportAddress &remote,
                              const string &type, const string &data,
                              void *meta_data);
  virtual void
  InvokeUnlogged(int replicaIdx, const string &request,
                 continuation_t continuation,
                 timeout_continuation_t timeoutContinuation = nullptr,
                 uint32_t timeout = DEFAULT_UNLOGGED_OP_TIMEOUT) {
    Panic("Not implemented");
  }
private:
  string req;
  continuation_t cont;
  uint64_t reqid;
  // tried but the abstraction is shit
  // QuorumSet<uint64_t, proto::ReplyMessage> quorum;
  int count;
};

} // namespace bft
} // namespace specpaxos

#endif