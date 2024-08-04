// main.cc
#include "dht11.h"
#include "mysql_connector.h"
#include <wiringPi.h>
#include <iostream>

int main() {
    std::cout << "Raspberry Pi wiringPi DHT11 Temperature test program" << std::endl;

    if (wiringPiSetup() == -1) {
        return 1;
    }

    DHT11 dht11Sensor(7);
    MySQLConnector db("10.24.0.1", "usr1", "debian", "amadoi");

    // Get the sampling rate from the database
    int samplingRate = db.getConstant("dht11_rate", 5000); // Default to 5000ms if not found

    while (true) {
        dht11Sensor.readData();
        db.insertData(dht11Sensor.getTemperature(), dht11Sensor.getHumidity(), dht11Sensor.getStatus(), 1);

        delay(samplingRate); // wait for the defined sampling rate
    }

    return 0;
}
