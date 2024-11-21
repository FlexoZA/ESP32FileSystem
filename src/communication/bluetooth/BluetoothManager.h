#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <Arduino.h>

class BluetoothManager {
public:
    BluetoothManager();
    void begin();
    String getCurrentText();
    int getTextLength();
    
private:
    String currentText = "Now Playing - Rick Astley: Never Gonna Give You Up";  // Example text
    int textLength;
    void calculateTextLength();
};

#endif