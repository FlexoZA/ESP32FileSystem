#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <DHT.h>

class SensorManager {
public:
    SensorManager();
    void begin();
    void update();
    float getTemperature();
    float getHumidity();

private:
    DHT dht;
    float temperature;
    float humidity;
    unsigned long lastReadTime;
    static const unsigned long READ_INTERVAL = 2000; // Read every 2 seconds
};

#endif
