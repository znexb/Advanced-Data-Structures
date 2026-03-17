#ifndef PLAYER_LIST_H
#define PLAYER_LIST_H


typedef struct player_list {
    signed short index;
    char **names;
    signed short *scores;
} player_list;


void ln();

player_list* create_player_list();

void free_player_list(player_list* pl);

player_list** create_player_list_list();

void free_player_list_list(player_list** pll);

void add_to_player_list(player_list* pl, char* n, signed short s);

signed short find_player(player_list* pl, char* n);

int get_index(player_list* pl, char* name, char** p_name, signed short *p_score, void (*print_status_pnf)(void));

void update_score_player_list(player_list* pl, char* n, signed short s);

void remove_name_player_list(player_list* pl, char* n);

void print_player_list(player_list* pl);


#endif