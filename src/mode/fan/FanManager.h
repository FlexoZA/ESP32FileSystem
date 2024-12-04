#ifndef FAN_MANAGER_H
#define FAN_MANAGER_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class FanManager {
private:
    const int fanCenterX = 30;  // X position for the fan (left side)
    const int fanCenterY;        // Y position (centered vertically)
    const int fanRadius = 25;
    int rotationStep = 0;

    Adafruit_SSD1306& display;    // Reference to the display object

public:
    FanManager(Adafruit_SSD1306& disp);
    void animateFan();
    void drawFanBlade(int angle);
};

#endif
