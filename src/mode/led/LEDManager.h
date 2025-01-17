#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Adafruit_NeoPixel.h>
#include "config/Config.h"

enum class LEDMode {
    SOLID_WHITE,
    RAINBOW,
    SOLID_BLUE,
    SOLID_RED,
    MODE_COUNT
};

class LEDManager {
private:
    Adafruit_NeoPixel strip;
    LEDMode currentMode;
    uint8_t brightness;
    uint32_t lastUpdate;
    uint16_t rainbowCycle;
    
    // Helper methods
    void showSolidColor(uint32_t color);
    void showRainbow();
    uint32_t wheel(byte wheelPos);

public:
    LEDManager(uint8_t pin, uint16_t numPixels);
    void begin();
    void update();
    void setBrightness(uint8_t level);
    uint8_t getBrightness() const;
    void setMode(LEDMode mode);
    LEDMode getMode() const;
    void nextMode();
};

#endif
