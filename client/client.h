#ifndef AR_CLIENT_H
#define AR_CLIENT_H

#include <string>
#include <netinet/in.h>

class Client
{
public:
    Client(const std::string &server_ip, int port, double frequency, size_t b_size);
    void start();

private:
    std::string server_ip;
    int port;
    double frequency;
    size_t buf_size;

    void send_motion_control_info(int client_socket, struct sockaddr_in servaddr);
    void receive_position_data(int client_socket);
};

#endif // AR_CLIENT_H
