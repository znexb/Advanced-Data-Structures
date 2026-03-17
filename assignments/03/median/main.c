#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "RBT.h"
#include "input.h"

int main() {
    tree* t = create_tree(NULL);
    unsigned short frq[256] = {0};
    unsigned short medians[16] = {0};
    while (true) {
        char line[256];
        fgets(line, sizeof(line), stdin);
        int count = 0;

        line[strcspn(line, "\n")] = '\0';
        char** tokens = split(line, " ", &count);

        if (!strcmp(tokens[0], "END")) {
            size_t i = 0;
            while (medians[i]) { printf("%hu\n", medians[i]); i++; }

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
            node* nodes;
            size_t size;
            parse_tree(*t, &nodes, &size);

            if (size == 0) {
                printf("0\n");
                free(nodes);
                continue;
            }

            unsigned short sum = 0;
            unsigned short tmp[256];
            memcpy(tmp, frq, 256 * sizeof(unsigned short));
            for (size_t i = 0; i < size; i++)
                while (tmp[nodes[i].key]) {
                    sum += nodes[i].key;
                    tmp[nodes[i].key]--;
                }

            size_t i = 0;
            while(medians[i]) i++;
            if (i > 15) { printf("Median list full. Please use END to output!\n"); continue; }
            medians[i] = sum / size;

            free(nodes);
        }
    }
}