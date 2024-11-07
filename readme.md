## ESP32 Menu navigation/modes

- ### modes
    - **led**
        - brightness (Adjust LED brightness)
        - colour schemes (Select LED colour scheme)
        - back (Back to previous menu)

    - **media**
        - volume (Adjust volume of connected bluetooth audio device)
    - **fan**
       - fan speed (Adjust fan speed)

## Folder structure

- ### src
    - main.cpp
    - **config**
        - Config.h
    - **display** (OLED 0.96" Display I2C Interface White SSD1315)
        - DisplayManager.h
        - DisplayManager.ppc
    - **input** (HW-040 Rotary encoder for menu navigation)
        - InputManager.h
        - InputManager.cpp
    - **mode**
        - ModeManager.h
        - ModeManager.cpp
        - **fan** (Control and save fan speed via rotary encoder)
            - FanManager.h
            - FanManager.cpp
        - **led** (ADAFRUIT Neopixel CJMCU-28128-16 16 LED's)
            - LEDManager.h
            - LEDManager.cpp
        - **media** (Bluetooth media control with previous/Play Pause/next buttons and volume control via rotary encoder)
            - MediaManager.h
            - MediaManager.cpp
    - **time** (Gets time from API via wifi)
        - TimeManager.h
        - TimeManager.cpp
    - **communication**
        - **wifi**
            - WifiManager.h
            - WifiManager.cpp
        - **Bluetooth**
            - BluetoothManager.h
            - BluetoothManager.cpp

## Pin layout
- **Encoder**
    - CLK GPIO27
    - DT GPIO14
    - SW GPIO113
    - VCC to 5V
    - GND to ground
- **Buttons**
    - BTN1 (Previous/Restart) - GPIO4
    - BTN2 (Pause/Play) - GPIO5
    - BTN3 (Skip/Next) - GPIO18
    - BTN4 (Quick mode change) - GPIO17
- **Humidity and temperature sensor**
    - vcc to 3.3V
    - GND to Ground
    - Data to - GPIO19

## Platform and requirements
```ini
platform = espressif32
board = esp32dev
framework = arduino
lib_deps = 
	adafruit/Adafruit NeoPixel@^1.12.3
	adafruit/Adafruit SSD1306@^2.5.13
	madhephaestus/ESP32Encoder@^0.11.7
	adafruit/DHT sensor library@^1.4.6
	adafruit/Adafruit Unified Sensor@^1.1.14
```

## How the menu navigation works

By default the local time and other info will be displayed on the screen. It will always return to this display after 3 seconds(change this time delay in gonfig.h) if there is no input from user.
to enter the mode select menu, press the encoder button (GPIO13) only a single mode will be displayed at a time IE Led, Media, Fan you can then rotate the encoder clockwise or anti clockwise to the desired mode. Press the encoder button again to enter the selected mode.

Selected mode (led)
After entering the led mode there is 3 options IE Brightness control, Color scheme and back. In brightness control (click encoder button) you can adjust the brightness of the LED regardless of what color scheme is selected the brightness works from 0 to 100% and a progress bar will be displayed on screen this value is stored by clicking the encoder button again after adjusting the brightness using the encoder turn. It then returns to the previous menu.

In color scheme you will have multiple options for the Neopixel 16led IE white, rainbow, blue, red.
After entering this mode the LED will change color scheme based on encoder turn. On the desired color scheme press the encoder button to store selected color scheme as default. If no input from user the led will change back to default color scheme and screen will go back to main screen.

Back will return to previous menu.

Selected mode (media)
After entering this mode it will go directly to volume control for the bluetooth media control. there is 3 more dedicated buttons for previous/restart double click for previous, play/pause, next. The volume is changed with turning the encoder and saved after a set amount of time if no input from user.

Selected mode (fan)
After selecting this mode it will go directly to fan speed control. Change the fan speed the fan speed will be set and returned to previous menu after a set amount of time if no user input.

When we are in the main screen when turning the encoder it will change the media control volume. it will auto save the volume level after user input.

## TODO
1: Get encoder working using ESP32Encoder library, Serial log the data
2: Make documentation on how the process works to get the encoder working
3: Start building menu structure with demo data, Serial log the data
4: Make documentation on integrating a file system (menu system)
5: Build out main screen (OLED integration)
6: Integrate Wifi and TimeManager - display results
7: Make documentation on OLED integration and how to pass data to the OLED
8: Build menu screens with demo data
9: Integrate LED and LED-inputs and LED-displays
10: Integrate temperature and humidity sensor - display results
11: Integrate Bluetooth and media controls - display result
12: Integrate fan controls - display results
13: Integrate quick change button to switch between LED and Media 