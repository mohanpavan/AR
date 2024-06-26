
#ifndef AR_SERVER_H
#define AR_SERVER_H

#include <netinet/in.h>

class Server
{
public:
    Server(int port, double frequency, size_t b_size);
    void start();

private:
    int port;
    double frequency;
    size_t buf_size;

    void receive_and_send_data(int server_socket);
};

#endif // AR_SERVER_H
