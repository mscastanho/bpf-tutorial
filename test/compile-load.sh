#!/bin/bash
if [ -z $1 ]; then
    echo "Usage: $0 <iface>"
    exit 1
fi

iface="$1"
clang -O2 -emit-llvm -c xdp.c -o - | llc -march=bpf -filetype=obj -o xdp.o
sudo ip -force link set dev $iface xdp obj xdp.o sec .text