#include <stdio.h>
#include <stdlib.h>


#include <stdbool.h>

typedef struct node {
    signed short key;
    struct node *lft;
    struct node *rgt;
    struct node *pnt;
    bool col; // Bool with the following specification: RED == TRUE & BLACK == FALSE;
} node;

typedef struct tree {
    struct node *rt;
} tree;


// Search

bool search_found (node *n, signed short key) { return n->key == key; }

// n_key = NODE key, s_key = SEARCHED key
bool key_sm (signed short n_key, signed short s_key) { return s_key < n_key; }

bool is_tree_null (tree bst) { return !bst.rt; }

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

node* search (tree bst, signed short key) {
    if( is_tree_null (bst) ) return NULL;
    return search_node (bst.rt, key);
}


// Minimum & Maximum

node* max (node *n) {
    if (n->rgt) return max (n->rgt);
    return n;
}

node* min (node *n) {
    if (n->lft) return min (n->lft);
    return n;
}

node* minmax (tree t, bool which) { // max == true & min == false
    if (is_tree_null (t)) return NULL;
    if (which) { return max (t.rt); }
    return min (t.rt);
}


// Insert

tree* create_tree (node *rt) {
    tree *t = malloc (sizeof (tree));
    if (!t) { perror ("malloc"); return NULL; }
    t->rt = rt;
    return t;
}

node* create_node (signed short key, node *pnt) {
    node *n = malloc (sizeof (node));
    if (!n) { perror ("malloc"); return NULL; }
    n->key = key;
    n->lft = NULL;
    n->rgt = NULL;
    n->pnt = pnt;
    n->col = true; // Red by default makes it insert friendly
}

void node_exists () { printf ("Node already exists!\n"); }

void insert_node (node* p, bool dir, node *c, node *n) {
    if (!c) {
        if (!dir) p->lft = n;
        else     p->rgt = n;
        n->pnt = p;
        return;
    }
    signed short c_key = c->key;
    signed short n_key = n->key;
    if (key_sm (c_key, n_key)) insert_node (c, false, c->lft, n);
    else                       insert_node (c, true , c->rgt, n);
} // p = Parent node, dir : false = left, true  = right, c = Current node, n = Newly added node

void rotate_left (tree *t, node *n) {
    node *r = n->rgt;                           // Initialize right of node
    n->rgt = r->lft;                            // Switch children
    if (r->lft)                r->lft->pnt = n; // If the right of node has a child, save it
    r->pnt = n->pnt;                            // Switch parents
    if (!n->pnt)               t->rt = r;       // If root, update it
    else if (n == n->pnt->lft) n->pnt->lft = r; // Recouple the parent to child link
    else                       n->pnt->rgt = r;
    r->lft = n;                                 // Og node becomes a child of the 'promoted' node
    n->pnt = r;                                 // Update parent of 'demoted' node
}

void rotate_right(tree *t, node *n) { // Symmetric to rotate_left
    node *l = n->lft;
    n->lft = l->rgt;
    if (l->rgt) l->rgt->pnt = n;
    l->pnt = n->pnt;
    if (!n->pnt)               t->rt = l;
    else if (n == n->pnt->rgt) n->pnt->rgt = l;
    else                       n->pnt->lft = l;
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

void insert (tree *t, signed short key) {
    // Binary search tree insertion + Red coloring
    node *n = create_node (key, NULL);
    if (!t->rt) t->rt = n;
    else       insert_node (NULL, true, t->rt, n);
    insert_fixup (t, n); // TBD
}

void insert_n (tree *t, signed short *keys, size_t n) {
    signed short *it = keys;
    for(; it < keys + n; ++it) { insert(t, *it); }
}


// Delete

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
    n->col = false;
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
    if (p != n)                n->key = p->key;       // If the successor is not equal to the node, as per the 'easy' delete case, exchange information. In this case, I only have a key for my RBT implementation
    if (!p->col) delete_fixup (t, c, p->pnt);         // If the color of the successor is BLACK, delete_fixup
    return p;                                         // Return the successor of the node. In 'safe' delete, it is the deleted node itself.
}

// Below is the delete function for BST. I implemented it because I saw it in the lecture notes, only then to realize that this is just the BST deletion and it was showcased only for comparison purposes.
// node* del (tree *t, node *n) {
//     node *p, *c;                                  // p = successor, c = child
//     if (!n->lft || !n->rgt)    p = n;             // If the node to be del'd has only one child or zero childre, successor is the node itself. Let this be called "easy delete".
//     else                       p = successor (n); //    Else, find successor of n through the algorithm
//     if (p->lft)                c = p->lft;        // If the successor has a left child, set the child
//     else                       c = p->rgt;        //    Else right-side
//     if (c)                     c->pnt = p->pnt;   // If the child exists, then set the parent of the child as the successor's child
//     if (!p->pnt)               t->rt = c;         // If the successor has no parent, set the root as the child
//     else if (p == p->pnt->lft) p->pnt->lft = c;   //    Else if the successor is its parent's left child, set that as the child
//     else                       p->pnt->rgt = c;   //    Else (right-side), idem for right-side
//     if (p != n)                n->key = p->key;   // If the successor is not the del'd node, transfer information
//     return p;                                     // Return the successor
// }


// Print

void printn (node *n) { if(!n) return; printf ("%p = %d\n", n, n->key); }

void printn_null () { printf ("ERROR ::: Node is null!\n"); }

