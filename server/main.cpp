#include "server.h"
#include "config_reader.h"

void data_received_callback(const std::string& data) {
    std::cout << "--Server: Position: " << data << std::endl;
}

int main()
{
    const ConfigReader config("config.ini");

    int port = 8080;
    double frequency = .1F;
    size_t buf_size = 1024;
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
    // server.set_callback(data_received_callback);
    server.start();
    std::this_thread::sleep_for(std::chrono::seconds(60));
    server.stop();

    return 0;
}
