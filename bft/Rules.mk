d := $(dir $(lastword $(MAKEFILE_LIST)))

SRCS += $(addprefix $(d), \
	replica.cc client.cc)

PROTOS += $(addprefix $(d), \
	    bft-proto.proto)

OBJS-bft-client := $(o)client.o $(o)bft-proto.o \
                   $(OBJS-client) $(LIB-message) \
                   $(LIB-configuration)

OBJS-bft-replica := $(o)replica.o $(o)bft-proto.o \
                   $(OBJS-replica) $(LIB-message) \
                   $(LIB-configuration)
