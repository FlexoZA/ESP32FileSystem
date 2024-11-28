#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "config/Config.h"

class WifiManager {
private:
    bool connected;
    bool blinkState;
    unsigned long lastBlinkTime;

public:
    WifiManager();
    void begin();
    void update();
    bool isConnected() const { return connected; }
    bool getBlinkState() const { return blinkState; }
};

#endif