#include <iostream>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "config.h"

class UDPClient {
public:
    UDPClient(const std::string &server_ip, int port, double frequency)
        : server_ip(server_ip), port(port), frequency(frequency) {}

    void start() {
        int client_socket;
        struct sockaddr_in servaddr;

        if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            std::cerr << "Socket creation error" << std::endl;
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(port);
        servaddr.sin_addr.s_addr = inet_addr(server_ip.c_str());

        std::thread send_thread(&UDPClient::send_motion_control_info, this, client_socket, servaddr);
        std::thread receive_thread(&UDPClient::receive_position_data, this, client_socket);

        send_thread.join();
        receive_thread.join();

        close(client_socket);
    }

private:
    std::string server_ip;
    int port;
    double frequency;

    void send_motion_control_info(int client_socket, struct sockaddr_in servaddr) {
        int motion_control = 0;
        while (true) {
            std::string message = "Motion Control: " + std::to_string(motion_control);
            sendto(client_socket, message.c_str(), message.length(),
                   MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0 / frequency)));
            motion_control++;
        }
    }

    void receive_position_data(int client_socket) {
        char buffer[BUFFER_SIZE] = {0};
        int last_position = -1;
        while (true) {
            int n = recvfrom(client_socket, buffer, BUFFER_SIZE,
                             MSG_WAITALL, nullptr, nullptr);
            buffer[n] = '\0';
            std::cout << "Server : " << buffer << std::endl;

            int position = std::stoi(buffer + 10); // Assuming "Position: " is fixed prefix

            if (position == last_position) {
                std::cout << "Repeated data detected" << std::endl;
            } else if (position < last_position) {
                std::cout << "Out-of-order data detected" << std::endl;
            } else if (position > last_position + 1) {
                std::cout << "Lost data detected" << std::endl;
            }
            last_position = position;
        }
    }
};

int main(int argc, char *argv[]) {
    std::string server_ip = "127.0.0.1";
    int port = DEFAULT_PORT;
    double frequency = DEFAULT_FREQUENCY;

    if (argc > 1) {
        server_ip = argv[1];
    }
    if (argc > 2) {
        port = std::stoi(argv[2]);
    }
    if (argc > 3) {
        frequency = std::stod(argv[3]);
    }

    UDPClient client(server_ip, port, frequency);
    client.start();

    return 0;
}
