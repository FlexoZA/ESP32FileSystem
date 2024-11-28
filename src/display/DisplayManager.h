#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_SSD1306.h>
#include "../config/Config.h"
#include "../communication/bluetooth/BluetoothManager.h"
#include "../communication/wifi/WifiManager.h"

class DisplayManager {
public:
    DisplayManager(int width, int height, TwoWire *wire, BluetoothManager& btManager, WifiManager& wifiMgr);
    void begin();
    void drawDefaultScreen(float temperature, float humidity);
    void updateScrollingText(const String& text);
    void drawPlayIcon(int x, int y);

private:
    Adafruit_SSD1306 display;
    BluetoothManager& bluetoothManager;
    WifiManager& wifiManager;
    int screenWidth;
    int screenHeight;
    int scrollPosition = 0;
    unsigned long lastScrollTime = 0;
    const int SCROLL_DELAY = 50;  // Scroll speed in milliseconds
    const int SCROLL_STEP = 1;    // Pixels to move per step
};

#endif // DISPLAY_MANAGER_H