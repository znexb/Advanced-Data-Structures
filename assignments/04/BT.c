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

node* create_empty_node(size_t max_children_count, bool is_leaf, node* parent) {
    node* new_node = malloc(sizeof(*new_node));
    if (!new_node) { perror("malloc"); return NULL; }

    new_node->keys = malloc((max_children_count - 1) * sizeof(signed short));
    new_node->children = is_leaf ? NULL : malloc(max_children_count * sizeof(node*));

    new_node->key_count = 0;
    new_node->max_children_count = max_children_count;
    new_node->parent = parent;
    new_node->is_leaf = is_leaf;

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


void tree_null_err() { printf("tree_null error"); }

node* search_node(node* node, const signed short key) {
    if (!node) return NULL;

    signed short* it = node->keys;
    size_t pos = 0;
    for (; it < node->keys + node->key_count; it++) {
        if      (key < *it)     break;
        else if (key == *it)    return node;
        else                    ++pos;
    }

    if (node->is_leaf) return NULL;

    if (!node->children)            return NULL;
    else if (!node->children[pos])  return NULL;
    else                            return search_node(node->children[pos], key);
}

node* search(tree tree, const signed short key) {
    if (!tree.root) { tree_null_err(); return NULL; }

    return search_node(tree.root, key);
}


node* find_insertion_leaf(node* current, const signed short key, size_t* pos, node** parent) {
    if (!current) return NULL;

    *pos = 0;
    while (*pos < current->key_count) {
        if (key < current->keys[*pos]) break;
        if (key == current->keys[*pos]) return NULL;
        ++(*pos);
    }

    if (current->is_leaf) return current;

    *parent = current;
    return find_insertion_leaf(current->children[*pos], key, pos, parent);
}

void reorder_keys(node* leaf, size_t pos) {
    for (size_t i = leaf->key_count; i > pos; --i) {
        leaf->keys[i] = leaf->keys[i - 1];
    }
    leaf->key_count++;
}

signed short* load_keys(node* leaf, const signed short key, signed short* out) {
    size_t i = 0, j = 0;
    while (i < leaf->key_count && leaf->keys[i] < key) out[j++] = leaf->keys[i++];
    out[j++] = key;
    while (i < leaf->key_count) out[j++] = leaf->keys[i++];
}

void insert_into_parent(node* parent, node* left, node* right, signed short mid_key) {
    size_t pos = 0;

    while (pos < parent->key_count && parent->children[pos] != left) pos++;

    for (size_t i = parent->key_count; i > pos; --i) parent->keys[i] = parent->keys[i - 1];

    for (size_t i = parent->key_count + 1; i > pos + 1; --i) parent->children[i] = parent->children[i - 1];

    parent->keys[pos] = mid_key;
    parent->children[pos + 1] = right;
    right->parent = parent;

    parent->key_count++;
}

void split_leaf(tree* tree, node* leaf, signed short key) {
    size_t max_keys = leaf->max_children_count - 1;
    signed short* temp = malloc((max_keys + 1) * sizeof(signed short));

    load_keys(leaf, key, temp);
    
    size_t mid = (max_keys + 1) / 2;

    node* sibling = create_empty_node(leaf->max_children_count, true, leaf->parent);

    leaf->key_count = 0;
    for (size_t i = 0; i < mid; i++) {
        leaf->keys[i] = temp[i];
        leaf->key_count++;
    }

    for(size_t i = mid; i < max_keys + 1; i++) {
        sibling->keys[sibling->key_count++] = temp[i];
    }

    signed short promoted = sibling->keys[0];

    if (!leaf->parent) {
        node* new_root = create_empty_node(leaf->max_children_count, false, NULL);

        new_root->keys[0] = promoted;
        new_root->key_count = 1;
        new_root->children[0] = leaf;
        new_root->children[1] = sibling;

        leaf->parent = new_root;
        sibling->parent = new_root;
        tree->root = new_root;
    } else {
        node* parent = leaf->parent;
        insert_into_parent(parent, leaf, sibling, promoted);
    }

    free(temp);
}

void insert(tree* tree, const signed short key) {
    if (!tree->root) { tree_null_err(); return; }

    size_t pos = 0;
    node* parent = NULL;
    node* leaf = find_insertion_leaf(tree->root, key, &pos, &parent);
    if (leaf->key_count < leaf->max_children_count - 1) {
        reorder_keys(leaf, pos);
        leaf->keys[pos] = key;
    } else {

    }
}


int main() {
    unsigned short order;
    scanf("%hu", order);

}