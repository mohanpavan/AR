#ifndef AR_CLIENT_H
#define AR_CLIENT_H

#include <string>
#include <netinet/in.h>

class Client
{
public:
    Client(const std::string &server_ip, const int port, const double frequency, const size_t b_size);
    void start();

private:
    int m_port;
    double m_frequency;
    size_t m_buf_size;
    std::string m_server_ip;
    int m_socket;
    sockaddr_in server_addr{};
    void send_motion_control_info() const;
    void receive_position_data() const;
};

#endif // AR_CLIENT_H
