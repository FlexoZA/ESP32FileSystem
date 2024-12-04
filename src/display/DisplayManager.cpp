#include "DisplayManager.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>

DisplayManager::DisplayManager(int width, int height, TwoWire *wire, BluetoothManager& btManager, WifiManager& wifiMgr, TimeManager& timeMgr, MediaManager& mediaMgr) 
    : screenWidth(width), 
      screenHeight(height), 
      display(width, height, wire, OLED_RESET),
      bluetoothManager(btManager),
      wifiManager(wifiMgr),
      timeManager(timeMgr),
      mediaManager(mediaMgr),
      fanManager(display) {  // Initialize the reference
}

void DisplayManager::begin() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    
    display.clearDisplay();
    display.display();
}

void DisplayManager::showProgress(int value) {
    Serial.printf("Progress set to: %d%%\n", value);  // Add debug print
    progressValue = constrain(value, 0, 100);
    showingProgressBar = true;
    progressBarStartTime = millis();
}

void DisplayManager::drawDefaultScreen(float temperature, float humidity) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    // Top left: Temperature and Humidity
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(temperature, 1);
    //display.print(20); // Remove if sensor is connected
    display.print("C - ");
    display.print((int)humidity);
    //display.print((int) 20); // Remove if sensor is connected
    display.print("%");

    // Bluetooth connection
    display.setTextSize(1);
    display.setCursor(80, 0); 
    if (bluetoothManager.isDeviceConnected()) {
        display.print("BT");
    } else if (bluetoothManager.getBlinkState()) {
        display.print("BT");
    }

    // WIFI connection
    display.setTextSize(1);
    display.setCursor(100, 0);
    if (wifiManager.isConnected()) {
        display.print("WIFI");
    } else if (wifiManager.getBlinkState()) {
        display.print("WIFI");
    }

    // Center: Either Time or Progress Bar
    unsigned long currentTime = millis();
    bool isShowingProgress = showingProgressBar && (currentTime - progressBarStartTime < PROGRESS_BAR_DURATION);

    if (isShowingProgress) {
        // Draw progress bar
        int barWidth = 88;
        int barHeight = 16;
        int barX = 20;
        int barY = 24;
        
        display.drawRect(barX, barY, barWidth, barHeight, SSD1306_WHITE);
        int fillWidth = (barWidth - 4) * progressValue / 100;
        display.fillRect(barX + 2, barY + 2, fillWidth, barHeight - 4, SSD1306_WHITE);
        
        display.setTextSize(1);
        display.setCursor(barX + (barWidth/2) - 10, barY + 4);
        display.print(progressValue);
        display.print("%");
    } else {
        // Draw time
        showingProgressBar = false;
        display.setTextSize(2);
        display.setCursor(20, 24);
        display.print(timeManager.getFormattedTime());
    }

    // Quick Control Mode indicator - only show if progress bar is not visible
    if (!isShowingProgress) {
        display.setCursor(90, 28);
        display.setTextSize(1);
        switch(currentQuickMode) {
            case QuickControlMode::VOLUME:
                display.print("[VOL]");
                break;
            case QuickControlMode::BRIGHTNESS:
                display.print("[BRI]");
                break;
        }
    }

    // Bottom: Media Status
    display.setTextSize(1);
    String mediaText = mediaManager.getCurrentText();
    int textWidth = mediaText.length() * 6; // Assuming each character is approximately 6 pixels wide
    int cursorX = (screenWidth - textWidth) / 2; // Center the text horizontally
    display.setCursor(cursorX, 52);  // Using the same y-position as before
    display.print(mediaText);

    display.display();
}

void DisplayManager::setQuickControlMode(QuickControlMode mode) {
    currentQuickMode = mode;
}