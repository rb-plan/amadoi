// main.cc
#include "dht11.h"
#include "dht22.h"

int main() {
    std::cout << "Raspberry Pi wiringPi DHT11/DHT22 Temperature test program" << std::endl;

    if (wiringPiSetup() == -1) {
        return 1;
    }

    DHT11 dht11Sensor(7);
    DHT22 dht22Sensor(0); // 假设DHT22连接到GPIO 0

    while (true) {
        std::cout << "DHT11 Sensor Reading:" << std::endl;
        dht11Sensor.readData();
        std::cout << "DHT22 Sensor Reading:" << std::endl;
        dht22Sensor.readData();
        delay(10000); // wait 10 sec to refresh
    }

    return 0;
}
