#include "mode/media/MediaManager.h"

MediaManager::MediaManager(BluetoothManager& btManager) 
    : bluetoothManager(btManager), isPlaying(false) {
    currentText = "Media Player Ready";
}

void MediaManager::begin() {
    // Serial.println("Media Manager Started");
    // logTextDimensions(currentText);
}

void MediaManager::update() {
    // Future implementation for media controls
}

String MediaManager::getCurrentText() const {
    return currentText;
}

void MediaManager::togglePlayPause() {
    isPlaying = !isPlaying;
    
    // Send command via Bluetooth
    bluetoothManager.playPause();
    
    // Update display text
    if (isPlaying) {
        currentText = "▶ Playing";
    } else {
        currentText = "⏸ Paused";
    }
}

//void MediaManager::logTextDimensions(const String& text) {
    // Serial.print("Text received: ");
    // Serial.println(text);
    // Serial.flush();
//}