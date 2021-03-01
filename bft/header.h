#ifndef _BFT_HEADER_H
#define _BFT_HEADER_H

#include <cstdint>

namespace specpaxos {
namespace bft {

template <int n> struct __attribute__((packed)) Metadata {
  std::uint8_t type;
  std::uint32_t seqnum;
  std::uint16_t payloadlen;
  std::uint32_t switchid;
  std::uint32_t switchsigs[n + 1];
  std::uint8_t payloaddigest[20];
  char data[0]; // pointer passed to protobuf
};

enum SwitchType {
  REQ = 1,
  REP = 2,
};

}; // namespace bft
}; // namespace specpaxos

#endif