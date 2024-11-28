#include "WifiManager.h"

WifiManager::WifiManager() : connected(false), blinkState(false), lastBlinkTime(0) {}

void WifiManager::begin() {
    // Serial.println("Connecting to WiFi...");
    // Serial.print("SSID: ");
    // Serial.println(WIFI_SSID);
    
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    unsigned long startAttemptTime = millis();
    
    while (WiFi.status() != WL_CONNECTED && 
           millis() - startAttemptTime < WIFI_CONNECT_TIMEOUT) {
        delay(500);
        // Serial.print(".");
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        connected = true;
        // Serial.println("\nWiFi connected!");
        // Serial.print("IP address: ");
        // Serial.println(WiFi.localIP());
    } else {
        connected = false;
        // Serial.println("\nWiFi connection failed!");
    }
}

void WifiManager::update() {
    connected = (WiFi.status() == WL_CONNECTED);
    
    if (!connected) {
        unsigned long currentTime = millis();
        if (currentTime - lastBlinkTime >= WIFI_BLINK_INTERVAL) {
            blinkState = !blinkState;
            lastBlinkTime = currentTime;
        }
    } else {
        blinkState = true;
    }
    
    if (!connected && WiFi.status() != WL_CONNECTED) {
        // Serial.println("WiFi connection lost. Attempting to reconnect...");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    }
}