#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_SSD1306.h>
#include "../config/Config.h"

class DisplayManager {
public:
    DisplayManager(int width = SCREEN_WIDTH, int height = SCREEN_HEIGHT, TwoWire *wire = nullptr);
    void begin();
    void drawDefaultScreen();
    void updateScrollingText(const String& text);

private:
    Adafruit_SSD1306 display;
    int screenWidth;
    int screenHeight;

    void drawWiFiIcon(int x, int y);
    void drawBluetoothIcon(int x, int y);

    // ... existing private members ...
    int scrollPosition = 0;
    unsigned long lastScrollTime = 0;
    const int SCROLL_DELAY = 50;  // Scroll speed in milliseconds
    const int SCROLL_STEP = 1;    // Pixels to move per step
};

#endif // DISPLAY_MANAGER_H