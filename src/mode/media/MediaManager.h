#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <Arduino.h>
#include "communication/bluetooth/BluetoothManager.h"

class MediaManager {
public:
    MediaManager(BluetoothManager& btManager);
    void begin();
    void update();
    String getCurrentText() const;
    void togglePlayPause();

private:
    String currentText;
    bool isPlaying;
    BluetoothManager& bluetoothManager;
};

#endif