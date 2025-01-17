#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <Arduino.h>
#include "config/Config.h"
#include <time.h>

class TimeManager {
private:
    struct tm timeinfo;
    const char* ntpServer = "pool.ntp.org";
    bool timeInitialized;
    String currentTime;
    
public:
    TimeManager();
    void begin();
    void update();
    String getFormattedTime() const;
    bool isTimeInitialized() const { return timeInitialized; }
};

#endif