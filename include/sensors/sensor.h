// sensor.h
#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
public:
    virtual void readData() = 0; // Pure virtual function
    virtual int getTemperature() const = 0;
    virtual int getHumidity() const = 0;
    virtual int getStatus() const =0;
    virtual ~Sensor() {}
};

#endif // SENSOR_H
