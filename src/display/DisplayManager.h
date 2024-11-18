#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_SSD1306.h>
#include "../config/Config.h"

class DisplayManager {
public:
    DisplayManager(int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT, TwoWire *wire = nullptr);
    void begin();
    void drawDefaultScreen();

private:
    Adafruit_SSD1306 display;
    int screenWidth;
    int screenHeight;

    void drawWiFiIcon(int x, int y);
    void drawBluetoothIcon(int x, int y);
};

#endif // DISPLAY_MANAGER_H