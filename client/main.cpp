#include "client.h"
#include "config_reader.h"

int main(int argc, char *argv[])
{
    const ConfigReader config("config.ini");

    int port=8080;
    double frequency = 1.F;
    size_t buf_size = 1024;
    std::string server_ip = "127.0.0.1";
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
