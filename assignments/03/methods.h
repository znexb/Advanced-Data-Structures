#ifndef METHODS_H
#define METHODS_H


typedef struct node {
    signed short key;   // Short determines low capacity. Range of signed short is [0, 65535].
    char* nam;
    struct node *lft;   // Pointer to the left child.
    struct node *rgt;   // Pointer to the right child.
    struct node *pnt;
    bool col;
} node;

typedef struct tree {
    struct node *rt;
} tree;

typedef struct player_list {
    signed short index;
    char **names;
    signed short *scores;
} player_list;


void add (tree* lb, player_list* pl);


#endif