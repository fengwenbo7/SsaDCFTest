#libzstd
yum install epel-release.noarch -y
yum install libzstd.x86_64 -y
printf "========================================================================"
printf "libzstd install complete!"
printf "========================================================================\n"

#libssl
cd /usr/local/src
wget https://github.com/openssl/openssl/archive/OpenSSL_1_1_1c.tar.gz
tar xzvf ./OpenSSL_1_1_1c.tar.gz
cd openssl-OpenSSL_1_1_1c/
./config
make && make install
export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
printf "========================================================================"
printf "libssl install complete!"
printf "========================================================================\n"

#libcjson
cd /usr/local/src
git clone https://github.com/DaveGamble/cJSON.git
cd cJSON/
make & make install
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
printf "========================================================================"
printf "libcjson install complete!"
printf "========================================================================\n"

#firewall
systemctl stop firewalld.service
systemctl disable firewalld.service

#lz4 
yum update lz4 -y

#libdcf
cd $DCFTEST_BASE
export LD_LIBRARY_PATH=$(pwd)/lib:$LD_LIBRARY_PATH
gcc ./src/dcf_test.c ./src/dcf_demo.c ./src/message.c -L ./lib -ldcf  -I ./include -g -o ./build/DCFTestClient -pthread