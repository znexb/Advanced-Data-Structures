#include <stdio.h>
#include <stdlib.h>

// typedef is a compile-time naming convenience, thus I avoid having to write 'struct node' each time I want to use it. It is an 'alias'.
typedef struct node {
    signed short key;   // Short determines low capacity. Range of signed short is [0, 65535].
    struct node *lft;   // Pointer to the left child.
    struct node *rgt;   // Pointer to the right child.
} node;

typedef struct tree {
    struct node *rt;    // Reference to the root of the tree
} tree;


/* COMPARISON abstractions */

// The purpose of these functions is controlling cognitive load and error surface.

#include <stdbool.h>

bool search_found(node *nd, signed short key) {
    return nd->key == key;
} //    - The function above checks the success condition for searching in BST.

bool key_sm(signed short node_key, signed short key) {
    return key < node_key;
}

bool key_gt(signed short node_key, signed short key) {
    return key > node_key;
}


// Search

node* search_node(node *nd, signed short key) {
    if(!nd) return NULL; // NULL node case

    if(search_found(nd, key)) return nd; // SUCCESS case - key found in tree!
    else {
        signed short cur = nd->key; // Temporary value to avoid the nd->key rewrite and dereference.
        if(key_sm(cur, key)) { // If searched key is smaller than current node's key
            if(nd->lft) return search_node(nd->lft, key); // If left node of current node exists, traverse to it.
            else        return NULL;                      // Else, the searched node does not exist.
        } else if(key_gt(cur, key)) { // Else if searched key is greater
            if(nd->rgt) return search_node(nd->rgt, key); // Traverse right
            else        return NULL; // Else stop
        }
    } 
}

node* search(tree bst, signed short key) {
    if(bst.rt == NULL) return NULL;  // Null tree case

    return search_node(bst.rt, key); // Search node by node, recursively.
}


// Minimum & Maximum

node* minimum_node(node *nd) {
    if(!nd) return NULL; // NULL node case

    if(!nd->lft) return nd;
    else         return minimum_node(nd->lft);
}

node* minimum(tree bst) {
    if(!bst.rt) return NULL; // NULL tree case

    return minimum_node(bst.rt); // Idem for search_node.
}

node* maximum_node(node *nd) {
    if(!nd) return NULL;

    if(!nd->rgt) return nd;
    else         return maximum_node(nd->rgt);
}

node* maximum(tree bst) {
    if(!bst.rt) return NULL;

    return maximum_node(bst.rt);
}


// Insert

node* create_node(signed short key) {
    node* n = malloc(sizeof(node));
    if(!n) { // Memory allocation failure!
        perror("malloc");
        return NULL;
    }
    n->key = key;
    n->lft = NULL;
    n->rgt = NULL;
    return n;
}
//  ! malloc implies free, thus I need a destroy function.

void insert_node(node* ls, bool dir, node *nd, node *nn) {
    if(!nd) { // If the current node is NULL
        if(!dir) ls->lft = nn; // Link parent left-side if direction is left
        else     ls->rgt = nn; // Right-side
        return;
    } 

    signed short cur = nd->key;
    signed short key = nn->key;
    if(key_sm(cur, key))      insert_node(nd, false, nd->lft, nn); // If the key is smaller than the current node's key
    else if(key_gt(cur, key)) insert_node(nd, true,  nd->rgt, nn);
} // ls = Parent node; dir = Direction (left - false, right - true); nd = Current node; nn = Node to be added

void insert(tree *bst, signed short key) {
    if(search(*bst, key)) return; // If key already exists, don't bother.

    node *nn = create_node(key);
    
    if(!bst->rt) bst->rt = nn; // If there is no root...
    else         insert_node(NULL, false, bst->rt, nn);
}

void arr_insert(tree *bst, signed short *arr, size_t n) {
    signed short *it = arr; // Memory efficient iterator - pointer - 4 / 8 bytes
    for(; it < arr + n; ++it) { insert(bst, *it); }
}


// Delete

node* parent_node(node *cr, node *nd) {
    if(cr->lft == nd || cr->rgt == nd) return cr;
    else {
        if(key_sm(cr->key, nd->key))      return parent_node(cr->lft, nd);
        else if(key_gt(cr->key, nd->key)) return parent_node(cr->rgt, nd);
    }
} // cr == Current node

node* parent(tree bst, node* nd) {
    if(!bst.rt) return NULL;

    if(!nd) return NULL;
    
    if(!search(bst, nd->key)) return NULL;

    return parent_node(bst.rt, nd);
}

void delete_node() {}

