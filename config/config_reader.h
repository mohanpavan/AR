#ifndef AR_CONFIG_READER_H
#define AR_CONFIG_READER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

class ConfigReader {
public:
    ConfigReader(const std::string& filename) {
        loadConfigFile(filename);
    }

    bool getValue(const std::string& key, std::string& value) const {
        auto it = configMap.find(key);
        if (it != configMap.end()) {
            value = it->second;
            return true;
        }
        return false;
    }

private:
    std::unordered_map<std::string, std::string> configMap;

    void loadConfigFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open config file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::string key, value;
            if (parseLine(line, key, value)) {
                configMap[key] = value;
            }
        }
        file.close();
    }

    bool parseLine(const std::string& line, std::string& key, std::string& value) const {
        std::istringstream iss(line);
        std::string token;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            return true;
        }
        return false;
    }
};

#endif // AR_CONFIG_READER_H
