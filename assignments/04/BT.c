#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    if (!new_tree) { perror("malloc"); return NULL; }
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
    return out;
}

void insert_into_parent(tree* t, node* parent, node* left, node* right, signed short mid_key) {
    if (parent->key_count < parent->max_children_count - 1) {
        size_t pos = 0;
        while (pos < parent->key_count && parent->children[pos] != left) pos++;

        for (size_t i = parent->key_count; i > pos; --i) parent->keys[i] = parent->keys[i - 1];
        for (size_t i = parent->key_count + 1; i > pos + 1; --i) parent->children[i] = parent->children[i - 1];

        parent->keys[pos] = mid_key;
        parent->children[pos + 1] = right;
        right->parent = parent;

        parent->key_count++;
    } else {
        size_t max_keys = parent->max_children_count - 1;
        
        signed short* temp_keys = malloc((max_keys + 1) * sizeof(signed short));
        node** temp_children = malloc((max_keys + 2) * sizeof(node*));
        
        size_t pos = 0;
        while (pos < parent->key_count && parent->children[pos] != left) pos++;
        
        size_t i = 0, j = 0;
        for (; i < pos; i++, j++) temp_keys[j] = parent->keys[i];
        temp_keys[j++] = mid_key;
        for (; i < parent->key_count; i++, j++) temp_keys[j] = parent->keys[i];
        
        i = 0, j = 0;
        for (; i <= pos; i++, j++) temp_children[j] = parent->children[i];
        temp_children[j++] = right;
        for (; i <= parent->key_count; i++, j++) temp_children[j] = parent->children[i];
        
        size_t mid_index = (max_keys + 1) / 2;
        signed short new_mid_key = temp_keys[mid_index];
        
        node* sibling = create_empty_node(parent->max_children_count, false, parent->parent);
        
        parent->key_count = 0;
        for (i = 0; i < mid_index; i++) {
            parent->keys[i] = temp_keys[i];
            parent->children[i] = temp_children[i];
            if (parent->children[i]) parent->children[i]->parent = parent;
            parent->key_count++;
        }
        parent->children[i] = temp_children[i];
        if (parent->children[i]) parent->children[i]->parent = parent;
        
        for (i = mid_index + 1, j = 0; i < max_keys + 1; i++, j++) {
            sibling->keys[j] = temp_keys[i];
            sibling->children[j] = temp_children[i];
            if (sibling->children[j]) sibling->children[j]->parent = sibling;
            sibling->key_count++;
        }
        sibling->children[j] = temp_children[i];
        if (sibling->children[j]) sibling->children[j]->parent = sibling;
        
        if (!parent->parent) {
            node* new_root = create_empty_node(parent->max_children_count, false, NULL);
            new_root->keys[0] = new_mid_key;
            new_root->children[0] = parent;
            new_root->children[1] = sibling;
            new_root->key_count = 1;

            parent->parent = new_root;
            sibling->parent = new_root;
            t->root = new_root;
        } else {
            insert_into_parent(t, parent->parent, parent, sibling, new_mid_key);
        }
        
        free(temp_keys);
        free(temp_children);
    }
}

void split_leaf(tree* tree, node* leaf, signed short key) {
    size_t max_keys = leaf->max_children_count - 1;
    signed short* temp = malloc((max_keys + 1) * sizeof(signed short));

    load_keys(leaf, key, temp);
    
    size_t mid = (max_keys + 1) / 2;
    signed short promoted = temp[mid];

    node* sibling = create_empty_node(leaf->max_children_count, true, leaf->parent);

    leaf->key_count = 0;
    for (size_t i = 0; i < mid; i++) {
        leaf->keys[i] = temp[i];
        leaf->key_count++;
    }

    for (size_t i = mid + 1; i < max_keys + 1; i++) {
        sibling->keys[sibling->key_count++] = temp[i];
    }

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
        insert_into_parent(tree, parent, leaf, sibling, promoted);
    }

    free(temp);
}

