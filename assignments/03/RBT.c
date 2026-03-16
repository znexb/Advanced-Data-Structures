#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct node {
    signed short key;   // Short determines low capacity. Range of signed short is [0, 65535].
    char *nam;
    struct node *lft;   // Pointer to the left child.
    struct node *rgt;   // Pointer to the right child.
    struct node *pnt;
    bool col;    
} node;

typedef struct tree {
    struct node *rt;    // Reference to the root of the tree
} tree;


bool key_sm (signed short left_key, signed short right_key) { return left_key < right_key; }


tree* create_tree (node *rt) {
    tree *t = malloc (sizeof(tree));
    if (!t) { perror("malloc"); return NULL; }
    t->rt = rt;
    return t;
}

node* create_node (signed short key, char *nam, node *pnt) {
    node *n = malloc (sizeof(node));
    if (!n) { perror("malloc"); return NULL; }
    n->key = key;
    n->nam = nam;
    n->lft = NULL;
    n->rgt = NULL;
    n->pnt = pnt;
    n->col = true;
}


void insert_node(node* p, bool dir, node *c, node *n) {
    if (!c) {
        if (!dir) p->lft = n;
        else      p->rgt = n;
        n->pnt = p;
        return;
    }
    if (key_sm (c->key, n->key)) insert_node(c, false, c->lft, n);
    else                       insert_node(c, true , c->rgt, n);
}

void rotate_left (tree *t, node *n) {
    node *r = n->rgt;
    n->rgt = r->lft;
    if (r->lft)                 r->lft->pnt = n;
    r->pnt = n->pnt;
    if (!n->pnt)                t->rt = r;
    else if (n == n->pnt->lft)  n->pnt->lft = r;
    else                        n->pnt->rgt = r;
    r->lft = n;
    n->pnt = r;
}

void rotate_right (tree *t, node *n) {
    node *l = n->lft;
    n->lft = l->rgt;
    if (l->rgt)                 l->rgt->pnt = n;
    l->pnt = n->pnt;
    if (!n->pnt)                t->rt = l;
    else if (n == n->pnt->rgt)  n->pnt->rgt = l;
    else                        n->pnt->lft = l;
    l->rgt = n;
    n->pnt = l;
}

void insert_fixup (tree *t, node *n) {
    while (n != t->rt && n->pnt->col) {   // RED
        if (n->pnt == n->pnt->pnt->lft) { // Parent is left child of grandparent
            node *u = n->pnt->pnt->rgt;   // Uncle  
            if (u && u->col) {             // Case 1 = Uncle is RED
                n->pnt->col      = false; // Recolor parent BLACK
                u->col           = false; // Recolor uncle BLACK
                n->pnt->pnt->col = true;  // Recolor grandparent RED
                n = n->pnt->pnt;          // Promote node
            } else {
                if (n == n->pnt->rgt) {    // Case 2 = Triangle
                    n = n->pnt;
                    rotate_left (t, n);
                }
                n->pnt->col      = false; // Recolor parent BLACK
                n->pnt->pnt->col = true;  // Recolor grandparent RED
                rotate_right (t, n->pnt->pnt);
            }
        } else {                          // Mirror
            node *u = n->pnt->pnt->lft;
            if (u && u->col) {
                n->pnt->col      = false;
                u->col           = false;
                n->pnt->pnt->col = true;
                n = n->pnt->pnt;
            } else {
                if (n == n->pnt->pnt) {
                    n = n->pnt;
                    rotate_right (t, n);
                }
                n->pnt->col      = false;
                n->pnt->pnt->col = true;
                rotate_left (t, n->pnt->pnt);
            }
        }
    }
    t->rt->col                   = false; // Root is always BLACK.
}

void insert (tree *t, char* nam, signed short key) {
    // Binary search tree insertion + Red coloring
    node *n = create_node (key, nam, NULL);
    if (!t->rt) t->rt = n;
    else       insert_node (NULL, true, t->rt, n);
    insert_fixup (t, n); // TBD
}