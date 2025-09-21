// config_reader.cc
#include "config/config_reader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

ConfigReader::ConfigReader(const std::string& configFile) : configFile(configFile) {
}

ConfigReader::~ConfigReader() {
}

bool ConfigReader::loadConfig() {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << configFile << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        parseLine(line);
    }
    
    file.close();
    return true;
}

void ConfigReader::parseLine(const std::string& line) {
    // Skip empty lines and comments
    std::string trimmed = trim(line);
    if (trimmed.empty() || trimmed[0] == '#') {
        return;
    }
    
    size_t pos = trimmed.find(':');
    if (pos == std::string::npos) {
        return;
    }
    
    std::string key = trim(trimmed.substr(0, pos));
    std::string value = trim(trimmed.substr(pos + 1));
    
    if (!key.empty() && !value.empty()) {
        configData[key] = value;
    }
}

std::string ConfigReader::trim(const std::string& str) const {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string ConfigReader::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = configData.find(key);
    if (it != configData.end()) {
        return it->second;
    }
    return defaultValue;
}

int ConfigReader::getInt(const std::string& key, int defaultValue) const {
    auto it = configData.find(key);
    if (it != configData.end()) {
        try {
            return std::stoi(it->second);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing int value for key '" << key << "': " << e.what() << std::endl;
        }
    }
    return defaultValue;
}

double ConfigReader::getDouble(const std::string& key, double defaultValue) const {
    auto it = configData.find(key);
    if (it != configData.end()) {
        try {
            return std::stod(it->second);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing double value for key '" << key << "': " << e.what() << std::endl;
        }
    }
    return defaultValue;
}
