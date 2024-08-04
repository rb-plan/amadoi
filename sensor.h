// sensor.h
#ifndef SENSOR_H
#define SENSOR_H

class Sensor {
public:
    virtual void readData() = 0;
    virtual ~Sensor() {}
};

#endif // SENSOR_H
