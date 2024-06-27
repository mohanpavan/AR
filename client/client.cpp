#include <iostream>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "client.h"
#include <random>
#include "position_message.h"

Client::Client(const std::string &server_ip, const int port,
               const double frequency, const size_t b_size)
    : m_port(port), m_frequency(frequency), m_buf_size(b_size),
      m_server_ip(server_ip), m_socket(0) {}

void Client::start()
{
    if ((m_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(m_port);
    server_addr.sin_addr.s_addr = inet_addr(m_server_ip.c_str());

    std::thread send_thread(&Client::send_motion_control_info, this);
    std::thread receive_thread(&Client::receive_position_data, this);

    send_thread.join();
    receive_thread.join();

    close(m_socket);
}

void Client::send_motion_control_info() const
{
    PositionMessage control = {0};
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(-5, 5);
    while (true)
    {
        int chance = dist6(dev);
        if (chance == 0)
        {
            control.position.x = dist6(dev);
            control.position.y = dist6(dev);
            control.position.z = dist6(dev);
            std::string message = control.serialize();
            sendto(m_socket, message.c_str(), message.length(),
                   MSG_CONFIRM, (const struct sockaddr *)&server_addr, sizeof(server_addr));
            control.id++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0 / m_frequency)));
    }
}

void Client::receive_position_data() const
{
    char message[m_buf_size] = {0};
    int last_position = -1;
    while (true)
    {
        const size_t n = recvfrom(m_socket, message, m_buf_size, MSG_WAITALL, nullptr, nullptr);
        message[n] = '\0';
        std::cout << "Server : " << message << std::endl;

        PositionMessage position = PositionMessage::deserialize(message);
        if (position.id == last_position)
        {
            std::cout << "Repeated data detected" << std::endl;
        }
        else if (position.id < last_position)
        {
            std::cout << "Out-of-order data detected" << std::endl;
        }
        else if (position.id > last_position + 1)
        {
            std::cout << "Lost data detected" << std::endl;
        }
        last_position = position.id;
    }
}
