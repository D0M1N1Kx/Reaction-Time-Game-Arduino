#ifndef REACTION_TIME_GAME_ARDUINO_PLAYER_H
#define REACTION_TIME_GAME_ARDUINO_PLAYER_H

#include "reaction_record.h"

typedef struct {
    char name[40];
    ReactionRecord history[50];
    int scoreCount;
} Player;

#endif
