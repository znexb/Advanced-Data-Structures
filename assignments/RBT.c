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

bool is_tree_null(tree bst) { return !bst.rt; }

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
    if(is_tree_null(bst)) return NULL;
    return search_node(bst.rt, key);
}


// Minimum & Maximum

node* max(node *n) {
    if(n->rgt) return max(n->rgt);
    return n;
}

node* min(node *n) {
    if(n->lft) return min(n->lft);
    return n;
}

node* minmax(tree t, bool which) { // max == true & min == false
    if(is_tree_null(t)) return NULL;
    if(which) { return max(t.rt); }
    return min(t.rt);
}


// Insert

tree* create_tree(node *rt) {
    tree *t = malloc(sizeof(tree));
    if(!t) { perror("malloc"); return NULL; }
    t->rt = rt;
    return t;
}

node* create_node(signed short key) {
    node *n = malloc(sizeof(node));
    if(!n) { perror("malloc"); return NULL; }
    n->key = key;
    n->lft = NULL;
    n->rgt = NULL;
    n->col = true; // Red by default makes it insert friendly
}

void node_exists() { printf("Node already exists!\n"); }

void insert_node(node* p, bool dir, node *c, node *n) {
    if(!c) {
        if(!dir) p->lft = n;
        else     p->rgt = n;
        return;
    }
    signed short c_key = c->key;
    signed short n_key = n->key;
    if(key_sm(c_key, n_key)) insert_node(c, false, c->lft, n);
    else                     insert_node(c, true , c->rgt, n);
} // p = Parent node, dir : false = left, true  = right, c = Current node, n = Newly added node

void insert_fixup() // TBC

void insert(tree *t, signed short key) {
    // Binary search tree insertion + Red coloring
    if(search(*t, key)) { node_exists(); return; }
    node *n = create_node(key);
    if(!t->rt) t->rt = n;
    else       insert_node(NULL, true, t->rt, n);
    // Fixup
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

void print_n_search(tree t, signed short *keys, size_t n) {
    signed short *it = keys;
    for(; it < keys + n; ++it) { print_search(t, *it); }
}

void print_minmax(tree t) {
    printf("Minimum ::: ");
    printn(minmax(t, false));
    printf("Maximum ::: ");
    printn(minmax(t, true));
} 


void header(char* s) { printf("%s", s); }

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

    header("Search\n");
    signed short keys[] = {150, 110};
    size_t n = sizeof(keys) / sizeof(keys[0]);
    print_n_search(rbt, keys, n);
    endl();
    
    header("Minimum & Maximum\n");
    print_minmax(rbt);
    endl();

    printf("Success!\n");
}