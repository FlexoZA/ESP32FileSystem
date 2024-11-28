#include "MediaManager.h"

MediaManager::MediaManager() {
    currentText = "Now Playing - Rick Astley: Never Gonna Give You Up";
}

void MediaManager::begin() {
    Serial.println("Media Manager Started");
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