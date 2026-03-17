#ifndef RBT_H
#define RBT_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct node {
    signed short key;
    struct node *lft;
    struct node *rgt;
    struct node *pnt;
    bool col;    
} node;

typedef struct tree {
    struct node *rt;
} tree;

tree* create_tree (node* root);

void insert (tree* t, signed short key);

node* delete (tree *t, node *n);

node* search (tree t, signed short key);

void parse_tree (tree t, node **nodes, size_t *size);

#endif