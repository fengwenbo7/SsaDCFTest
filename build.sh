ifconfig wlp3s0:1 192.168.0.101 up
ifconfig wlp3s0:2 192.168.0.102 up
ifconfig wlp3s0:3 192.168.0.103 up
ifconfig wlp3s0:4 192.168.0.104 up
ifconfig wlp3s0:5 192.168.0.105 up
ifconfig wlp3s0:6 192.168.0.106 up


export LD_LIBRARY_PATH=$(pwd)/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/library/openssl/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/library/cJSON/lib:$LD_LIBRARY_PATH

printf "env set complete.\n"
gcc ./src/dcf_test.c ./src/dcf_demo.c ./src/message.c -L ./lib -ldcf -L /home/library/cJSON/lib -lcjson  -I ./include -g -o ./build/DCFTestClient -pthread
printf "build success.\n"