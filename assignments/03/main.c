#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "RBT.h"
#include "player_list.h"


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


void add(tree* lb, player_list* pl) {
    ln();
    printf("Insert player name: ");
    char name[16];
    scanf("%15s", &name);
    printf("Insert player score: ");
    signed short score;
    scanf("%hd", &score);
    insert(lb, name, score); 
    printf("Added to RBT...\n");        // DBG
    add_to_player_list(pl, name, score);   
    printf("Added to pl...\n");         // DBG
    ln(); printf("Player added successfully!\n");
    ln();
}


void update(tree* lb, player_list* pl) {
    ln();
    printf("Insert player name: ");
    char name[16];
    scanf("%15s", &name);
    signed short player_index = find_player(pl, name);
    if(player_index == -1) { printf("Player not found!\n"); ln(); return; }
    char* player_name = pl->names[player_index];
    signed short player_score = pl->scores[player_index];
    printf("Insert delta: ");
    signed short delta;
    scanf("%hd", &delta);
    signed short new_score = player_score + delta;
    delete(lb, search(*lb, player_score));
    insert(lb, player_name, new_score);
    printf("Updated RBT...\n");        // DBG
    update_score_player_list(pl, player_name, new_score);
    printf("Updated pl...\n");         // DBG
    ln(); printf("Score update successful!\n");
    ln();
}


void _remove(tree* lb, player_list* pl) {
    ln();
    printf("Insert player name: ");
    char name[16];
    scanf("%15s", &name);
    signed short player_index = find_player(pl, name);
    if(player_index == -1) { printf("Player not found!\n"); ln(); return; }
    char* player_name = pl->names[player_index];
    signed short player_score = pl->scores[player_index];
    delete(lb, search(*lb, player_score));
    printf("Updated RBT...\n");        // DBG
    remove_name_player_list(pl, player_name);
    printf("Update pl...\n");          // DBG
    ln(); ln();
}


void top_k(tree* lb, player_list* pl) {
    ln();
    printf("Insert k: ");
    signed short k;
    scanf("%hd", &k);
    if (k <= 0) return;
    node* max_score = max(lb->rt);
    for (size_t i = 0; i < k; i++) {
        printf("%s ::: %d", max_score->nam, max_score->key);
        max_score = successor(max_score);
    }
    ln(); ln();
}


int main() {
    tree* leaderboard = create_tree(NULL);
    player_list* player_list = create_player_list();
                // insert(leaderboard, "John", 120);
                // add_to_player_list(player_list, "John", 120);
                // insert(leaderboard, "Doe", 100);
                // add_player_list(player_list, "Doe", 100);
    menu();
    while (true) {
        short option = 1;
        scanf("%hd", &option);
        switch (option) {
            case 1: {
                add(leaderboard, player_list);
                break;
            } case 2: {
                update(leaderboard, player_list);
                break;
            } case 3: {
                _remove(leaderboard, player_list);
                break;
            } case 4: {
                top_k(leaderboard, player_list);
                break;
            } case 1337: {
                print_player_list(player_list);
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