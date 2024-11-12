#include "ModeManager.h"

ModeManager::ModeManager() {
    currentState = MenuState::MAIN_DISPLAY;
    currentMainMenuItem = MainMenuItem::FAN;
    currentLEDMenuItem = LEDMenuItem::BRIGHTNESS;
    lastEncoderValue = 0;
    currentMenuIndex = 0;
}

void ModeManager::begin() {
    Serial.println("Mode Manager Initialized");
    Serial.println("Current Menu Item: FAN");  // Initial state
}

void ModeManager::update(int encoderValue, bool buttonPressed) {
    // Handle encoder rotation
    if (encoderValue != lastEncoderValue) {
        int difference = encoderValue - lastEncoderValue;
        
        switch (currentState) {
            case MenuState::MENU_SELECT:
                handleMainMenu(difference);
                break;
            case MenuState::LED_MENU:
                handleLEDMenu(difference);
                break;
            default:
                // In main display, do nothing for now
                break;
        }
        
        lastEncoderValue = encoderValue;
    }
    
    // Handle button press
    if (buttonPressed) {
        switch (currentState) {
            case MenuState::MAIN_DISPLAY:
                currentState = MenuState::MENU_SELECT;
                Serial.println("Entering Menu Selection Mode");
                Serial.print("Current Menu Item: ");
                switch (currentMainMenuItem) {
                    case MainMenuItem::FAN:
                        Serial.println("FAN");
                        break;
                    case MainMenuItem::LED:
                        Serial.println("LED");
                        break;
                    case MainMenuItem::MEDIA:
                        Serial.println("MEDIA");
                        break;
                    default:
                        break;
                }
                break;
                
            case MenuState::MENU_SELECT:
                switch (currentMainMenuItem) {
                    case MainMenuItem::FAN:
                        currentState = MenuState::FAN_MENU;
                        Serial.println("Entering Fan Menu");
                        break;
                    case MainMenuItem::LED:
                        currentState = MenuState::LED_MENU;
                        Serial.println("Entering LED Menu");
                        break;
                    case MainMenuItem::MEDIA:
                        currentState = MenuState::MEDIA_MENU;
                        Serial.println("Entering Media Menu");
                        break;
                    default:
                        break;
                }
                break;
                
            case MenuState::LED_MENU:
                if (currentLEDMenuItem == LEDMenuItem::BACK) {
                    currentState = MenuState::MENU_SELECT;
                    Serial.println("Returning to Main Menu");
                }
                break;
                
            default:
                currentState = MenuState::MENU_SELECT;
                Serial.println("Returning to Main Menu");
                break;
        }
    }
}

void ModeManager::handleMainMenu(int difference) {
    // Get current index
    int currentIndex = static_cast<int>(currentMainMenuItem);
    
    // Calculate new index
    currentIndex += (difference > 0) ? 1 : -1;
    
    // Wrap around
    if (currentIndex < 0) {
        currentIndex = static_cast<int>(MainMenuItem::MENU_COUNT) - 1;
    } else if (currentIndex >= static_cast<int>(MainMenuItem::MENU_COUNT)) {
        currentIndex = 0;
    }
    
    // Update current menu item
    currentMainMenuItem = static_cast<MainMenuItem>(currentIndex);
    
    // Print current selection
    Serial.print("Current Menu Item: ");
    switch (currentMainMenuItem) {
        case MainMenuItem::FAN:
            Serial.println("FAN");
            break;
        case MainMenuItem::LED:
            Serial.println("LED");
            break;
        case MainMenuItem::MEDIA:
            Serial.println("MEDIA");
            break;
        default:
            break;
    }
}

void ModeManager::handleLEDMenu(int difference) {
    // Get current index
    int currentIndex = static_cast<int>(currentLEDMenuItem);
    
    // Calculate new index
    currentIndex += (difference > 0) ? 1 : -1;
    
    // Wrap around
    if (currentIndex < 0) {
        currentIndex = static_cast<int>(LEDMenuItem::MENU_COUNT) - 1;
    } else if (currentIndex >= static_cast<int>(LEDMenuItem::MENU_COUNT)) {
        currentIndex = 0;
    }
    
    // Update current menu item
    currentLEDMenuItem = static_cast<LEDMenuItem>(currentIndex);
    
    // Print current selection
    Serial.print("Current LED Menu Item: ");
    switch (currentLEDMenuItem) {
        case LEDMenuItem::BRIGHTNESS:
            Serial.println("BRIGHTNESS");
            break;
        case LEDMenuItem::COLOR_SCHEME:
            Serial.println("COLOR SCHEME");
            break;
        case LEDMenuItem::BACK:
            Serial.println("BACK");
            break;
        default:
            break;
    }
}
