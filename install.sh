#!/bin/bash
set -ex

git clone https://github.com/JayjeetAtGithub/eRPC
./eRPC/scripts/packages/ubuntu18/required.sh

git clone https://github.com/linux-rdma/rdma-core
cd rdma-core
apt-get install -y build-essential cmake gcc libibverbs-dev ibverbs-utils libudev-dev libnl-3-dev libnl-route-3-dev ninja-build pkg-config valgrind python3-dev cython3 python3-docutils pandoc
cmake .
make -j32 install
cd ..

apt install -y make cmake g++ gcc libnuma-dev libgflags-dev numactl
sudo modprobe ib_uverbs
sudo modprobe ib_ipoib
sudo modprobe mlx4_ib

cd eRPC
cmake . -DPERF=OFF -DTESTING=OFF -DTRANSPORT=infiniband
make -j32 install

git clone https://github.com/JayjeetAtGithub/erpc-arrow
cd erpc-arrow
./deploy_arrow.sh
make infiniband

sudo bash -c "echo 2048 > /sys/devices/system/node/node0/hugepages/hugepages-2048kB/nr_hugepages"
sudo mkdir /mnt/huge
sudo mount -t hugetlbfs nodev /mnt/huge
