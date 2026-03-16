#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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

void free_player_list(player_list* pl) {
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

void add_to_player_list(player_list* pl, char* n, signed short s) {
    if (!pl || !n) return;
    if (pl->index >= 16) return;
    signed short i = pl->index++;
    strncpy(pl->names[i], n, 15);
    pl->names[i][15] = '\0';
    pl->scores[i] = s;
}

signed short find_player(player_list* pl, char* n) {
    for (signed short i = 0; i < pl->index && i < 16; i++)
        if (!strcmp(n, pl->names[i])) return i;
    return -1;
}

void update_score_player_list(player_list *pl, char* n, signed short s) {
    if (!pl || !n) return;
    if (pl->index >= 16) return;
    signed short i = find_player(pl, n);
    if (i == -1) return;
    pl->scores[i] = s;
}

void remove_name_player_list(player_list* pl, char* n) {
    signed short id = find_player(pl, n);
    if (id == -1) return;
    for (size_t i = id; i < pl->index; i++) {
        pl->names[i]  = pl->names[i + 1];
        pl->scores[i] = pl->scores[i + 1]; 
    }
    pl->index--;
}

void ln() { printf("\n"); }

void print_player_list(player_list* pl) {
    ln();
    if (pl->index == 0) { printf("Empty set!\n"); return; }
    for (size_t i = 0; i < pl->index && pl->names[i]; i++) {
        printf("%s ::: %hd \n", pl->names[i], pl->scores[i]);
    }
    ln();
}