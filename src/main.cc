// main.cc
#include "sensors/dht11.h"
#include "api/api_uploader.h"
#include "config/config_reader.h"
#include <wiringPi.h>
#include <iostream>

int main() {
    std::cout << "Raspberry Pi wiringPi DHT11 Temperature test program with API upload" << std::endl;

    if (wiringPiSetup() == -1) {
        return 1;
    }

    // Load configuration
    ConfigReader config("config/amadoi.yml");
    if (!config.loadConfig()) {
        std::cerr << "Failed to load configuration file" << std::endl;
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

    while (true) {
        dht11Sensor.readData();
        
        // Get raw sensor data
        auto cTemp = dht11Sensor.getTemperature();
        auto cHumi = dht11Sensor.getHumidity();
        
        printf("Raw sensor data - Temp: %d, Hum: %d\n", cTemp, cHumi);
        
        // Apply calibration if data is valid
        if (cTemp > 0 && cHumi > 0) {
            cTemp += tempOffset; // Apply temperature offset
            std::cout << "Calibrated data - Temp: " << cTemp << ", Hum: " << cHumi << std::endl;
            
            // Upload to API
            bool success = apiUploader.uploadSensorData(cTemp, cHumi, dht11Sensor.getStatus());
            if (!success) {
                std::cerr << "Failed to upload sensor data to API" << std::endl;
            }
        } else {
            std::cout << "Invalid sensor data, skipping upload" << std::endl;
        }

        delay(samplingRate); // wait for the defined sampling rate
    }

    return 0;
}
