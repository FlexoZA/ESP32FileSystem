#include "DisplayManager.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>

DisplayManager::DisplayManager(int width, int height, TwoWire *wire, BluetoothManager& btManager, WifiManager& wifiMgr, TimeManager& timeMgr) 
    : screenWidth(width), 
      screenHeight(height), 
      display(width, height, wire, OLED_RESET),
      bluetoothManager(btManager),
      wifiManager(wifiMgr),
      timeManager(timeMgr) {  // Initialize the reference
}

void DisplayManager::begin() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    
    display.clearDisplay();
    display.display();
}

void DisplayManager::updateScrollingText(const String& text) {
    static unsigned long lastUpdate = 0;
    unsigned long currentTime = millis();
    
    // Get text width for the current font size
    display.setTextSize(1);
    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    display.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &textHeight);

    // Only scroll if text is wider than screen
    if (textWidth > screenWidth) {
        if (currentTime - lastUpdate >= SCROLL_DELAY) {
            scrollPosition++;
            // Reset position when text has scrolled completely
            if (scrollPosition >= textWidth + screenWidth) {
                scrollPosition = -screenWidth;
            }
            lastUpdate = currentTime;
        }
        
        display.setTextSize(1);
        display.setCursor(-scrollPosition, 54);  // Bottom position
        display.print(text);
    } else {
        // Center the text if it's shorter than screen width
        display.setTextSize(1);
        int16_t centerX = (screenWidth - textWidth) / 2;
        display.setCursor(centerX, 54);
        display.print(text);
    }
}

void DisplayManager::drawPlayIcon(int x, int y) {
    int x1 = x;
    int y1 = y;
    int x2 = x;
    int y2 = y + 8;    // Height matches text size 1
    int x3 = x + 6;    // Width slightly less than height
    int y3 = y + 4;    // Midpoint of the height

    display.fillTriangle(x1, y1, x2, y2, x3, y3, SSD1306_WHITE);
}

void DisplayManager::showProgress(int value) {
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
    //display.print(20); // Remove if sensor is in
    display.print("C - ");
    display.print((int)humidity);
    //display.print((int) 20); // Remove if sensor is in
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

    // Play Icon
    drawPlayIcon(0, 28);

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

    // Bottom: Song Name
    updateScrollingText("Short text and then there is some longer test to perform");

    display.display();
}

void DisplayManager::setQuickControlMode(QuickControlMode mode) {
    currentQuickMode = mode;
}