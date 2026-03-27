node* find_insertion_leaf(node* curr, signed short key, size_t* pos) {
    if (!curr) return NULL;

    *pos = 0;
    while (*pos < curr->key_count) {
        if (key < curr->keys[*pos]) {
            break;
        }
        if (key == curr->keys[*pos]) {
            return NULL; // duplicate key
        }
        ++(*pos);
    }

    if (curr->is_leaf) {
        return curr;
    }

    return find_insertion_leaf(curr->children[*pos], key, pos);
}