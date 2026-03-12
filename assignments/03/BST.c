#include <stdio.h>
#include <stdlib.h>


typedef struct node {
    signed short key;   // Short determines low capacity. Range of signed short is [0, 65535].
    struct node *lft;   // Pointer to the left child.
    struct node *rgt;   // Pointer to the right child.
} node;

typedef struct tree {
    struct node *rt;    // Reference to the root of the tree
} tree;


#include <stdbool.h>
