// dht22.h
#ifndef DHT22_H
#define DHT22_H

#include "sensor.h"
#include <wiringPi.h>
#include <array>
#include <iostream>
#include <iomanip>

class DHT22 : public Sensor {
public:
    DHT22(int pin);
    void readData() override;

private:
    static const int MAXTIMINGS = 85;
    int pin;
    std::array<uint8_t, 5> data;

    void resetData();
};

#endif // DHT22_H
