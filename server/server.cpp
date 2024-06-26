#include "server.h"
#include <iostream>
#include <cstring>
#include <utility>
#include <unistd.h>
#include <sys/socket.h>

Server::Server(int port, double frequency, size_t b_size)
    : m_port(port), m_frequency(frequency), m_buf_size(b_size), m_socket(0), m_running(false) {}

Server::~Server() {
    stop();
}

void Server::start() {
    memset(&m_server_addr, 0, sizeof(m_server_addr));
    memset(&m_client_addr, 0, sizeof(m_client_addr));
    // Creating socket file descriptor
    if ((m_socket = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Filling server information
    m_server_addr.sin_family = AF_INET; // IPv4
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(m_port);

    // Bind the socket with the server address
    if (bind(m_socket, (const struct sockaddr *)&m_server_addr, sizeof(m_server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    m_running = true;
    m_server_thread = std::thread(&Server::receive_and_send_data, this);
}

void Server::stop() {
    m_running = false;
    if (m_server_thread.joinable()) {
        m_server_thread.join();
    }
    close(m_socket);
}

void Server::set_callback(CallbackType callback) {
    m_callback = std::move(callback);
}

void Server::receive_and_send_data() {
    socklen_t len = sizeof(m_client_addr);
    char buffer[m_buf_size];

    int position = 0;
    while (m_running) {
        const size_t n = recvfrom(m_socket, buffer, m_buf_size,
                                  MSG_WAITALL, (struct sockaddr *)&m_client_addr, &len);
        if (n > 0) {
            buffer[n] = '\0';
            std::string data_received(buffer);

            if (m_callback) {
                m_callback(data_received);
            } else {
                std::cout << "Client : " << buffer << std::endl;
            }

            std::string message = "Position: " + std::to_string(position);
            sendto(m_socket, message.c_str(), message.length(),
                   MSG_CONFIRM, (const struct sockaddr *)&m_client_addr, len);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0 / m_frequency)));
            position++;
        }
    }
}
