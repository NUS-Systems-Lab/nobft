#!/bin/bash

cat << EOF > mininet.conf
f 1
replica $1:2021
replica $2:2021
replica $3:2021
replica $4:2021
EOF
