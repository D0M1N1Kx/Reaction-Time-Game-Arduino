#ifndef REACTION_TIME_GAME_ARDUINO_REACTION_RECORD_H
#define REACTION_TIME_GAME_ARDUINO_REACTION_RECORD_H

#include <time.h>

typedef struct {
    unsigned long reactionTime;
    time_t timestamp;
} ReactionRecord;

#endif
