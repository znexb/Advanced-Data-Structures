#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#include "RBT.h"
#include "player_list.h"


void print_status(char* msg) { ln(); printf("%s\n", msg); ln(); }

void print_status_pnf() { print_status("Player not found!"); }

char* get_name() {
    char* name = malloc(16);
    scanf("%15s", name);
    return name;
}

signed short get_score() {
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
    char* name = get_name();
    
    if (find_player(pl, name) != -1) { print_status("Player already exists!"); return; }

    printf("Insert player score: ");
    signed short score = get_score();

    add_operations(lb, pl, name, score);
    print_status("Player added successfully!");
    free(name);
}


signed short get_delta(signed short p_score) {
    signed short delta = get_score();
    return delta + p_score;
}

int update_operations(tree* lb, player_list* pl, char* p_name, signed short p_score, signed short new_score) {
    node* player_node = search(*lb, p_score);
    if(!player_node) { print_status_pnf(); return -1; }
    delete(lb, player_node);
    insert(lb, p_name, new_score);
    printf("Updated RBT...\n");        // DBG
    update_score_player_list(pl, p_name, new_score);
    printf("Updated pl...\n");         // DBG
    return 0;
}

void update(tree* lb, player_list* pl) {
    ln();

    printf("Insert player name: ");
    char* name = get_name();
    
    char* player_name;
    signed short player_score;
    if(get_index(pl, name, &player_name, &player_score, print_status_pnf)) { print_status("Player not found in player list!"); return; }

    printf("Insert delta: ");
    signed short new_score = get_delta(player_score);

    if(update_operations(lb, pl, player_name, player_score, new_score)) return;
    
    print_status("Score update successful!");
}


int _remove_operations(tree* lb, player_list* pl, char* p_name, signed short p_score) {
    node* player_node = search(*lb, p_score);
    if(!player_node) { print_status_pnf(); return -1; }
    delete(lb, player_node);
    printf("Updated RBT...\n");        // DBG
    remove_name_player_list(pl, p_name);
    printf("Update pl...\n");          // DBG
    return 0;
}

void _remove(tree* lb, player_list* pl) {
    ln();

    printf("Insert player name: ");
    char* name = get_name();

    char* player_name;
    signed short player_score;
    if(get_index(pl, name, &player_name, &player_score, print_status_pnf)) return;

    if(_remove_operations(lb, pl, player_name, player_score)) return;
    
    print_status("Player removed successfully!");
}



void print_top_k(tree* lb, signed short k) {
    node* n = max(lb->rt);
    for (size_t i = 0; i < k; i++) {
        printf("%s ::: %d\n", n->nam, n->key);
        n = predecessor(n);
    }
}

void top_k(tree* lb, player_list* pl) {
    ln();

    printf("Insert k: ");
    signed short k = get_score();

    if (k <= 0 || k > pl->index) { print_status("K is invalid!"); return; }
    
    print_top_k(lb, k);

    ln();
}