#ifndef BST_H
#define BST_H


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


tree* create_tree (node *rt);


void insert (tree *t, char *name, signed short key);

node* delete (tree* t, node *n);

node* search (tree t, signed short key);


node* max (node* n);

node* predecessor (node* n);


#endif