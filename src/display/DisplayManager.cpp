#include "DisplayManager.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>

DisplayManager::DisplayManager(int width, int height, TwoWire *wire) 
    : screenWidth(width), screenHeight(height), 
      display(width, height, wire, OLED_RESET) {
}

void DisplayManager::begin() {
    if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    
    display.clearDisplay();
    display.display();
}

// WiFi icon (16x16 pixels)
const unsigned char WIFI_ICON[] PROGMEM = {
    0b00000000, 0b00000000,
    0b00011000, 0b00000000,
    0b01100110, 0b00000000,
    0b10000001, 0b10000000,
    0b01000010, 0b01000000,
    0b00100100, 0b00100000,
    0b00011000, 0b00000000,
    0b00000000, 0b00000000
};

// Bluetooth icon (16x16 pixels)
const unsigned char BLUETOOTH_ICON[] PROGMEM = {
    0b00011000, 0b00000000,
    0b00001100, 0b00000000,
    0b00000110, 0b00000000,
    0b01111111, 0b00000000,
    0b00111110, 0b00000000,
    0b01111111, 0b00000000,
    0b00011100, 0b00000000,
    0b00001000, 0b00000000
};

void DisplayManager::updateScrollingText(const String& text) {
    int16_t x1, y1;
    uint16_t w, h;
    
    // Measure the text width
    display.setTextSize(1);
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    
    // Only scroll if enough time has passed
    if (millis() - lastScrollTime >= SCROLL_DELAY) {
        scrollPosition--;
        
        // Reset position when text has scrolled completely off screen
        if (scrollPosition < -w) {
            scrollPosition = screenWidth;
        }
        
        lastScrollTime = millis();
    }

    // Create a filled black rectangle as background to hide overflow
    display.fillRect(0, 54, screenWidth, 10, SSD1306_BLACK);
    
    // Draw the scrolling text
    display.setCursor(scrollPosition, 55);
    display.print(text);
}

void DisplayManager::drawDefaultScreen() {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    // Top left: Temperature and Humidity
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("25.5C - 60%");

    // Bluetooth connection
    display.setTextSize(1);
    display.setCursor(80, 0); 
    display.print("bt");

    // WIFI connection
    display.setTextSize(1);
    display.setCursor(100, 0);
    display.print("wifi");

    // Center: Time in larger text
    display.setTextSize(2);
    display.setCursor(15, 22);
    display.print("14:30");

    // Mode display
    display.setTextSize(1);
    display.setCursor(90, 27);
    display.print("(LED)");

    // Bottom: Song Name
    updateScrollingText("Altimate trolls: Never Gonna Give You Up");

    display.display();
}