void insert(tree* tree, const signed short key) {
    if (!tree->root) { tree_null_err(); return; }

    size_t pos = 0;
    node* parent = NULL;
    node* leaf = find_insertion_leaf(tree->root, key, &pos, &parent);
    
    if (!leaf) {
        printf("Key already exists or tree error\n");
        return;
    }

    if (leaf->key_count < leaf->max_children_count - 1) {
        reorder_keys(leaf, pos);
        leaf->keys[pos] = key;
    } else {
        split_leaf(tree, leaf, key);
    }
}

node* get_predecessor_node(node* curr, size_t pos) {
    node* curr_child = curr->children[pos];
    while (!curr_child->is_leaf) curr_child = curr_child->children[curr_child->key_count];
    return curr_child;
}

size_t get_pos_in_parent(node* n) {
    node* p = n->parent;
    if (!p) return 0;
    for (size_t i = 0; i <= p->key_count; i++) {
        if (p->children[i] == n) return i;
    }
    return 0;
}

void fix_shortage(tree* t, node* curr) {
    if (curr == t->root) {
        if (curr->key_count == 0 && !curr->is_leaf) {
            t->root = curr->children[0];
            t->root->parent = NULL;
            free_node(curr);
        }
        return;
    }

    size_t min_k = (curr->max_children_count + 1) / 2 - 1;
    if (curr->key_count >= min_k) return;

    node* p = curr->parent;
    size_t pos_in_parent = get_pos_in_parent(curr);

    node* left_sibling = (pos_in_parent > 0) ? p->children[pos_in_parent - 1] : NULL;
    node* right_sibling = (pos_in_parent < p->key_count) ? p->children[pos_in_parent + 1] : NULL;

    if (left_sibling && left_sibling->key_count > min_k) {
        for (size_t i = curr->key_count; i > 0; i--) curr->keys[i] = curr->keys[i - 1];
        if (!curr->is_leaf) {
            for (size_t i = curr->key_count + 1; i > 0; i--) curr->children[i] = curr->children[i - 1];
        }
        
        curr->keys[0] = p->keys[pos_in_parent - 1];
        p->keys[pos_in_parent - 1] = left_sibling->keys[left_sibling->key_count - 1];
        
        if (!curr->is_leaf) {
            curr->children[0] = left_sibling->children[left_sibling->key_count];
            curr->children[0]->parent = curr;
        }
        curr->key_count++;
        left_sibling->key_count--;
        return;
    }

    if (right_sibling && right_sibling->key_count > min_k) {
        curr->keys[curr->key_count] = p->keys[pos_in_parent];
        p->keys[pos_in_parent] = right_sibling->keys[0];

        if (!curr->is_leaf) {
            curr->children[curr->key_count + 1] = right_sibling->children[0];
            curr->children[curr->key_count + 1]->parent = curr;
            for (size_t i = 1; i <= right_sibling->key_count; i++) right_sibling->children[i - 1] = right_sibling->children[i];
        }
        
        for (size_t i = 1; i < right_sibling->key_count; i++) right_sibling->keys[i - 1] = right_sibling->keys[i];
        
        curr->key_count++;
        right_sibling->key_count--;
        return;
    }

    if (left_sibling) {
        left_sibling->keys[left_sibling->key_count] = p->keys[pos_in_parent - 1];
        size_t j = left_sibling->key_count + 1;
        for (size_t i = 0; i < curr->key_count; i++, j++) {
            left_sibling->keys[j] = curr->keys[i];
            if (!curr->is_leaf) {
                left_sibling->children[j] = curr->children[i];
                left_sibling->children[j]->parent = left_sibling;
            }
        }
        if (!curr->is_leaf) {
            left_sibling->children[j] = curr->children[curr->key_count];
            left_sibling->children[j]->parent = left_sibling;
        }
        left_sibling->key_count += curr->key_count + 1;

        for (size_t i = pos_in_parent - 1; i < p->key_count - 1; i++) p->keys[i] = p->keys[i + 1];
        for (size_t i = pos_in_parent; i < p->key_count; i++) p->children[i] = p->children[i + 1];
        p->key_count--;
        
        free_node(curr);
        fix_shortage(t, p);
    } else if (right_sibling) {
        curr->keys[curr->key_count] = p->keys[pos_in_parent];
        size_t j = curr->key_count + 1;
        for (size_t i = 0; i < right_sibling->key_count; i++, j++) {
            curr->keys[j] = right_sibling->keys[i];
            if (!curr->is_leaf) {
                curr->children[j] = right_sibling->children[i];
                curr->children[j]->parent = curr;
            }
        }
        if (!curr->is_leaf) {
            curr->children[j] = right_sibling->children[right_sibling->key_count];
            curr->children[j]->parent = curr;
        }
        curr->key_count += right_sibling->key_count + 1;

        for (size_t i = pos_in_parent; i < p->key_count - 1; i++) p->keys[i] = p->keys[i + 1];
        for (size_t i = pos_in_parent + 1; i < p->key_count; i++) p->children[i] = p->children[i + 1];
        p->key_count--;

        free_node(right_sibling);
        fix_shortage(t, p);
    }
}

