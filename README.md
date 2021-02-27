This is work-in-process implementation of Network-Ordered BFT protocol.

# Step to run protocols with normal switch

```
# start a star-shape mininet topology with 5 hosts
# h1-4 for replica and h5 for client
$ sudo mn --topo single,5
mininet> h1 ./conf_mininet.sh h1 h2 h3 h4
mininet> h1 ./bench/replica -c mininet.conf -i 0 -m vr &
mininet> h2 ./bench/replica -c mininet.conf -i 1 -m vr &
mininet> h3 ./bench/replica -c mininet.conf -i 2 -m vr &
mininet> h4 ./bench/replica -c mininet.conf -i 3 -m vr &
mininet> h5 ./bench/client -c mininet.conf -m vr -n 100000
```

Change `-m vr` to `-m spec/fastpaxos/unreplicated` for other protocols. Use Ctrl-D to exit mininet after running.

One-click mininet script is in progress.

# Step to run protocols with network-ordered programmable switch

In progress.
