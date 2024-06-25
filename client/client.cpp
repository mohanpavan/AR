#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <thread>

#define PORT 8080

struct PositionData {
    int sequence;
    double x, y, z;
};

struct MotionControl {
    int sequence;
    double vx, vy, vz;
};

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Socket creation error" << std::endl;
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }
    
    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }
    
    PositionData position;
    MotionControl motion;
    int last_sequence = -1;
    
    while(true) {
        // Receive position data
        recv(sock, &position, sizeof(position), 0);
        auto receive_time = std::chrono::steady_clock::now();
        std::cout << "Received position at " 
                  << std::chrono::duration_cast<std::chrono::milliseconds>(receive_time.time_since_epoch()).count() 
                  << "ms: " << position.x << ", " << position.y << ", " << position.z << std::endl;
        
        // Check for communication failures
        if (position.sequence <= last_sequence) {
            std::cout << "Warning: Repeated or out-of-order data detected!" << std::endl;
        } else if (position.sequence > last_sequence + 1) {
            std::cout << "Warning: Lost data detected!" << std::endl;
        }
        last_sequence = position.sequence;
        
        // Generate motion control data (simple example)
        motion.sequence = position.sequence;
        motion.vx = -position.x * 0.1;  // Move towards origin
        motion.vy = -position.y * 0.1;
        motion.vz = -position.z * 0.1;
        
        // Send motion control data
        send(sock, &motion, sizeof(motion), 0);
        std::cout << "Sent motion control: " << motion.vx << ", " << motion.vy << ", " << motion.vz << std::endl;
    }
    
    return 0;
}