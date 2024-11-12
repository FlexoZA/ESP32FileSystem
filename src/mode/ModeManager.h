// ModeManager.h
#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include <Arduino.h>

enum class MenuState {
    MAIN_DISPLAY,    // Default state showing time
    MENU_SELECT,     // Menu selection mode
    LED_MENU,        // LED submenu
    FAN_MENU,        // Fan control
    MEDIA_MENU       // Media control
};

enum class MainMenuItem {
    FAN = 0,
    LED = 1,
    MEDIA = 2,
    MENU_COUNT = 3  // Helper to know total number of menu items
};

enum class LEDMenuItem {
    BRIGHTNESS = 0,
    COLOR_SCHEME = 1,
    BACK = 2,
    MENU_COUNT = 3  // Helper to know total number of menu items
};

class ModeManager {
private:
    MenuState currentState;
    MainMenuItem currentMainMenuItem;
    LEDMenuItem currentLEDMenuItem;
    int lastEncoderValue;
    int currentMenuIndex;  // Keep track of current position in numerical form
    
    void handleMainMenu(int difference);
    void handleLEDMenu(int difference);
    
public:
    ModeManager();
    void begin();
    void update(int encoderValue, bool buttonPressed);
    MenuState getCurrentState() { return currentState; }
};

#endif