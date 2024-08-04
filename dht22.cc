// dht22.cc
#include "dht22.h"

DHT22::DHT22(int pin) : pin(pin) {
    data.fill(0);
}

void DHT22::resetData() {
    data.fill(0);
}

void DHT22::readData() {
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0;
    float f; // fahrenheit

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
        f = data[2] * 9. / 5. + 32;
        std::cout << "Humidity = " << static_cast<int>(data[0]) << "." << static_cast<int>(data[1])
                  << " % Temperature = " << static_cast<int>(data[2]) << "." << static_cast<int>(data[3])
                  << " *C (" << std::fixed << std::setprecision(1) << f << " *F)" << std::endl;
    } else {
        std::cout << "Data not good, skip" << std::endl;
    }
}

