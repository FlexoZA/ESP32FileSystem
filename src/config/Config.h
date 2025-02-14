#ifndef CONFIG_H
#define CONFIG_H

// Encoder Pin Configuration
#define ENCODER_CLK_PIN 27  // CLK GPIO pin for encoder
#define ENCODER_DT_PIN 14   // DT GPIO pin for encoder
#define ENCODER_SW_PIN 13   // SW GPIO pin for encoder

#define AD_KEYBOARD_PIN 33   // ADKeyboard analog input pin

#define RELAY 25 // HW-482 relay

// Timing Constants
#define DEBOUNCE_DELAY 80  // Debounce delay for button
//#define DOUBLE_PRESS_DELAY 400 // ms between presses for double press
#define PREVIOUS_BUTTON_DELAY 300 // Previous button press delay for double press

// DHT11 Pin Configuration
#define DHT_PIN 19

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

// WiFi Configuration
#define WIFI_SSID "Lab"
#define WIFI_PASSWORD "jason789"
#define WIFI_CONNECT_TIMEOUT 10000  // 10 seconds timeout for connection attempts
#define WIFI_BLINK_INTERVAL 1000    // Blink interval for WiFi status indicator

// Time Configuration
#define GMT_OFFSET_SEC 7200  // GMT+2 (2 hours * 3600 seconds)

// LED Configuration
#define LED_PIN 32  // Change this to match your actual pin connection
#define NUM_LEDS 16

// Quick Control Modes
enum class QuickControlMode {
    VOLUME,
    BRIGHTNESS,
    MODE_COUNT
};

#endif // CONFIG_H