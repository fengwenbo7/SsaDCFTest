gcc ./src/dcf_test.c ./src/dcf_demo.c ./src/message.c -L ./lib -ldcf  -I ./include -g -o ./build/DCFTestClient -pthread
cd build&&./DCFTestClient
