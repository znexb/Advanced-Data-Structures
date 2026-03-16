#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


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
    n->nam = malloc (strlen(nam) + 1);
    if(!n->nam) { perror("malloc"); free(n); return NULL; }
    strcpy(n->nam, nam);
    n->lft = NULL;
    n->rgt = NULL;
    n->pnt = pnt;
    n->col = true;
    return n;
}


void insert_node(node* p, bool dir, node *c, node *n) {
    if (!c) {
        if (!dir) p->lft = n;
        else      p->rgt = n;
        n->pnt = p;
        return;
    }
    if (key_sm (c->key, n->key)) insert_node(c, false, c->lft, n);
    else                         insert_node(c, true , c->rgt, n);
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


node* min (node *n) {
    if (n->lft) return min (n->lft);
    return n;
}

node* successor (node *n) { // Node n is the node whose successor is searched for.
    // Let node n = "successed node" = 'sn'
    if (n->rgt) return min (n->rgt);                // If sn's right child exists, return the minimum of the subtree with root at that.
    node *p = n->pnt;                               // Let node p be the parent of sn.
    while (p && n == p->rgt) { n = p; p = p->pnt; } // While a parent exists AND sn is on the right-side of the previous parent, THEN set sn as the parent AND update the parent.
    return p;                                       // Return the previous parent. This avoids a NULL case.
}

void delete_fixup (tree *t, node *n, node *g) { // Node n is the node to be fixed up and g is a local-copy node which holds the successor's parent.
    node *p, *s;
    while (n != t->rt && !n->col) { // If the node to be fixed up is not the root of the tree AND its color IS black. Let the fixed up node = 'fun'
        p = n ? n->pnt : g;
        if (n == p->lft) {     // If 'fun' is a left child of its parent
            s = p->rgt;        // Then the node's sibling is on the right side of fun's parent.
            // Case 1 : Transforms the og situation ("RBT-delete_fixup-Case1.jpg") such that the sibling becomes black, allowing the algorithm to move to cases 2, 3, 4.
            if (s->col) {                   // If the color of the sibling is RED
                s->col = false;             // Then color it BLACK.
                p->col = true;         // Color the parent RED, as per RBT coloring rules.
                rotate_left (t, p);    // Rotate the parent to the left.
                s = p->rgt;            // Update the sibling to be on n's new parent's right-side.
            }
            // Case 2 : RBT-delete_fixup-Case2.jpg : Moves the double black upward, thus the subtree black-height becomes balanced, but the parent might now be double black, thus the loop is repeated.
            if (!s->lft->col && !s->rgt->col) {  // If the color of both children of the new sibling are BLACK
                s->col = true;                   // Color the sibling RED.
                n = p;                      // Set the 'fun' node to its parent.
            } else {
                // Case 3 : RBT-delete_fixup-Case3.jpg : Converts the tree into case 4, as this is a prep step.
                if (!s->rgt->col) {  // If only its right child is BLACK.
                    s->lft->col = false;    // Color its rch BLACK.
                    s->col = true;          // And color it RED.
                    rotate_right (t, s);    // Rotate the sibling to the right.
                    s = p->rgt;        // Update the sibling to fun's parent's right child.
                }
                // Case 4 : RBT-delete_fixup-Case4.jpg : This resolves the double black and terminates the algorithm.
                s->col = p->col;       // Color consistency between the sibling and fun.
                p->col = false;        // Color fun's parent BLACK.
                s->rgt->col = false;   // Color sibling's right child BLACK.
                rotate_left (t, p);    // Rotate the parent of fun to the left.
                n = t->rt;             // Updated fun to be the tree's root.
            }
        } else { // Perfect mirror image
            s = p->lft; 
            if (s->col) { // Case 1
                s->col = false;
                p->col = true;
                rotate_right (t, p);
                s = p->lft;
            }
            if (!s->rgt->col && !s->lft->col) { // Case 2
                s->col = true;
                n = p;
            } else {
                if (!s->lft->col) { // Case 3
                    s->rgt->col = false;
                    s->col = true;
                    rotate_left (t, s);
                    s = p->lft;
                }
                // Case 4
                s->col = p->col;
                p->col = false;
                s->lft->col = false;
                rotate_right(t, p);
                n = t->rt;  // Updated fun to be the tree's root.
            }
        }
    }
    if(n) n->col = false;
}

node* delete (tree *t, node *n) {
    node *p, *c;                                      // p = successor; c = child
    if (!n->lft || !n->rgt)    p = n;                 // If node has less than one or one child, do 'easy' delete
    else                       p = successor (n);     //    Else if node has two children, find successor
    if (p->lft)                c = p->lft;            // If child is on the left side of successor, acknowledge. This set of ifs updates the connection successor ->ch child
    else                       c = p->rgt;            //    Else right-side child, idem
    if (c)                     c->pnt = p->pnt;       // If a child indeed exists, the parent of it becomes the parent of the successor. This if updates the connection child ->pt successor 
    if (!p->pnt)               t->rt = c;             // If the successor has no parent, then the root must be set as the child. This if set updates the successor's parent ->ch connection to the child.
    else if (p == p->pnt->lft) p->pnt->lft = c;       //    Else if the successor is its parent's left child, set the parent's left child as the child
    else                       p->pnt->rgt = c;       //    Else (if the successor is its parent's right child), idem but right-wise
    if (p != n) {
        n->key = p->key;
        n->nam = p->nam;
    }
    if (!p->col) delete_fixup (t, c, p->pnt);         // If the color of the successor is BLACK, delete_fixup
    return p;                                         // Return the successor of the node. In 'safe' delete, it is the deleted node itself.
}


bool search_found (node *n, signed short key) { return n->key == key; }

bool is_tree_null (tree t) { return !t.rt; }

node* search_node (node *n, signed short key) {
    if (!n) return NULL;

    if (search_found (n, key) ) return n;
    else {
        signed short cur = n->key;
        if( key_sm (cur, key) ) {
            if (n->lft) return search_node (n->lft, key);
            else        return NULL;
        } else { // Implied else is key >= cur. The case key == cur can never occur because of the search_found check. Thus, we are left with key > cur.
            if (n->rgt) return search_node (n->rgt, key);
            else        return NULL;
        }
    }
}

node* search (tree t, signed short key) {
    if( is_tree_null (t) ) return NULL;
    return search_node (t.rt, key);
}


node* max (node *n) {
    if (n->rgt) return max (n->rgt);
    return n;
}