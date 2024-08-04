// main.cc
#include "dht11.h"
#include "dht22.h"
#include "mysql_connector.h"
#include <wiringPi.h>

int main() {
    std::cout << "Raspberry Pi wiringPi DHT11/DHT22 Temperature test program" << std::endl;

    if (wiringPiSetup() == -1) {
        return 1;
    }

    DHT11 dht11Sensor(7);
    DHT22 dht22Sensor(0); // Assuming DHT22 is connected to GPIO 0
    MySQLConnector db("10.24.0.1", "usr1", "debian", "amadoi");

    while (true) {
        dht11Sensor.readData();
        db.insertData(dht11Sensor.getTemperature(), dht11Sensor.getHumidity(), 0, 1);

        dht22Sensor.readData();
        db.insertData(dht22Sensor.getTemperature(), dht22Sensor.getHumidity(), 0, 2);

        delay(5000); // wait 5 seconds to refresh
    }

    return 0;
}
