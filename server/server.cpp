// server.cpp
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <cmath>

#define PORT 8080
#define FREQUENCY 0.01 // 0.01 Hz = 100 seconds per cycle

struct PositionData {
    int sequence;
    double x, y, z;
};

struct MotionControl {
    int sequence;
    double vx, vy, vz;
};

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    // Accept incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    PositionData position;
    MotionControl motion;
    int sequence = 0;
    
    auto next_time = std::chrono::steady_clock::now();
    const std::chrono::duration<double> interval(1.0 / FREQUENCY);

    while(true) {
        // Wait until the next interval
        std::this_thread::sleep_until(next_time);
        
        // Generate position data
        position.sequence = sequence++;
        position.x = sin(sequence * 0.1);
        position.y = cos(sequence * 0.1);
        position.z = tan(sequence * 0.1);
        
        // Send position data
        send(new_socket, &position, sizeof(position), 0);
        std::cout << "Sent position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
        
        // Receive motion control data
        recv(new_socket, &motion, sizeof(motion), 0);
        std::cout << "Received motion control: " << motion.vx << ", " << motion.vy << ", " << motion.vz << std::endl;
        
        // Check for communication failures
        if (motion.sequence != position.sequence) {
            std::cout << "Warning: Sequence mismatch detected!" << std::endl;
        }
        
        // Calculate the next wake-up time
        next_time += interval;
    }
    
    return 0;
}