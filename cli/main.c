#include <stdio.h>
#include "player.h"
#include "reaction_record.h"

#define MAX_PLAYERS 10

int loadPlayers(Player players[], int *playerCount) {
    FILE *file = fopen("players_data.dat", "rb");

    if (file == NULL) {
        *playerCount = 0;
        return 0;
    }

    fread(playerCount, sizeof(int), 1, file);
    fread(players, sizeof(Player), *playerCount, file);
    fclose(file);

    return 1;
}

void savePlayers(Player players[], int playerCount) {
    FILE *file = fopen("players_data.dat", "wb");

    if (file == NULL) {
        printf("Error when open file!\n");
        return;
    }

    fwrite(&playerCount, sizeof(int), 1, file);
    fwrite(players, sizeof(Player), playerCount, file);
    fclose(file);

    printf("Datas saved successfully!\n");
}

int main() {
    Player players[MAX_PLAYERS];
    int playerCount = 0;

    loadPlayers(players, &playerCount);

    return 0;
}