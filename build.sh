export LD_LIBRARY_PATH=$(pwd)/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
printf "env set complete.\n"
gcc ./src/dcf_test.c ./src/dcf_demo.c ./src/message.c -L ./lib -ldcf  -I ./include -g -o ./build/DCFTestClient -pthread
printf "build success.\n"