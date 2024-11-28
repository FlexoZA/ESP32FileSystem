#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <Arduino.h>
#include "communication/bluetooth/BluetoothManager.h"
#include "config/Config.h"

class MediaManager {
public:
    MediaManager(BluetoothManager& btManager);
    void begin();
    void update();
    String getCurrentText() const;
    void togglePlayPause();
    void nextTrack();
    void handlePreviousButton();

private:
    String currentText;
    bool isPlaying;
    BluetoothManager& bluetoothManager;
    unsigned long lastPreviousButtonPress;
    int previousButtonPressCount;
    static const unsigned long DOUBLE_PRESS_DELAY = 400; // ms between presses for double press
};

#endif