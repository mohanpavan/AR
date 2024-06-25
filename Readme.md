docker build . -t udp:0.1

docker run --net=host -it udp:0.1
# server
cd ./build/bin/
./server

# client
cd ./build/bin
./client