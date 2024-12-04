#include "mode/media/MediaManager.h"

MediaManager::MediaManager(BluetoothManager& btManager) 
    : bluetoothManager(btManager), 
      isPlaying(false),
      lastPreviousButtonPress(0),
      previousButtonPressCount(0),
      currentVolume(30),
      messageDisplayTime(0),
      isMessageDisplayed(false) {
    currentText = "Media Player Ready";
}

void MediaManager::begin() {
    // Serial.println("Media Manager Started");
    // logTextDimensions(currentText);
}

void MediaManager::update() {
    unsigned long currentTime = millis();
    
    // Check if a temporary message is displayed
    if (isMessageDisplayed && (currentTime - messageDisplayTime >= 2000)) {
        isMessageDisplayed = false; // Reset the flag
        // Revert to the current play/pause state text
        currentText = isPlaying ? "Playing" : "Paused";
    }

    if (previousButtonPressCount > 0 && 
        (currentTime - lastPreviousButtonPress > DOUBLE_PRESS_DELAY)) {
        previousButtonPressCount = 0;
    }
}

String MediaManager::getCurrentText() const {
    return currentText;
}

void MediaManager::togglePlayPause() {
    static unsigned long lastToggleTime = 0;
    unsigned long currentTime = millis();
    
    // Add debounce protection
    if (currentTime - lastToggleTime < 300) { // 300ms debounce
        return;
    }
    lastToggleTime = currentTime;

    if (!bluetoothManager.isDeviceConnected()) {
        currentText = "Not Connected";
        return;
    }
    
    isPlaying = !isPlaying;
    
    // Update display text
    if (isPlaying) {
        currentText = "Playing";
    } else {
        currentText = "Paused";
    }
    
    // Send command
    bluetoothManager.playPause();
}

void MediaManager::nextTrack() {
    static unsigned long lastNextTime = 0;
    unsigned long currentTime = millis();
    
    // Add debounce protection
    if (currentTime - lastNextTime < 300) { // 300ms debounce
        return;
    }
    lastNextTime = currentTime;

    if (!bluetoothManager.isDeviceConnected()) {
        currentText = "Not Connected";
        return;
    }
    
    currentText = "Next Track";
    messageDisplayTime = millis(); // Set the time to revert
    isMessageDisplayed = true; // Indicate a message is displayed
    bluetoothManager.nextTrack();
}

void MediaManager::handlePreviousButton() {
    static unsigned long lastHandleTime = 0;
    unsigned long currentTime = millis();
    
    // Use DEBOUNCE_DELAY from Config.h
    if (currentTime - lastHandleTime < PREVIOUS_BUTTON_DELAY) {
        return;
    }
    lastHandleTime = currentTime;

    if (!bluetoothManager.isDeviceConnected()) {
        currentText = "Not Connected";
        return;
    }

    // Check if this is a second press within the double-press window
    if (currentTime - lastPreviousButtonPress < DOUBLE_PRESS_DELAY) {
        previousButtonPressCount++;
        if (previousButtonPressCount == 2) {
            currentText = "Previous Track";
            messageDisplayTime = millis(); // Set the time to revert
            isMessageDisplayed = true; // Indicate a message is displayed
            bluetoothManager.previousTrack();
            previousButtonPressCount = 0; // Reset counter
        }
    } else {
        // First press or too long since last press
        previousButtonPressCount = 1;
        currentText = "Restart Track";
        messageDisplayTime = millis(); // Set the time to revert
        isMessageDisplayed = true; // Indicate a message is displayed
        bluetoothManager.restartTrack();
    }
    
    lastPreviousButtonPress = currentTime;
}

void MediaManager::adjustVolume(int delta) {
    static unsigned long lastVolumeChange = 0;
    unsigned long currentTime = millis();
    if (currentTime - lastVolumeChange < 50) {
        return;
    }
    lastVolumeChange = currentTime;

    // Calculate new volume (5% per step)
    int newVolume = currentVolume + (delta * 5);  // 5% per tick
    newVolume = constrain(newVolume, MIN_VOLUME, MAX_VOLUME);
    
    if (newVolume != currentVolume) {
        // Determine direction
        if (delta > 0) {
            Serial.println("Volume Up");
            bluetoothManager.volumeUp();
        } else {
            Serial.println("Volume Down");
            bluetoothManager.volumeDown();
        }
        
        currentVolume = newVolume;
        Serial.printf("Current Volume: %d%%\n", currentVolume);
        delay(20);
    }
}

void MediaManager::setVolume(int volume) {
    // Constrain the input volume to valid range
    volume = constrain(volume, MIN_VOLUME, MAX_VOLUME);
    
    // Reset to 0 first
    for (int i = 0; i < 20; i++) {  // Reduced number of steps to avoid over-muting
        bluetoothManager.volumeDown();
        delay(20);
    }
    
    // Then set to desired level
    currentVolume = 0;
    int stepsNeeded = volume / 5;  // Since each step is 5%
    
    for (int i = 0; i < stepsNeeded; i++) {
        bluetoothManager.volumeUp();
        delay(20);
    }
    
    currentVolume = volume;
}

//void MediaManager::logTextDimensions(const String& text) {
    // Serial.print("Text received: ");
    // Serial.println(text);
    // Serial.flush();
//}