void delete_key_from_node(tree* t, node* curr, signed short key) {
    if (!curr) return;
    
    size_t pos = 0;
    while (pos < curr->key_count && curr->keys[pos] < key) pos++;

    if (pos < curr->key_count && curr->keys[pos] == key) {
        if (curr->is_leaf) {
            for (size_t i = pos; i < curr->key_count - 1; i++) curr->keys[i] = curr->keys[i + 1];
            curr->key_count--;
            fix_shortage(t, curr);
        } else {
            node* pred = get_predecessor_node(curr, pos);
            signed short pred_key = pred->keys[pred->key_count - 1];
            curr->keys[pos] = pred_key;
            delete_key_from_node(t, curr->children[pos], pred_key);
        }
    } else {
        if (curr->is_leaf) return;
        delete_key_from_node(t, curr->children[pos], key);
    }
}

void delete(tree* t, const signed short key) {
    if (!t->root) return;
    delete_key_from_node(t, t->root, key);
}

void print_tree(node* n, int level) {
    if (!n) return;
    for (size_t i = 0; i < n->key_count; i++) {
        if (!n->is_leaf) print_tree(n->children[i], level + 1);
        for (int k = 0; k < level; k++) printf("    ");
        printf("%d\n", n->keys[i]);
    }
    if (!n->is_leaf) print_tree(n->children[n->key_count], level + 1);
}

int main() {
    printf("B-Tree structural verification...\n");
    // Using order 3 (max children = 3, max keys = 2) to trigger splits easily
    size_t order = 3;
    node* root = create_empty_node(order, true, NULL);
    tree* t = create_tree(root);

    signed short keys_to_insert[] = {10, 20, 5, 6, 12, 30, 7, 17};
    int num_inserts = sizeof(keys_to_insert) / sizeof(keys_to_insert[0]);

    for (int i = 0; i < num_inserts; i++) {
        printf("\nINSERTING %d\n", keys_to_insert[i]);
        insert(t, keys_to_insert[i]);
        print_tree(t->root, 0);
        printf("----------------");
    }

    signed short keys_to_delete[] = {6, 12, 20, 10};
    int num_deletes = sizeof(keys_to_delete) / sizeof(keys_to_delete[0]);

    for (int i = 0; i < num_deletes; i++) {
        printf("\nDELETING %d\n", keys_to_delete[i]);
        delete(t, keys_to_delete[i]);
        print_tree(t->root, 0);
        printf("----------------");
    }

    printf("\nAll operations successul!\n");
    return 0;
}