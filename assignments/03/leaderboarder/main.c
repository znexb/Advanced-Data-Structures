#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "RBT.h"
#include "player_list.h"
#include "runtime_methods.h"
#include "qqq_methods.h"
#include "input.h"


void idt(size_t n) { for(size_t i = 0; i < n; ++i) printf("    "); }

void print_filler() { for(size_t i = 0; i < 12; ++i) printf("=#"); printf("\n"); }

void print_filler_n(size_t n) { for(size_t i = 0; i < n; ++i) print_filler(); }

void options() {
    ln();   printf("Options ::: ");                                         ln();   ln();
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
    printf("Options ::: ");                 ln();
    idt(1); printf("1. QQQ operations -- ADS assignment\n");
    idt(1); printf("2. Runtime app\n");
    idt(1); printf("Anything else will quit the program!\n");
    ln();
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


void console_runtime_app(tree* lb, player_list* pl) {
    options();
    idt(3); printf("Enter a number: "); ln();
    while (true) {
        short option;
        scanf("%hd", &option);
        switch (option) {
            case 1: {
                add(lb, pl);
                break;
            } case 2: {
                update(lb, pl);
                break;
            } case 3: {
                _remove(lb, pl);
                break;
            } case 4: {
                top_k(lb, pl);
                break;
            } case 1337: {
                print_player_list(pl);
                break;
            } default: {
                quit();
                return;
                break;
            }
        }
    }
}

void qqq_app(tree* lb, player_list* pl) {
    options();
    idt(3); printf("Enter operations! Example: \"ADD John 42\"");   ln();
    idt(3); printf("Enter END to stop and print results");          ln();
    ln();
    player_list** tops = create_player_list_list();
    while (true) {
        char line[256];
        fgets(line, sizeof(line), stdin);
        int count = 0;

        line[strcspn(line, "\n")] = '\0';
        char** tokens = split(line, " ", &count);

        // for(int i = 0; i < count; i++) { printf("%s\n", tokens[i]); }

        if (!strcmp(tokens[0], "END")) {
            print_tops(tops);
            free_player_list_list(tops);
            return;
        } if (!strcmp(tokens[0], "ADD")) {
            add_player_score(lb, pl, tokens[1], tokens[2]);
        } if (!strcmp(tokens[0], "UPDATE")) {
            update_player_score(lb, pl, tokens[1], tokens[2]);
        } if (!strcmp(tokens[0], "REMOVE")) {
            remove_player(lb, pl, tokens[1]);
        } if (!strcmp(tokens[0], "TOP")) {
            top_k_(tops, lb, pl, tokens[1]);
        } if (!strcmp(tokens[0], "DEBUG")) {
            print_player_list(pl);
        }
    }
}

int main() {
    tree* leaderboard = create_tree(NULL);
    player_list* player_list = create_player_list();
    debug_sample(leaderboard, player_list);
    while (true) {
        menu();
        short option;
        scanf("%hd", &option);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        switch (option) {
            case 1: {
                qqq_app(leaderboard, player_list);
                break;
            } case 2: {
                console_runtime_app(leaderboard, player_list);
                break;
            } default: {
                quit();
                return 0;
                break;
            }
        }
    }
    free(leaderboard);
    free_player_list(player_list);
    return 0;
}