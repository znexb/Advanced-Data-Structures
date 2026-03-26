#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    signed short* keys;
    struct node** children;
    struct node* parent;
} node;

typedef struct tree {
    struct node* root;
} tree;


node* create_node(const signed short* keys, size_t key_count, node* const* children, size_t children_count, node* parent) {
    node* new_node = malloc(sizeof(*new_node));
    if (!new_node) { perror("malloc"); return NULL; }
    
    new_node->keys = NULL;
    new_node->children = NULL;
    new_node->parent = parent;

    if (key_count > 0) {
        new_node->keys = malloc(key_count * sizeof(*new_node->keys));
        if (!new_node->keys) { perror("malloc"); free(new_node); return NULL; }
        memcpy(new_node->keys, keys, key_count * sizeof(*keys));
    }

    if (children_count > 0) {
        new_node->children = malloc(children_count * sizeof(*new_node->children));
        if (!new_node->children) { perror("malloc"); free(new_node); free(new_node->keys); return NULL; }
        memcpy(new_node->children, children, children_count * sizeof(*children));
    }
    
    return new_node;
}

void free_node(node* node) { free(node->children); free(node->keys); free(node); }


tree* create_tree(node* node) {
    tree *new_tree = malloc(sizeof(tree));
    if (!new_tree) { perror("malloc"); return; }
    new_tree->root = node;
    return new_tree;
}

// free(tree); No need for free function lol


int main() {
    unsigned short order;
    scanf("%hu", order);

}