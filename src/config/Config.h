#ifndef CONFIG_H
#define CONFIG_H

// Encoder Pin Configuration
#define ENCODER_CLK_PIN 27  // CLK GPIO pin for encoder
#define ENCODER_DT_PIN 14   // DT GPIO pin for encoder
#define ENCODER_SW_PIN 13   // SW GPIO pin for encoder

// Timing Constants
#define DEBOUNCE_DELAY 80  // Debounce delay for button

// DHT11 Pin Configuration
#define DHT_PIN 19

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

#endif // CONFIG_H