// dht11.cc
#include "sensors/dht11.h"

DHT11::DHT11(int pin) : pin(pin), temperature(0), humidity(0), status(2) {
    data.fill(0);
}

void DHT11::resetData() {
    data.fill(0);
}

bool DHT11::isValidData(uint8_t humidity, uint8_t temperature) const {
    // DHT11 specifications:
    // Temperature: 0-50°C (0-50)
    // Humidity: 20-95% (20-95)
    // Check for obviously invalid values like 255 (sensor error)
    
    if (temperature == 255 || humidity == 255) {
        return false; // 255 indicates sensor error
    }
    
    if (temperature > 50) {
        return false; // Temperature out of DHT11 range
    }
    
    if (humidity < 20 || humidity > 95) {
        return false; // Humidity out of DHT11 range
    }
    
    return true;
}

void DHT11::readData() {
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0;

    resetData();

    // pull pin down for 18 milliseconds
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(18);
    // then pull it up for 40 microseconds
    digitalWrite(pin, HIGH);
    delayMicroseconds(40);
    // prepare to read the pin
    pinMode(pin, INPUT);

    // detect change and read data
    for (int i = 0; i < MAXTIMINGS; i++) {
        counter = 0;
        while (digitalRead(pin) == laststate) {
            counter++;
            delayMicroseconds(1);
            if (counter == 255) {
                break;
            }
        }
        laststate = digitalRead(pin);

        if (counter == 255) {
            break;
        }

        // ignore first 3 transitions
        if ((i >= 4) && (i % 2 == 0)) {
            // shove each bit into the storage bytes
            data[j / 8] <<= 1;
            if (counter > 16) {
                data[j / 8] |= 1;
            }
            j++;
        }
    }

    // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
    // print it out if data is good
    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        // Additional validity checks for DHT11
        if (isValidData(data[0], data[2])) {
            humidity = data[0];
            temperature = data[2];
            status = 0; // Data is good
        } else {
            humidity = 0;
            temperature = 0;
            status = 2; // Data is invalid (out of range)
            std::cout << "Error: Sensor data out of valid range - Temp: " << (int)data[2] 
                      << ", Hum: " << (int)data[0] << std::endl;
        }
    } else {
        // Check if we got any valid-looking data even with checksum failure
        if (isValidData(data[0], data[2])) {
            humidity = data[0];
            temperature = data[2];
            status = 1; // Data may be valid but checksum failed
            std::cout << "Warning: Checksum failed, but data appears valid. j=" << j 
                      << ", checksum=" << (int)data[4] 
                      << ", expected=" << ((data[0] + data[1] + data[2] + data[3]) & 0xFF) << std::endl;
        } else {
            humidity = 0;
            temperature = 0;
            status = 2; // Data is invalid
            std::cout << "Error: Invalid sensor data - Temp: " << (int)data[2] 
                      << ", Hum: " << (int)data[0] 
                      << ", j=" << j << ", checksum=" << (int)data[4] << std::endl;
        }
    }
}

int DHT11::getTemperature() const {
    return temperature;
}

int DHT11::getHumidity() const {
    return humidity;
}

int DHT11::getStatus() const {
    return status;
}