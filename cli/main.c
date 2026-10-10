#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
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

// AI GENERATED FUNCTION
int open_serial_port(const char *device_path) {
    int fd = open(device_path, O_RDONLY | O_NOCTTY);
    if (fd < 0) {
        return -1;
    }

    struct termios tty;
    if (tcgetattr(fd, &tty) != 0) {
        close(fd);
        return -1;
    }

    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        close(fd);
        return -1;
    }

    return fd;
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
                    printf("[%d] %s (%d measurement)\n", i, players[i].name, players[i].scoreCount);
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
            // CASE 2 IS AI GENERATED
            case 2: {
                if (activePlayerIndex == -1) {
                    printf("No active player!\n");
                    break;
                }

                printf("\nWait for Arduino (/dev/ttyACM0)...\n");

                int fd = open_serial_port("/dev/ttyACM0");
                if (fd < 0) {
                    printf("Error: Failed to open port.\n");
                    break;
                }

                usleep(2000000);

                char read_buf[256];
                int idx = 0;
                char ch;

                while (idx < sizeof(read_buf) - 1) {
                    int n = read(fd, &ch, 1);
                    if (n > 0) {
                        if (ch == '\n' || ch == '\r') {
                            if (idx > 0) break;
                            continue;
                        }
                        read_buf[idx++] = ch;
                    } else {
                        break;
                    }
                }
                read_buf[idx] = '\0';
                close(fd);

                if (idx > 0) {
                    char *p = read_buf;
                    while (*p && (*p < '0' || *p > '9')) {
                        p++;
                    }

                    unsigned long incomingTime = 0;
                    if (*p != '\0') {
                        incomingTime = strtoul(p, NULL, 10);
                    }

                    if (incomingTime > 0) {
                        int count = players[activePlayerIndex].scoreCount;
                        if (count < 50) {
                            players[activePlayerIndex].history[count].reactionTime = incomingTime;
                            players[activePlayerIndex].history[count].timestamp = time(NULL);
                            players[activePlayerIndex].scoreCount++;
                            printf("Success! Fixed for the player: %lu ms\n", incomingTime);
                        } else {
                            printf("The player's reaction record's list is full!\n");
                        }
                    } else {
                        printf("Invalid data: '%s'\n", read_buf);
                    }
                } else {
                    printf("Data not arrived from the port.\n");
                }
                break;
            }
            case 3: {
                if (activePlayerIndex == -1) {
                    printf("No active player!\n");
                    break;
                }
                Player *p = &players[activePlayerIndex];
                printf("\n--- %s results ---\n", p->name);
                for (int i = 0; i < p->scoreCount; i++) {
                    time_t t = p->history[i].timestamp;
                    struct tm *tm_info = localtime(&t);
                    char timeBuffer[26];
                    strftime(timeBuffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);

                    printf("%d. Time: %lu ms (%s)\n", i + 1, p->history[i].reactionTime, timeBuffer);
                }
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