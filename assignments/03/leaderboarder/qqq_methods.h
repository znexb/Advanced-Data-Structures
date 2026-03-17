#ifndef QQQ_METHODS_H
#define QQQ_METHODS_H


void add_player_score(tree* lb, player_list* pl, char* name, char* value);

void update_player_score(tree* lb, player_list* pl, char* name, char* value);

void remove_player(tree* lb, player_list* pl, char* name);

void top_k_(player_list** t, tree* lb, player_list* pl, char* value);

void print_tops(player_list** pll);


#endif