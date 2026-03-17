#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "RBT.h"
#include "input.h"

int main() {
    tree* t = create_tree(NULL);
    unsigned short frq[256] = {0};
    while (true) {
        char line[256];
        fgets(line, sizeof(line), stdin);
        int count = 0;

        line[strcspn(line, "\n")] = '\0';
        char** tokens = split(line, " ", &count);

        if (!strcmp(tokens[0], "END")) {
            return 0;
        } else if (!strcmp(tokens[0], "ADD")) {
            signed short value;
            if (str_to_short(tokens[1], &value) || value > 255) { printf("Invalid value!\n"); continue; }

            insert(t, value);
            frq[value]++;
        } else if (!strcmp(tokens[0], "REMOVE")) {
            signed short value;
            if (str_to_short(tokens[1], &value) || value > 255) { printf("Invalid value!\n"); continue; }

            if      (frq[value] == 0) { printf("Value does not exist!\n"); }
            else if (frq[value] == 1) {
                node* del = search(*t, value);
                if (!del) { printf("Tree error ::: Node not found!\n"); continue; }
                delete(t, del);
                frq[value]--;
            } else   frq[value]--;
        } else if (!strcmp(tokens[0], "MEDIAN")) {
            node* nodes = malloc(1 * sizeof(node));
            size_t size = 1;
            parse_tree(*t, &nodes, &size);

            unsigned short sum = 0;
            for (size_t i = 0; i < size; i++)
                while (frq[nodes[i].key]) {
                    sum += nodes[i].key;
                    frq[nodes[i].key]--;
                }
            printf("%hd\n", sum / size - 1);
        }
    }
}