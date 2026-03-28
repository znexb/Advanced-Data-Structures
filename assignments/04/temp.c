void split_leaf(tree* tree, node* leaf, signed short key) {
    size_t max_keys = leaf->max_children_count - 1;
    signed short* temp = malloc((max_keys + 1) * sizeof(signed short));

    load_keys(leaf, key, temp);

    size_t mid = (max_keys + 1) / 2;

    // create sibling
    node* sibling = create_empty_node(leaf->max_children_count, true, leaf->parent);

    // left node
    leaf->key_count = 0;
    for (size_t i = 0; i < mid; i++) {
        leaf->keys[i] = temp[i];
        leaf->key_count++;
    }

    // right node
    for (size_t i = mid; i < max_keys + 1; i++) {
        sibling->keys[sibling->key_count++] = temp[i];
    }

    signed short promoted = sibling->keys[0];

    // if root
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