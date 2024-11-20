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

void DisplayManager::updateScrollingText(const String& text) {
    int16_t x1, y1;
    uint16_t w, h;
    
    // Measure the text width and height
    display.setTextSize(1);
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    
    // Only scroll if enough time has passed
    if (millis() - lastScrollTime >= SCROLL_DELAY) {
        scrollPosition--;
        
        // Reset position when text has scrolled one full cycle
        if (scrollPosition < -(w + screenWidth)) {  // text width + screen width
            scrollPosition = 0;
        }
        
        lastScrollTime = millis();
    }

    // Set text color to ensure proper drawing
    display.setTextColor(SSD1306_WHITE);
    
    // Create a much larger filled black rectangle as background
    display.fillRect(0, 50, screenWidth, 16, SSD1306_BLACK);
    
    // Draw two copies of the text for seamless scrolling
    display.setCursor(scrollPosition, 56);
    display.print(text);
    
    display.setCursor(scrollPosition + w + screenWidth, 56);  // Ensure no overlap
    display.print(text);
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
    display.setCursor(20, 24);
    display.print("14:30");

    // Play Icon - replacing the ">" text
    drawPlayIcon(0, 28);

    // Mode display 
    display.setTextSize(1);
    display.setCursor(90, 28);
    display.print("(LED)");

    // Bottom: Song Name
    updateScrollingText("Rick Astley: Never Gonna Give You Up");

    // Move display.display() here to update everything at once
    display.display();
}
