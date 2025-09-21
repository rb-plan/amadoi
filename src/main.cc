// main.cc
#include "sensors/dht11.h"
#include "api/api_uploader.h"
#include "config/config_reader.h"
#include <wiringPi.h>
#include <iostream>
#include <string>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -c, --config FILE    Specify configuration file (default: config/amadoi.yml)" << std::endl;
    std::cout << "  -h, --help           Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << programName << "                           # Use default config" << std::endl;
    std::cout << "  " << programName << " -c /etc/amadoi/config.yml  # Use custom config" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string configFile = "config/amadoi.yml";
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-c" || arg == "--config") {
            if (i + 1 < argc) {
                configFile = argv[++i];
            } else {
                std::cerr << "Error: --config requires a file path" << std::endl;
                printUsage(argv[0]);
                return 1;
            }
        } else {
            std::cerr << "Error: Unknown argument '" << arg << "'" << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }

    std::cout << "Raspberry Pi wiringPi DHT11 Temperature test program with API upload" << std::endl;
    std::cout << "Using configuration file: " << configFile << std::endl;

    if (wiringPiSetup() == -1) {
        return 1;
    }

    // Load configuration
    ConfigReader config(configFile);
    if (!config.loadConfig()) {
        std::cerr << "Failed to load configuration file: " << configFile << std::endl;
        return 1;
    }

    // Initialize sensor with pin from config
    int sensorPin = config.getInt("sensor_pin", 7);
    DHT11 dht11Sensor(sensorPin);

    // Initialize API uploader
    std::string apiUrl = config.getString("api_url", "http://localhost:8080/habitat/raw/add");
    std::string deviceCode = config.getString("device_code", "SENS-FARM01");
    ApiUploader apiUploader(apiUrl, deviceCode);

    // Get configuration values
    int samplingRate = config.getInt("sampling_rate", 5000);
    int tempOffset = config.getInt("temp_offset", -2);

    std::cout << "Configuration loaded:" << std::endl;
    std::cout << "  API URL: " << apiUrl << std::endl;
    std::cout << "  Device Code: " << deviceCode << std::endl;
    std::cout << "  Sampling Rate: " << samplingRate << "ms" << std::endl;
    std::cout << "  Sensor Pin: " << sensorPin << std::endl;
    std::cout << "  Temperature Offset: " << tempOffset << std::endl;

    std::cout << "\nStarting sensor monitoring loop..." << std::endl;
    std::cout << "Press Ctrl+C to stop\n" << std::endl;

    while (true) {
        dht11Sensor.readData();
        
        // Get raw sensor data
        auto cTemp = dht11Sensor.getTemperature();
        auto cHumi = dht11Sensor.getHumidity();
        auto status = dht11Sensor.getStatus();
        
        printf("Raw sensor data - Temp: %d, Hum: %d, Status: %d\n", cTemp, cHumi, status);
        
        // Check sensor status and data validity
        if (status == 0) {
            // Data is good
            cTemp += tempOffset; // Apply temperature offset
            std::cout << "Calibrated data - Temp: " << cTemp << ", Hum: " << cHumi << std::endl;
            
            // Upload to API
            bool success = apiUploader.uploadSensorData(cTemp, cHumi, status);
            if (!success) {
                std::cerr << "Failed to upload sensor data to API" << std::endl;
            }
        } else if (status == 1) {
            // Data may be valid but checksum failed
            cTemp += tempOffset; // Apply temperature offset
            std::cout << "Using data with checksum warning - Temp: " << cTemp << ", Hum: " << cHumi << std::endl;
            
            // Upload to API with warning
            bool success = apiUploader.uploadSensorData(cTemp, cHumi, status);
            if (!success) {
                std::cerr << "Failed to upload sensor data to API" << std::endl;
            }
        } else {
            // Data is invalid
            std::cout << "Invalid sensor data (status: " << status << "), skipping upload" << std::endl;
            std::cout << "Possible causes: sensor not connected, power issue, or hardware problem" << std::endl;
        }

        delay(samplingRate); // wait for the defined sampling rate
    }

    return 0;
}
