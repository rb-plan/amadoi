// dht11.h
#ifndef DHT11_H
#define DHT11_H

#include "sensors/sensor.h"
#include <wiringPi.h>
#include <array>
#include <iostream>
#include <iomanip>

class DHT11 : public Sensor {
public:
    DHT11(int pin);
    void readData() override;
    int getTemperature() const override;
    int getHumidity() const override;
    int getStatus() const override;

private:
    static const int MAXTIMINGS = 85;
    int pin;
    std::array<uint8_t, 5> data;

    void resetData();
    bool isValidData(uint8_t humidity, uint8_t temperature) const;
    int temperature;
    int humidity;
    int status;
};

#endif // DHT11_H
