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
    int activePlayerIndex = -1;

    loadPlayers(players, &playerCount);

    int running = 1;
    while (running) {
        printf("\n--- REACTION TIME GAME ---\n");
        if (activePlayerIndex != -1) {
            printf("Activated player: %s\n", players[activePlayerIndex].name);
        } else {
            printf("No player selected!\n");
        }

        printf("1. Select or create player\n");
        printf("2. Start measurement (wait for USB)\n");
        printf("3. Check player's records\n");
        printf("4. Save and exit\n");
        printf("Choice: ");

        int choice;
        // VALIDATE CHOICE
        if (scanf("%d", &choice) != 1) {
            break;
        }

        switch (choice) {
            case 1: {
                printf("\nSaved Players\n");
                for (int i = 0; i < playerCount; i++) {
                    printf("[%d] %s (%d measurement)", i, players[i].name, players[i].scoreCount);
                }
                printf("Select player (write last player index + 1 to create): ");

                int pChoice;
                scanf("%d", &pChoice);

                if (pChoice == playerCount && playerCount < MAX_PLAYERS) {
                    printf("\nNew player name: ");
                    scanf("%39s", players[playerCount].name);
                    players[playerCount].scoreCount = 0;
                    activePlayerIndex = playerCount;
                    playerCount++;
                } else if (pChoice >= 0 && pChoice < playerCount) {
                    activePlayerIndex = pChoice;
                } else {
                    printf("Invalid choice!\n");
                }
                break;
            }
            case 2: {
                break;
            }
            case 3: {
                break;
            }
            case 4: {
                savePlayers(players, playerCount);
                running = 0;
                break;
            }
            default:
                printf("Not known option!\n");
        }
    }
    return 0;
}