#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node {
    signed short* keys;
    struct node** children;
    struct node* parent;
} node;

typedef struct tree {
    struct node* root;
} tree;

node* create_node(const signed short* keys, size_t key_count, node* parent) {
    node* new_node = malloc(sizeof(*new_node));
    if (!new_node) {
        perror("malloc");
        return NULL;
    }

    new_node->keys = NULL;
    new_node->children = NULL;
    new_node->parent = parent;

    if (key_count > 0) {
        new_node->keys = malloc(key_count * sizeof(*new_node->keys));
        if (!new_node->keys) {
            perror("malloc");
            free(new_node);
            return NULL;
        }

        for (size_t i = 0; i < key_count; i++) {
            new_node->keys[i] = keys[i];
        }

        /* alternatively:
           memcpy(new_node->keys, keys, key_count * sizeof(*keys));
        */
    }

    return new_node;
}