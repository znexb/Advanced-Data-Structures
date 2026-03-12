#include <stdio.h>
#include <stdlib.h>

#include "BST.h"


typedef struct node {
    signed short key;   // Short determines low capacity. Range of signed short is [0, 65535].
    struct node *lft;   // Pointer to the left child.
    struct node *rgt;   // Pointer to the right child.
} node;




void ln() { printf("\n"); }

void idt(size_t n) { for(size_t i = 0; i < n; ++i) printf("    "); }

void print_filler() { for(size_t i = 0; i < 12; ++i) printf("=#"); printf("\n"); }

void print_filler_n(size_t n) { for(size_t i = 0; i < n; ++i) print_filler(); }

void options() {
    printf("Options ::: ");                                         ln();   ln();
    idt(1); printf("1. ADD    {player} {score}");                           ln();
    idt(2); printf("- Add a new player with the given score.");             ln();
    idt(1); printf("2. UPDATE {player} {delta}");                           ln();
    idt(2); printf("- Increase or decrease the player's score by delta.");  ln();
    idt(1); printf("3. REMOVE {player}");                                   ln();
    idt(2); printf("- Remove the player from the leaderboard.");            ln();
    idt(1); printf("4. TOP    k");                                             ln();
    idt(2); printf("- Return the k players with the highest scores.");      ln();
}

void menu() {
    print_filler();
    printf("LIVE LEADERBOARDer --- C");     ln();
    print_filler_n(3);                      ln();
    options();                              ln();
}


int main() {
    while(true) {
        menu();
        scanf("%d");
    }
}