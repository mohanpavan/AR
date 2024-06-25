#include "client.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

Client::Client(const std::string &server_ip, const int port, const double frequency, const size_t b_size)
    : m_port(port), m_frequency(frequency), m_buf_size(b_size), m_server_ip(server_ip), m_socket(0) {}

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
    int motion_control = 0;
    while (true)
    {
        std::string message = "Motion Control: " + std::to_string(motion_control);
        sendto(m_socket, message.c_str(), message.length(),
               MSG_CONFIRM, (const struct sockaddr *)&server_addr, sizeof(server_addr));
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0 / m_frequency)));
        motion_control++;
    }
}

void Client::receive_position_data() const
{
    char buffer[m_buf_size] = {0};
    int last_position = -1;
    while (true)
    {
        const size_t n = recvfrom(m_socket, buffer, m_buf_size,
                         MSG_WAITALL, nullptr, nullptr);
        buffer[n] = '\0';
        std::cout << "Server : " << buffer << std::endl;

        int position = std::stoi(buffer + 10); // Assuming "Position: " is fixed prefix

        if (position == last_position)
        {
            std::cout << "Repeated data detected" << std::endl;
        }
        else if (position < last_position)
        {
            std::cout << "Out-of-order data detected" << std::endl;
        }
        else if (position > last_position + 1)
        {
            std::cout << "Lost data detected" << std::endl;
        }
        last_position = position;
    }
}
