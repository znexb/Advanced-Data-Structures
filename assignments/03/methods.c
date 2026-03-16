#include <stdlib.h>
#include <stdbool.h>


#include "RBT.h"
#include "player_list.h"


char* get_name(tree* lb, player_list* pl) {
    char* name = malloc(16);
    scanf("%15s", name);
    return name;
}

signed short get_score(tree* lb, player_list* pl) {
    signed short score;
    scanf("%hd", &score);
    return score;
}

void add_operations(tree* lb, player_list* pl, char* name, signed short score) {
    insert(lb, name, score); 
    printf("Added to RBT...\n");        // DBG
    add_to_player_list(pl, name, score);   
    printf("Added to pl...\n");         // DBG
}

void add(tree* lb, player_list* pl) {
    ln();
    
    printf("Insert player name: ");
    char* name = get_name(lb, pl);
    
    if (find_player(pl, name) != -1) { print_status("Player already exists!"); return; }

    printf("Insert player score: ");
    signed short score = get_score(lb, pl);

    add_operations(lb, pl, name, score);
    print_status("Player added successfully!");
    free(name);
}