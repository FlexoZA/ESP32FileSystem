#include "MediaManager.h"

MediaManager::MediaManager() {
    currentText = "Now Playing - Rick Astley: Never Gonna Give You Up 12";
}

void MediaManager::begin() {
    delay(100);  // Give Serial a moment
    Serial.print("Testing Serial... ");  // Add this line
    Serial.flush();  // Force the output
    delay(100);  // Wait a bit
    Serial.println("Media Manager Started");
    Serial.flush();
    logTextDimensions(currentText);
}

void MediaManager::update() {
    // Future implementation for media controls
}

String MediaManager::getCurrentText() const {
    return currentText;
}

void MediaManager::logTextDimensions(const String& text) {
    Serial.print("Text received: ");
    Serial.println(text);
    Serial.flush();
}