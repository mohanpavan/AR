#include "server.h"
#include "config_reader.h"

int main()
{
    ConfigReader config("config.ini");

    int port;
    double frequency;
    size_t buf_size;
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

    Server server(port, frequency, buf_size);
    server.start();

    return 0;
}
