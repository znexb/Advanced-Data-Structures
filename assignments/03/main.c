#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "RBT.h"
#include "player_list.h"
#include "methods.h"


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


void debug_sample(tree* leaderboard, player_list* player_list) {
    insert(leaderboard, "John", 120);
    add_to_player_list(player_list, "John", 120);
    insert(leaderboard, "Doe", 100);
    add_to_player_list(player_list, "Doe", 100);
    insert(leaderboard, "Costi", 200);
    add_to_player_list(player_list, "Costi", 200);
}


int main() {
    tree* leaderboard = create_tree(NULL);
    player_list* player_list = create_player_list();
    debug_sample(leaderboard, player_list);
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