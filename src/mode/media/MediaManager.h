#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#include <Arduino.h>

class MediaManager {
public:
    MediaManager();
    void begin();
    void update();
    String getCurrentText() const;
    void logTextDimensions(const String& text);

private:
    String currentText;
    void calculateTextDimensions();
};

#endif