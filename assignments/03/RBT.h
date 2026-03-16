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


bool key_sm (signed short left_key, signed short right_key);


tree* create_tree (node *rt);

node* create_node (signed short key, node *pnt);

void insert_node (bool dir, node *c, node *n);

void rotate_left (tree *t, node *n);

void rotate_right (tree *t, node *n);

void insert_fixup (tree *t, node *n);

void insert (tree *t, char *name, signed short key);


node* min (node* n);

node* successor (node* n);

void delete_fixup (tree* t, node* n, node* g);

node* delete (tree *t, node *n);


bool search_found (node* n, signed short key);

bool is_tree_null (tree t);

node* search_node (node* n, signed short key);

node* search (tree t, signed short key);


node* max (node *n);


#endif