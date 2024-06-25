#include <iostream>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "config.h"

class Server {
public:
    Server(int port, int frequency)
            : port(port), frequency(frequency) {}

    void start() {
        int server_fd, client_socket;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("Socket failed");
            exit(EXIT_FAILURE);
        }

        // Forcefully attaching socket to the port
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
            perror("Setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Binding the socket to the network address and port
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            perror("Bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0) {
            perror("Listen");
            exit(EXIT_FAILURE);
        }
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept");
            exit(EXIT_FAILURE);
        }

        send_position_data(client_socket);

        close(client_socket);
        close(server_fd);
    }

private:
    int port;
    int frequency;

    void send_position_data(int client_socket) {
        int position = 0;
        while (true) {
            std::string message = "Position: " + std::to_string(position);
            send(client_socket, message.c_str(), message.size(), 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frequency));
            position++;
        }
    }
};

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    int frequency = DEFAULT_FREQUENCY;

    if (argc > 1) {
        port = std::stoi(argv[1]);
    }
    if (argc > 2) {
        frequency = std::stoi(argv[2]);
    }

    Server server(port, frequency);
    server.start();

    return 0;
}
