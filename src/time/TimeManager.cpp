#include "time/TimeManager.h"

TimeManager::TimeManager() : timeInitialized(false) {
    currentTime = "00:00";
}

void TimeManager::begin() {
    configTime(GMT_OFFSET_SEC, 0, ntpServer); // No DST offset
    update(); // Initial time fetch
}

void TimeManager::update() {
    if (getLocalTime(&timeinfo)) {
        char timeStr[6];
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d", 
                timeinfo.tm_hour,
                timeinfo.tm_min);
        currentTime = String(timeStr);
        timeInitialized = true;
    }
}

String TimeManager::getFormattedTime() const {
    return currentTime;
}