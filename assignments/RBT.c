#include <stdio.h>
#include <stdlib.h>


#include <stdbool.h>

typedef struct node {
    signed short key;
    struct node *lft;
    struct node *rgt;
    bool col; // Bool with the following specification: RED == TRUE & BLACK == FALSE;
} node;

typedef struct tree {
    struct node *rt;
} tree;


// Search

bool search_found(node *n, signed short key) { return n->key == key; }

// n_key = NODE key, s_key = SEARCHED key
bool key_sm(signed short n_key, signed short s_key) { return s_key < n_key; }

node* search_node(node *n, signed short key) {
    if(!n) return NULL;

    if(search_found(n, key)) return n;
    else {
        signed short cur = n->key;
        if(key_sm(cur, key)) {
            if(n->lft) return search_node(n->lft, key);
            else       return NULL;
        } else { // Implied else is key >= cur. The case key == cur can never occur because of the search_found check. Thus, we are left with key > cur.
            if(n->rgt) return search_node(n->rgt, key);
            else       return NULL;
        }
    }
}

node* search(tree bst, signed short key) {
    if(bst.rt == NULL) return NULL;

    return search_node(bst.rt, key);
}


// Print

void printn(node *n) { printf("%p = %d\n", n, n->key); }

void printn_null() { printf("ERROR ::: Node is null!\n"); }

// void print_search(tree t, signed short key) {
//     node *n = search(t, key);
//     if(n) printn(n);
//     else printn_null();
// }
// Trashed this version because: v2 avoids unnecessary 'else' + v2 keeps control flow flatter + v2 scales better in larger functions

void print_search(tree t, signed short key) {
    node *n = search(t, key);
    if(n) { printn(n); return; }
    printn_null();
}


void endl() { printf("\n"); }

int main() {
    // RBT_sample.jpg
    node n8 = {150, NULL, NULL, true};
    node n7 = {110, NULL, NULL, true};
    node n6 = {145, &n7, &n8, false};
    node n5 = {76, NULL, NULL, false};
    node n4 = {32, NULL, NULL, true};
    node n3 = {21, NULL, &n4, false};
    node n2 = {60, &n3, &n5, true};
    node n1 = {100, &n2, &n6, false};
    tree rbt = {&n1};

    printf("Search\n");
    print_search(rbt, 150);

    endl();
    printf("Success!\n");
}