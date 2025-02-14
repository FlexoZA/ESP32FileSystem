#include "BluetoothManager.h"
#include <BLE2902.h>

// Updated HID Report Map for consumer control
const uint8_t BluetoothManager::MEDIA_KEYS_REPORT_MAP[] = {
    0x05, 0x0C,        // Usage Page (Consumer)
    0x09, 0x01,        // Usage (Consumer Control)
    0xA1, 0x01,        // Collection (Application)
    0x15, 0x00,        // Logical Minimum (0)
    0x25, 0x01,        // Logical Maximum (1)
    0x75, 0x01,        // Report Size (1)
    0x95, 0x08,        // Report Count (8)
    0x09, 0xE9,        // Usage (Volume Increment)
    0x09, 0xEA,        // Usage (Volume Decrement)
    0x09, 0xCD,        // Usage (Play/Pause)
    0x09, 0xB5,        // Usage (Next Track)
    0x09, 0xB6,        // Usage (Previous Track)
    0x09, 0xB7,        // Usage (Stop)
    0x09, 0xB8,        // Usage (Eject)
    0x09, 0xB9,        // Usage (Shuffle)
    0x81, 0x02,        // Input (Data, Variable, Absolute)
    0xC0              // End Collection
};

BluetoothManager::BluetoothManager() : 
    deviceConnected(false), 
    oldDeviceConnected(false),
    blinkState(false) {
    hid = nullptr;
    mediaControl = nullptr;
}

void BluetoothManager::begin() {
    // Initialize BLE Device with NimBLE
    BLEDevice::init("ESP32-Media-Controller");
    
    // Create BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(this);
    
    // Create HID Device
    hid = new BLEHIDDevice(pServer);
    
    // Set manufacturer name
    hid->manufacturer()->setValue("Espressif");
    
    // Set HID report map
    hid->reportMap((uint8_t*)MEDIA_KEYS_REPORT_MAP, sizeof(MEDIA_KEYS_REPORT_MAP));
    
    // Create media control characteristic with proper permissions
    mediaControl = hid->inputReport(1); // Report ID 1
    
    // Start the HID service
    hid->startServices();
    
    // Start advertising with proper parameters
    BLEAdvertising* pAdvertising = pServer->getAdvertising();
    pAdvertising->setAppearance(0x03C4); // HID Generic Media Controls
    pAdvertising->addServiceUUID(hid->hidService()->getUUID());
    
    // Set advertising parameters for better connectivity
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMaxPreferred(0x12);
    
    pAdvertising->start();
    
    Serial.println("Bluetooth HID Device ready");
}

void BluetoothManager::update() {
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // Give the Bluetooth stack time to get ready
        
        // Only restart advertising if we're still not connected
        if (!deviceConnected && pServer) {
            pServer->startAdvertising();
            Serial.println("Restarting advertising");
        }
        oldDeviceConnected = deviceConnected;
    }
    
    if (deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }
    
    // Update blink state
    if (!deviceConnected) {
        unsigned long currentTime = millis();
        if (currentTime - lastBlinkTime >= 1000) {
            blinkState = !blinkState;
            lastBlinkTime = currentTime;
        }
    } else {
        blinkState = true;
    }
}

void BluetoothManager::volumeUp() {
    if (!deviceConnected || !mediaControl) {
        Serial.println("Cannot send volume up - not connected");
        return;
    }
    
    try {
        uint8_t mediaReport = 0;
        bitSet(mediaReport, 0);  // Set first bit for Volume Up
        
        mediaControl->setValue(&mediaReport, 1);
        mediaControl->notify();
        delay(20);
        
        mediaReport = 0;
        mediaControl->setValue(&mediaReport, 1);
        mediaControl->notify();
        
        Serial.println("Volume Up command sent");
    } catch (...) {
        Serial.println("Error sending volume up command");
    }
}