void delete(tree *bst, signed short key, void print_delete(node *p), void node_null()) {
    node *n = search(*bst, key);
    node *p = parent(*bst, n);

    if(!n) {
        node_null();
        return; // If the node does not exist, don't bother.
    } else if(!p) {
        return; // Code would crash without a parent
    } else {
        if(!n->lft && !n->rgt) { // No children
            if(p->lft == n) p->lft = NULL; // Nullify pointer connections
            else if(p->rgt == n) p->rgt = NULL;
        } else if((n->lft && !n->rgt) || (!n->lft && n->rgt)) { // One child - the following operation is SPLICING
            if(p->lft == n) { // If the parent's left-side is the node
                if(n->lft) p->lft = n->lft; // If the node's only child is left
                if(n->rgt) p->lft = n->rgt; // Right
            }
            if(p->rgt == n) { // Parent's right-side = node
                if(n->lft) p->rgt = n->lft; // Node's child = left
                if(n->rgt) p->rgt = n->rgt; // Right
            }
        } else { // There are two ways of doing the pointer transplant here: successor (min of right subtree) or predecessor (max of left subtree)
            node *s = minimum_node(n->rgt); // Chose min for no particular reason. There is no difference in complexity nor in correctness.
            node *s_p = parent(*bst, s);
            if(s_p != n) s_p->lft = s->rgt; // If the parent of the found successor is not the node to be deleted
            s->lft = n->lft;
            if(s != n->rgt) s->rgt = n->rgt; // EDGE CASE ::: If the found successor is n->rgt
            if(p->lft == n) p->lft = s; // TRANSPLANT into n's position
            else if(p->rgt == n) p->rgt = s;
        }
        print_delete(n);
        free(n); // Free memory --- THIS ASSUMES NODE WAS ADDED WITH MEMORY ALLOCATION
        return;
    }
}


// ERROR UTILITIES

void search_not_found(signed short key) { printf("ERROR ::: %d NOT FOUND!!!\n", key); }

void parent_not_found(signed short key) { printf("ERROR ::: PARENT OF %d NOT FOUND!!!\n", key); }

void tree_empty(tree *t) { printf("ERROR ::: Tree %p has no nodes!\n", t); }

void node_null() { printf("ERROR ::: Node does not exist\n"); }


// PRINT UTILITIES

void printpd(node *p) { printf("%p ::: %d\n", p, p->key); }

bool print_node(node *n) {
    if(n) {
        printpd(n);
        return true;
    }
    return false;
}

void print_search(tree t, signed short key) {
    node *n = search(t, key);
    if(print_node(n)) return;
    search_not_found(key);
}

void print_minimum(tree *t) {
    node *min = minimum(*t);
    if(!print_node(min)) { tree_empty(t); return; }
}

void print_maximum(tree *t) {
    node *max = maximum(*t);
    if(!print_node(max)) { tree_empty(t); return; }
}

void print_parent(tree *t, signed short key) {
    if(!t->rt) { tree_empty(t); return; }
    node *n = search(*t, key);
    if(n)  {
        node* p = parent(*t, n);
        if(p) {
            printpd(p);
            return;
        }
        parent_not_found(key);
        return;
    }
    search_not_found(key);
}

void print_delete(node *p) { printpd(p); }


int main() {
    // node n3 = {1, NULL, NULL};
    // node n2 = {2, &n3, NULL};
    // node n1 = {3, &n2, NULL};
    // tree rt = {&n1};

    // Legacy manual insertion

    // insert(t, 4);
    // insert(t, 2);
    // insert(t, 3);
    // insert(t, 6);
    // insert(t, 1);
    // insert(t, 2);
    // insert(t, 0);


    tree *t = malloc(sizeof(tree));
    t->rt = NULL;

    signed short arr[] = {5, 3, 2, 4};
    size_t n = sizeof(arr) / sizeof(arr[0]); // Memory efficient number of elements of the arr
    arr_insert(t, arr, n);


    // An empty tree
    tree *u = malloc(sizeof(tree));
    u->rt = NULL;

    signed short _ar[] = {};
    arr_insert(u, _ar, 0);


    printf("SEARCH\n");
    print_search(*t, 3);
    print_search(*t, 2);
    print_search(*t, 1);


    printf("\n");
    printf("MINIMUM\n");
    print_minimum(t);
    print_minimum(u); // Case for empty tree


    printf("\n");
    printf("MAXIMUM\n");
    print_maximum(t);
    print_maximum(u);


    printf("\n");
    printf("PARENTs\n");
    print_parent(t, 2);
    print_parent(t, 3);
    print_parent(t, 10);
    print_parent(t, 5);
    print_parent(u, 1);


    printf("\n");
    printf("DELETION\n");
    delete(t, 3, print_delete, node_null);
    delete(t, 10, print_delete, node_null);

    free(t);
    free(u);
    
    printf("\n");
    printf("SUCCESS!\n");
    return 0;
}