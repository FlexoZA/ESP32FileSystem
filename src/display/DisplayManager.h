#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "../config/Config.h"
#include "../communication/bluetooth/BluetoothManager.h"
#include "../communication/wifi/WifiManager.h"
#include "../time/TimeManager.h"
#include "../mode/media/MediaManager.h"
#include "../mode/fan/FanManager.h"

class DisplayManager {
private:
    int screenWidth;
    int screenHeight;
    Adafruit_SSD1306 display;
    BluetoothManager& bluetoothManager;
    WifiManager& wifiManager;
    TimeManager& timeManager;
    MediaManager& mediaManager;
    //FanManager fanManager;
    int scrollPosition = 0;
    unsigned long lastScrollTime = 0;
    const int SCROLL_DELAY = 50;  // Scroll speed in milliseconds
    const int SCROLL_STEP = 1;    // Pixels to move per step
    QuickControlMode currentQuickMode = QuickControlMode::VOLUME;
    bool showingProgressBar = false;
    unsigned long progressBarStartTime = 0;
    int progressValue = 0;
    static const unsigned long PROGRESS_BAR_DURATION = 3000; // 3 seconds

public:
    DisplayManager(int width, int height, TwoWire *wire, BluetoothManager& btManager, WifiManager& wifiMgr, TimeManager& timeMgr, MediaManager& mediaMgr);
    void begin();
    void showProgress(int value);
    void drawDefaultScreen(float temperature, float humidity);
    void setQuickControlMode(QuickControlMode mode);
};

#endif // DISPLAY_MANAGER_H