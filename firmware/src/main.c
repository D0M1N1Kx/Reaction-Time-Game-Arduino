#include <Arduino.h>

#define BUTTON_PIN 10
static const int LEDS[] = { 8, 9 };
static const int LEDS_LENGTH = sizeof(LEDS) / sizeof(LEDS[0]);

typedef enum {
    WAIT_FOR_START,
    WAIT_FOR_STIMULUS,
    WAIT_FOR_REACTION,
    SHOW_RESULT
} GameState;

GameState currentState = WAIT_FOR_START;

int main(void) {
    init();

    for (int i = 0; i < LEDS_LENGTH; i++) {
        digitalWrite(LEDS[i], OUTPUT);
    }

    digitalWrite(BUTTON_PIN, INPUT_PULLUP);

    while (true) {
        switch (currentState) {
            case WAIT_FOR_START: {
                // WAIT UNTIL THE PLAYER PRESS THE BUTTON TO START
                break;
            }
            case WAIT_FOR_STIMULUS: {
                // GENERATE RANDOM TIME BETWEEN 2-5s
                // IF REACH THE GIVEN TIME THEN NEXT STATE
                break;
            }
            case WAIT_FOR_REACTION: {
                // START A TIMER AND WAIT FOR THE PLAYER TO PRESS THE BUTTON
                break;
            }
            case SHOW_RESULT: {
                // SEND THE RESULT TO THE CONNECTED PC ON USB
                break;
            }
            default:
                break;
        }
    }

    return 0;
}