#include "client.h"
#include "config_reader.h"

int main(int argc, char *argv[])
{
    ConfigReader config("config.ini");

    std::string config_buffer;
    int port;
    double frequency;
    size_t buf_size;
    std::string server_ip;
    std::string value;

    if (config.getValue("PORT", value))
    {
        port = std::stoi(value);
        std::cout << "PORT: " << value << std::endl;
    }
    else
    {
        std::cerr << "PORT not found" << std::endl;
    }

    if (config.getValue("FREQUENCY", value))
    {
        frequency = std::stod(value);
        std::cout << "FREQUENCY: " << value << std::endl;
    }
    else
    {
        std::cerr << "FREQUENCY not found" << std::endl;
    }

    if (config.getValue("BUFFER_SIZE", value))
    {
        buf_size = std::stoul(value);
        std::cout << "BUFFER_SIZE: " << value << std::endl;
    }
    else
    {
        std::cerr << "BUFFER_SIZE not found" << std::endl;
    }

    if (config.getValue("SERVER_IP", server_ip))
    {
        std::cout << "SERVER_IP: " << server_ip << std::endl;
    }
    else
    {
        std::cerr << "SERVER_IP not found" << std::endl;
    }

    Client client(server_ip, port, frequency, buf_size);
    client.start();

    return 0;
}
