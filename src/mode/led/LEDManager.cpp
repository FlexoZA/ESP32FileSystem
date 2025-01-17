#include "LEDManager.h"

LEDManager::LEDManager(uint8_t pin, uint16_t numPixels) 
    : strip(numPixels, pin, NEO_GRB + NEO_KHZ800),
      currentMode(LEDMode::SOLID_WHITE),
      brightness(50),
      lastUpdate(0),
      rainbowCycle(0) {
}

void LEDManager::begin() {
    strip.begin();
    strip.setBrightness(brightness);
    strip.show();
}

void LEDManager::update() {
    unsigned long currentTime = millis();
    
    // Update LED effects every 20ms (50Hz)
    if (currentTime - lastUpdate >= 20) {
        switch (currentMode) {
            case LEDMode::SOLID_WHITE:
                showSolidColor(strip.Color(255, 255, 255));
                break;
            case LEDMode::RAINBOW:
                showRainbow();
                break;
            case LEDMode::SOLID_BLUE:
                showSolidColor(strip.Color(0, 0, 255));
                break;
            case LEDMode::SOLID_RED:
                showSolidColor(strip.Color(255, 0, 0));
                break;
            default:
                break;
        }
        lastUpdate = currentTime;
    }
}

void LEDManager::setBrightness(uint8_t level) {
    brightness = level;
    strip.setBrightness(level);
    strip.show();
}

uint8_t LEDManager::getBrightness() const {
    return brightness;
}

void LEDManager::setMode(LEDMode mode) {
    currentMode = mode;
    rainbowCycle = 0; // Reset animation cycle when changing modes
}

LEDMode LEDManager::getMode() const {
    return currentMode;
}

void LEDManager::nextMode() {
    int nextMode = (static_cast<int>(currentMode) + 1) % static_cast<int>(LEDMode::MODE_COUNT);
    currentMode = static_cast<LEDMode>(nextMode);
}

void LEDManager::showSolidColor(uint32_t color) {
    for(uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

void LEDManager::showRainbow() {
    for(uint16_t i = 0; i < strip.numPixels(); i++) {
        int pixelHue = (rainbowCycle + (i * 65536L / strip.numPixels())) % 65536;
        strip.setPixelColor(i, wheel(pixelHue * 31 / 65536));
    }
    strip.show();
    rainbowCycle = (rainbowCycle + 256) % 65536;
}

uint32_t LEDManager::wheel(byte wheelPos) {
    wheelPos = 255 - wheelPos;
    if(wheelPos < 85) {
        return strip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
    }
    if(wheelPos < 170) {
        wheelPos -= 85;
        return strip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
    wheelPos -= 170;
    return strip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}
