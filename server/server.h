
#ifndef AR_SERVER_H
#define AR_SERVER_H

#include <netinet/in.h>
#include <functional>
#include <string>
#include <thread>
#include <position_message.h>

class Server
{
public:
    using CallbackType = std::function<void(const std::string&)>;
    Server(const int port, const double frequency, const size_t b_size);
    virtual ~Server();
    void start();
    void stop();
    void set_callback(CallbackType callback);

private:
    void receive_and_send_data();

    int m_port;
    double m_frequency;
    size_t m_buf_size;
    int m_socket;
    sockaddr_in m_server_addr{};
    sockaddr_in m_client_addr{};
    CallbackType m_callback;
    std::thread m_server_thread;
    bool m_running;
    PositionMessage m_message;
};

#endif // AR_SERVER_H
