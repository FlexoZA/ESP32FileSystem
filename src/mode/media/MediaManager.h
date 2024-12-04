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
    void adjustVolume(int delta);
    int getVolume() const { return currentVolume; }
    void setVolume(int volume);

private:
    String currentText;
    bool isPlaying;
    BluetoothManager& bluetoothManager;
    unsigned long lastPreviousButtonPress;
    int previousButtonPressCount;
    static const unsigned long DOUBLE_PRESS_DELAY = 400; // ms between presses for double press
    int currentVolume = 30;  // Default volume at 30%
    static const int MIN_VOLUME = 0;
    static const int MAX_VOLUME = 100;
    unsigned long messageDisplayTime; // Time to revert to the current text
    bool isMessageDisplayed;           // Flag to indicate if a message is displayed
};

#endif