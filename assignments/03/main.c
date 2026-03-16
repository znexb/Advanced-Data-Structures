#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "RBT.h"


typedef struct player_list {
    signed short index;
    char **names;
    signed short *scores;
} player_list;

player_list* create_player_list() {
    player_list *pl = malloc (sizeof(player_list));
    if (!pl) { perror("malloc"); return NULL; }
    pl->index = 0;
    pl->names = malloc (16 * sizeof(char*));
    if (!pl->names) { perror("malloc"); free(pl); return NULL; }
    for (int i = 0; i < 16; i++) {
        pl->names[i] = malloc(16 * sizeof(char));
        if (!pl->names[i]) { 
            perror("malloc"); 
            for(int j = 0; j < 16; j++) free(pl->names[j]);
            free(pl->names);
            free(pl);
            return NULL; 
        }
    }
    pl->scores = malloc (16 * sizeof(signed short));
    if (!pl->scores) { 
        perror("malloc"); 
        for(int i = 0; i < 16; i++) free(pl->names[i]);
        free(pl->names);
        free(pl);
        return NULL; 
    }
    return pl;
}
// Honestly, I do not know why I bothered with manual malloc for each element. I guess I like pointless hard work. *POINTERless* work especially, haha

void free_player_list(player_list *pl) {
    if (!pl) return;
    if (pl->names) {
        for (int i = 0; i < 16; i++) {
            free(pl->names[i]);
        }
        free(pl->names);
    }
    free(pl->scores);
    free(pl);
}


void ln() { printf("\n"); }

void idt(size_t n) { for(size_t i = 0; i < n; ++i) printf("    "); }

void print_filler() { for(size_t i = 0; i < 12; ++i) printf("=#"); printf("\n"); }

void print_filler_n(size_t n) { for(size_t i = 0; i < n; ++i) print_filler(); }

void options() {
    printf("Options ::: ");                                         ln();   ln();
    idt(1); printf("1. ADD    {player} {score}");                           ln();
    idt(2); printf("- Add a new player with the given score.");             ln();
    idt(1); printf("2. UPDATE {player} {delta}");                           ln();
    idt(2); printf("- Increase or decrease the player's score by delta.");  ln();
    idt(1); printf("3. REMOVE {player}");                                   ln();
    idt(2); printf("- Remove the player from the leaderboard.");            ln();
    idt(1); printf("4. TOP    k");                                          ln();
    idt(2); printf("- Return the k players with the highest scores.");      ln();
}

void menu() {
    print_filler();
    printf("LIVE LEADERBOARDer --- C");     ln();
    print_filler_n(3);                      ln();
    options();                              ln();
}

void quit_anim() {
    usleep(250000); printf(".");
}

void quit() {
    printf("Quiting");
    for(size_t i = 0; i < 3; ++i) { quit_anim(); }
    ln();
    usleep(250000);
}

void update_player_list(player_list* pl, char* n, signed short s) {
    if (!pl || !n) return;
    if (pl->index >= 16) return;
    signed short i = pl->index++;
    strncpy(pl->names[i], n, 15);
    pl->names[i][15] = '\0';
    pl->scores[i] = s;
}

void add(tree* lb, player_list* pl) {
    ln();
    printf("Insert player name: ");
    char name[16];
    scanf("%15s", &name);
    ln();
    printf("Insert player score: ");
    signed short score;
    scanf("%hd", &score);
    insert(lb, name, score); 
    printf("Added to RBT...\n");        // DBG
    update_player_list(pl, name, score);   
    printf("Added to pl...\n");         // DBG
    ln(); printf("Player added successfully!\n");
    ln();
}


int main() {
    tree* leaderboard = create_tree(NULL);
    player_list* player_list = create_player_list();
    menu();
    while (true) {
        short option = 1;
        scanf("%hd", &option);
        switch (option) {
            case 1: {
                add(leaderboard, player_list);
                break;
            } case 2: {
                break;
            } case 3: {
                break;
            } case 4: {
                break;
            } default: {
                quit();
                return 1;
                break;
            }
        }
    }
    free(leaderboard);
    free_player_list(player_list);
    return 0;
}