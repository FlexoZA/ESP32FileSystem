#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Adafruit_SSD1306.h>
#include "../config/Config.h"
#include "../communication/bluetooth/BluetoothManager.h"
#include "../communication/wifi/WifiManager.h"
#include "../time/TimeManager.h"

class DisplayManager {
public:
    DisplayManager(int width, int height, TwoWire *wire, BluetoothManager& btManager, WifiManager& wifiMgr, TimeManager& timeMgr);
    void begin();
    void drawDefaultScreen(float temperature, float humidity);
    void updateScrollingText(const String& text);
    void drawPlayIcon(int x, int y);
    void setQuickControlMode(QuickControlMode mode);
    void showProgress(int value);

private:
    Adafruit_SSD1306 display;
    BluetoothManager& bluetoothManager;
    WifiManager& wifiManager;
    TimeManager& timeManager;
    int screenWidth;
    int screenHeight;
    int scrollPosition = 0;
    unsigned long lastScrollTime = 0;
    const int SCROLL_DELAY = 50;  // Scroll speed in milliseconds
    const int SCROLL_STEP = 1;    // Pixels to move per step
    QuickControlMode currentQuickMode = QuickControlMode::VOLUME;
    bool showingProgressBar = false;
    unsigned long progressBarStartTime = 0;
    int progressValue = 0;
    static const unsigned long PROGRESS_BAR_DURATION = 3000; // 3 seconds
};

#endif // DISPLAY_MANAGER_H