// void print_search(tree t, signed short key) {
//     node *n = search(t, key);
//     if(n) printn(n);
//     else printn_null();
// }
// Trashed this version because: v2 avoids unnecessary 'else' + v2 keeps control flow flatter + v2 scales better in larger functions

void print_search (tree t, signed short key) {
    node *n = search (t, key);
    if (n) { printn (n); return; }
    printn_null ();
}

// The function below is not strictly necessary, keeping it because it does not bother me.
void print_parent_key (tree t, signed short key) { // Print parent of a node based on knowing the key
    node *n = search (t, key);
    if (n) { printn (n); return; }
    printn_null ();
}

void print_parent (node *n) {
    if(n) { printn (n); return; }
    printn_null ();
}

void print_n_objects (tree t, signed short *keys, size_t n, void print_object(tree t, signed short key)) {
    signed short *it = keys;
    for (; it < keys + n; ++it) { print_object (t, *it); }
} // Proto Java interface type function

// This might be overly complex, but I implemented it for C experimentation purposes.
void print_n_objects_deref (tree t, node *nodes, size_t n, void print_object(tree t, signed short key)) {
    node *it = nodes;
    signed short keys[n];
    size_t i = 0;
    for (; it < nodes + n; ++it) { keys [i] = it->key; ++i; }
    print_n_objects (t, keys, n, print_object);
}

void print_minmax (tree t) {
    printf ("Minimum ::: ");
    printn (minmax(t, false));
    printf ("Maximum ::: ");
    printn (minmax(t, true));
}


// Extras

void parse_node (node *n, node **nodes, size_t *size) {
    if(n) { 
        (*nodes) [*size - 1] = *n; 
        ++*size; 
        *nodes = realloc (*nodes, *size * sizeof (node));
    }
    if(n->lft) parse_node (n->lft, nodes, size);
    if(n->rgt) parse_node (n->rgt, nodes, size);
}

void parse_tree (tree t, node **nodes, size_t *size) {
    if (!t.rt) return;                          // Null tree case
    parse_node (t.rt, nodes, size);
}

bool compare_heaps (node *h1, node *h2) {
    if (h1->key != h2->key) return false;
    node *it1 = 1 + h1;
    node *it2 = 1 + h2;
    while (it1 && it2) { if (it1 != it2) return false; }
    return true;
}

void compare_trees(tree t1, tree t2) {
    if (!t1.rt || !t2.rt) return;           // If any of the trees are empty
    unsigned short right_differences = 0;   // Difference metrics
    unsigned short left_differences = 0;
    node *nodes1 = malloc(1 * sizeof (node));    // At least one node because root is not empty
    size_t size1 = 1;                               // Size of nodes[]
    node *nodes2 = malloc(1 * sizeof (node));
    size_t size2 = 1;
    parse_tree(t1, &nodes1, &size1);
    parse_tree(t1, &nodes2, &size2);
    if (compare_heaps (nodes1, nodes2)) { printf("Equal"); return; }
    printf("Unequal");
}


void header (char* s) { printf ("%s", s); }

void endl () { printf ("\n"); }

int main () {
    // RBT_sample.jpg
    // node n8 = {150, NULL, NULL, NULL, true};
    // node n7 = {110, NULL, NULL, NULL, true};
    // node n6 = {145, &n7, &n8, NULL, false};
    // node n5 = {76, NULL, NULL, NULL, false};
    // node n4 = {32, NULL, NULL, NULL, true};
    // node n3 = {21, NULL, &n4, NULL, false};
    // node n2 = {60, &n3, &n5, NULL, true};
    // node n1 = {100, &n2, &n6, NULL, false};
    // n2.pnt = &n1;
    // n3.pnt = &n2;
    // n4.pnt = &n3;
    // n5.pnt = &n2;
    // n6.pnt = &n1;
    // n7.pnt = &n6;
    // n8.pnt = &n6;
    // tree rbt = {&n1};

    tree *rbt = malloc(sizeof(tree));
    rbt->rt = NULL;
    signed short keys [] = {100, 60, 21, 32, 76, 145, 110, 150};
    size_t n = sizeof (keys) / sizeof (keys[0]);
    insert_n (rbt, keys, n);

    header ("Search\n");
    signed short keys_s [] = {150, 110};
    size_t n_s = sizeof (keys_s) / sizeof (keys_s [0]);
    print_n_objects (*rbt, keys, n_s, print_search);
    endl ();

    // header ("Parents\n"); // This was not required by assignment, but here we are
    // node keys_p[] = {n8, n6};
    // size_t n_p = sizeof (keys_p) / sizeof (keys_p[0]);
    // print_n_objects_deref (rbt, keys_p, n_p, print_parent_key);
    // endl ();
    
    header ("Minimum & Maximum\n");
    print_minmax (*rbt);
    endl ();

    header ("Deletion test\n");
    insert(rbt, 23);
    print_search(*rbt, 23);
    node *d = search(*rbt, 23);
    delete(rbt, d);
    print_search(*rbt, 23);
    endl();

    header ("Parser tests\n");
    node *nodes = malloc(1 * sizeof (node));
    size_t nodes_size = 1;
    parse_tree (*rbt, &nodes, &n);
    free (nodes);
    endl ();

    // header ("Comparison tests\n"); It does not work properly, but the foundation is set!
    // compare_trees(*rbt, *rbt);
    // endl ();

    free(rbt);
    printf ("Success!\n");
}