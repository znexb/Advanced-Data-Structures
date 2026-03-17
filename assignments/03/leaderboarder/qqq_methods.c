#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdbool.h>

#include "RBT.h"
#include "player_list.h"


int str_to_short(const char* str, signed short* out) {
    char* endptr;
    errno = 0;

    long value = strtol(str, &endptr, 10);

    // Check: no digits found
    if (endptr == str) return -1;

    // Check: extra characters
    if (*endptr != '\0') return -2;

    // Check: overflow from strtol
    if (errno == ERANGE) return -3;

    // Check: fits in signed short
    if (value < SHRT_MIN || value > SHRT_MAX) return -4;

    *out = (signed short)value;
    return 0;
} // Another AI generated function. The scope of this project is not string programming.

void print_status_(char* msg) { ln(); printf("%s\n", msg); ln(); }

void print_status_pnf_() { print_status_("Player not found!"); }


void add_player_score(tree* lb, player_list* pl, char* name, char* value) {
    if (find_player(pl, name) != -1) { print_status_("Player already exists!"); return; }

    signed short score;
    if (str_to_short(value, &score)) { print_status_("Invalid score value!"); return; }

    insert(lb, name, score);
    add_to_player_list(pl, name, score);
}


void update_player_score(tree* lb, player_list* pl, char* name, char* value) {
    signed short delta;
    if (str_to_short(value, &delta)) { print_status_("Invalid delta value!"); return; }
    
    char* player_name;
    signed short player_score;
    if(get_index(pl, name, &player_name, &player_score, print_status_pnf_)) { print_status_("Player not found in player list!"); return; }
    
    signed short new_score = player_score + delta;

    node* player_node = search(*lb, player_score);
    if(!player_node) { print_status_pnf_(); return; }

    delete(lb, player_node);
    insert(lb, player_name, new_score);
    update_score_player_list(pl, player_name, new_score);
}


void remove_player(tree* lb, player_list* pl, char* name) {
    char* player_name;
    signed short player_score;
    if(get_index(pl, name, &player_name, &player_score, print_status_pnf_)) { print_status_("Player not found in player list!"); return; }

    node* player_node = search(*lb, player_score);
    if(!player_node) { print_status_pnf_(); return; }

    delete(lb, player_node);
    remove_name_player_list(pl, player_name);
}


void top_k_(player_list** t, tree* lb, player_list* pl, char* value) {
    signed short k;
    if (str_to_short(value, &k)) { print_status_("Invalid score value!"); return; }

    if (k <= 0 || k > pl->index) { print_status_("K is invalid!"); return; }

    node *n = max(lb->rt);
    player_list *top = create_player_list();
    for (size_t i = 0; i < k; i++) {
        add_to_player_list(top, n->nam, n->key);
        n = predecessor(n);
    }
    
    int i = 0; // Out of bounds check
    while (t[i]->index != 0) ++i;
    free_player_list(t[i]);
    t[i] = top;
}


void print_tops(player_list** pll) {
    int i = 0;
    while (pll[i]->index != 0) {
        print_player_list(pll[i]);
        printf("\n");
        i++;
    }
}