#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node {
    signed short* keys;
    size_t key_count;

    struct node** children;
    size_t max_children_count;

    struct node* parent;
    bool is_leaf;
} node;

typedef struct tree {
    struct node* root;
} tree;


node* create_node(const signed short* keys, size_t key_count, node* const* children, size_t max_children_count, node* parent, bool is_leaf) {
    if (key_count > max_children_count - 1) { printf("node_creation_error\n"); return NULL; }
    
    node* new_node = malloc(sizeof(*new_node));
    if (!new_node) { perror("malloc"); return NULL; }
    
    new_node->keys = NULL;
    new_node->key_count = key_count;

    new_node->children = NULL;
    new_node->max_children_count = max_children_count;

    new_node->parent = parent;
    new_node->is_leaf = is_leaf;

    if (key_count > 0) {
        if (!keys) { printf("keys null\n"); free(new_node); return NULL; }

        new_node->keys = malloc(key_count * sizeof(*new_node->keys));
        if (!new_node->keys) { perror("malloc"); free(new_node); return NULL; }
        memcpy(new_node->keys, keys, key_count * sizeof(*keys));
    }

    if (!is_leaf) {
        if (!children) { printf("children null\n"); free(new_node->keys); free(new_node); return NULL; }

        size_t children_count = key_count + 1;
        new_node->children = malloc(children_count * sizeof(*new_node->children));
        if (!new_node->children) { perror("malloc"); free(new_node); free(new_node->keys); return NULL; }
        for (size_t i = 0; i < children_count; i++) {
            if (children[i]) children[i]->parent = new_node;
        }
        memcpy(new_node->children, children, children_count * sizeof(*children));
    } else { new_node->children = NULL; }

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


node* search_node(node* node, const signed short key) {
    if (!node) return NULL;

    signed short* it = node->keys;
    size_t pos = 0;
    for (; it < node->keys + node->key_count; it++) {
        if      (key < *it)     break;
        else if (key == *it)    return node;
        else                    ++pos;
    }

    if (!node->children)            return NULL;
    else if (!node->children[pos])  return NULL;
    else                            return search_node(node->children[pos], key);
}

node* search(tree tree, const signed short key) {
    if (!tree.root) { printf("tree_null error"); return NULL; }

    return search_node(tree.root, key);
}


int main() {
    unsigned short order;
    scanf("%hu", order);

}