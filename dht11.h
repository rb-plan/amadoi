// dht11.h
#ifndef DHT11_H
#define DHT11_H

#include "sensor.h"
#include <wiringPi.h>
#include <array>
#include <iostream>
#include <iomanip>

class DHT11 : public Sensor {
public:
    DHT11(int pin);
    void readData() override;

private:
    static const int MAXTIMINGS = 85;
    int pin;
    std::array<uint8_t, 5> data;

    void resetData();
};

#endif // DHT11_H
