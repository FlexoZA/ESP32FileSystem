#include "FanManager.h"

void FanManager::animateFan() {
    // Clear the display
    display.clearDisplay();
    
    // Draw center circle for the fan
    display.drawCircle(fanCenterX, fanCenterY, 5, SSD1306_WHITE);
    
    // Draw 3 fan blades at 120-degree intervals
    drawFanBlade(rotationStep);
    drawFanBlade(rotationStep + 120);
    drawFanBlade(rotationStep + 240);
    
    // Update display
    display.display();
    
    // Increment rotation step
    rotationStep += 30;
    if (rotationStep >= 360) {
        rotationStep = 0;
    }
}

void FanManager::drawFanBlade(int angle) {
    // Calculate blade tip coordinates
    int x1 = fanCenterX + fanRadius * cos(radians(angle));
    int y1 = fanCenterY + fanRadius * sin(radians(angle));
    
    // Draw a thick line for the blade
    for (int thickness = 1; thickness <= 3; thickness++) {
        display.drawLine(fanCenterX, fanCenterY, x1, y1, SSD1306_WHITE);
    }
}
