#include <Arduino.h>

#define BUTTON_PIN 10
static const int LEDS[] = { 8, 9 };
static const int LEDS_LENGTH = sizeof(LEDS) / sizeof(LEDS[0]);

int main(void) {
    init();

    for (int i = 0; i < LEDS_LENGTH; i++) {
        digitalWrite(LEDS[i], OUTPUT);
    }

    digitalWrite(BUTTON_PIN, INPUT_PULLUP);

    while (true) {

    }

    return 0;
}