// config_reader.h
#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <string>
#include <map>

class ConfigReader {
public:
    ConfigReader(const std::string& configFile);
    ~ConfigReader();
    
    bool loadConfig();
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue = 0) const;
    double getDouble(const std::string& key, double defaultValue = 0.0) const;

private:
    std::string configFile;
    std::map<std::string, std::string> configData;
    
    void parseLine(const std::string& line);
    std::string trim(const std::string& str) const;
};

#endif // CONFIG_READER_H
