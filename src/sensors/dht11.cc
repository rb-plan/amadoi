// dht11.cc
#include "sensors/dht11.h"

DHT11::DHT11(int pin) : pin(pin), temperature(0), humidity(0) {
    data.fill(0);
}

void DHT11::resetData() {
    data.fill(0);
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
        humidity = data[0];
        temperature = data[2];
        status = 0; // Data is good
    } else {
        status = 2; // Data not good
        // std::cout << "Data not good, skip" << std::endl;
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