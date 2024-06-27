# Running

```bash
docker compose build
docker compose up
```

# development
```bash
docker build -t udp .
docker run --net=host -it udp

# server
cd ./build/bin/
./server

# client
cd ./build/bin
./client
```
