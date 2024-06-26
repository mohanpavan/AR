#include <iostream>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "config.h"

class UDPServer {
public:
    UDPServer(int port, double frequency)
        : port(port), frequency(frequency) {}

    void start() {
        int server_socket;
        struct sockaddr_in servaddr, cliaddr;

        // Creating socket file descriptor
        if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
            perror("Socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));
        memset(&cliaddr, 0, sizeof(cliaddr));

        // Filling server information
        servaddr.sin_family = AF_INET; // IPv4
        servaddr.sin_addr.s_addr = INADDR_ANY;
        servaddr.sin_port = htons(port);

        // Bind the socket with the server address
        if (bind(server_socket, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            perror("Bind failed");
            exit(EXIT_FAILURE);
        }

        receive_and_send_data(server_socket);
    }

private:
    int port;
    double frequency;

    void receive_and_send_data(int server_socket) {
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        char buffer[BUFFER_SIZE];

        int position = 0;
        while (true) {
            int n = recvfrom(server_socket, (char *)buffer, BUFFER_SIZE,
                             MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
            buffer[n] = '\0';
            std::cout << "Client : " << buffer << std::endl;

            std::string message = "Position: " + std::to_string(position);
            sendto(server_socket, message.c_str(), message.length(),
                   MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0 / frequency)));
            position++;
        }
    }
};

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    double frequency = DEFAULT_FREQUENCY;

    if (argc > 1) {
        port = std::stoi(argv[1]);
    }
    if (argc > 2) {
        frequency = std::stod(argv[2]);
    }

    UDPServer server(port, frequency);
    server.start();

    return 0;
}
