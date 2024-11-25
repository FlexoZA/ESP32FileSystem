#include "SensorManager.h"
#include "../config/Config.h"

SensorManager::SensorManager() : dht(DHT_PIN, DHT11), temperature(0), humidity(0), lastReadTime(0) {
}

void SensorManager::begin() {
    dht.begin();
}

void SensorManager::update() {
    unsigned long currentTime = millis();
    if (currentTime - lastReadTime >= READ_INTERVAL) {
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();
        lastReadTime = currentTime;
    }
}

float SensorManager::getTemperature() {
    return temperature;
}

float SensorManager::getHumidity() {
    return humidity;
}