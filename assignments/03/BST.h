#ifndef BST_H
#define BST_H


#include <stdbool.h>

typedef struct node {
    signed short key;   // Short determines low capacity. Range of signed short is [0, 65535].
    struct node *lft;   // Pointer to the left child.
    struct node *rgt;   // Pointer to the right child.
} node;


bool search_found(node *nd, signed short key);

bool key_sm(signed short node_key, signed short key);

bool key_gt(signed short node_key, signed short key);


node* search_node(node *nd, signed short key);




#endif