void BluetoothManager::volumeDown() {
    if (!deviceConnected || !mediaControl) {
        Serial.println("Cannot send volume down - not connected");
        return;
    }
    
    try {
        uint8_t mediaReport = 0;
        bitSet(mediaReport, 1);  // Set second bit for Volume Down
        
        mediaControl->setValue(&mediaReport, 1);
        mediaControl->notify();
        delay(20);
        
        mediaReport = 0;
        mediaControl->setValue(&mediaReport, 1);
        mediaControl->notify();
        
        Serial.println("Volume Down command sent");
    } catch (...) {
        Serial.println("Error sending volume down command");
    }
}

void BluetoothManager::playPause() {
    if (!deviceConnected || !mediaControl) {
        Serial.println("Cannot send command - not connected");
        return;
    }
    
    try {
        // Media control report (7 bits)
        uint8_t mediaReport = 0;
        bitSet(mediaReport, 2); // Bit 2 is Play/Pause
        
        // Send the press
        mediaControl->setValue(&mediaReport, 1);
        mediaControl->notify();
        delay(50); // Wait a bit longer
        
        // Send the release
        mediaReport = 0;
        mediaControl->setValue(&mediaReport, 1);
        mediaControl->notify();
        
        Serial.println("Play/Pause command sent");
    } catch (...) {
        Serial.println("Error sending media command");
        // Don't disconnect here, just log the error
    }
}

void BluetoothManager::nextTrack() {
    if (!deviceConnected || !mediaControl) {
        Serial.println("Cannot send command - not connected");
        return;
    }
    
    try {
        // Media control report (7 bits)
        uint8_t mediaReport = 0;
        bitSet(mediaReport, 3); // Bit 3 is Next Track
        
        // Send the press
        mediaControl->setValue(&mediaReport, 1);
        mediaControl->notify();
        delay(50);
        
        // Send the release
        mediaReport = 0;
        mediaControl->setValue(&mediaReport, 1);
        mediaControl->notify();
        
        Serial.println("Next Track command sent");
    } catch (...) {
        Serial.println("Error sending next track command");
    }
}

void BluetoothManager::previousTrack() {
    if (!deviceConnected || !mediaControl) {
        Serial.println("Cannot send command - not connected");
        return;
    }
    
    try {
        uint8_t mediaReport = 0;
        bitSet(mediaReport, 4); // Bit 4 is Previous Track
        
        mediaControl->setValue(&mediaReport, 1);
        mediaControl->notify();
        delay(50);
        
        mediaReport = 0;
        mediaControl->setValue(&mediaReport, 1);
        mediaControl->notify();
        
        Serial.println("Previous Track command sent");
    } catch (...) {
        Serial.println("Error sending previous track command");
    }
}

void BluetoothManager::restartTrack() {
    // For most media players, sending previous track once when 
    // the track has been playing will restart it
    previousTrack();
}

bool BluetoothManager::isDeviceConnected() const {
    return deviceConnected;
}

bool BluetoothManager::getBlinkState() const {
    return blinkState;
}

void BluetoothManager::onConnect(BLEServer* pServer) {
    deviceConnected = true;
    blinkState = true;
    Serial.println("Device connected");
    
    // Wait a brief moment for the connection to stabilize
    delay(500);
    
    // Set initial volume
    setInitialVolume();
}

void BluetoothManager::setInitialVolume() {
    // First, mute the volume by sending multiple volume down commands
    for(int i = 0; i < 0; i++) {  // Assuming 50 steps is enough to reach minimum
        volumeDown();
        delay(20);  // Small delay between commands
    }
    
    // Then set to desired volume
    setVolumeToPercent(DEFAULT_VOLUME_PERCENT);
}

void BluetoothManager::setVolumeToPercent(uint8_t targetPercent) {
    if (!deviceConnected || !mediaControl) {
        Serial.println("Cannot set volume - not connected");
        return;
    }
    
    targetPercent = constrain(targetPercent, 0, 100);
    
    // Assuming 50 steps is full volume range
    int volumeSteps = (targetPercent * 0) / 100;
    
    Serial.print("Setting volume to approximately ");
    Serial.print(targetPercent);
    Serial.println("%");
    
    for(int i = 0; i < volumeSteps; i++) {
        volumeUp();
        delay(20);  // Small delay between commands
    }
}

void BluetoothManager::onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
    // Don't immediately restart advertising
    // Let the update() method handle it
}