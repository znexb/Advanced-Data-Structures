#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** split(const char* str, const char* delim, int* count) {
    char* temp = strdup(str);
    if (!temp) return NULL;

    int capacity = 10;
    char** result = malloc(capacity * sizeof(char*));
    if (!result) {
        free(temp);
        return NULL;
    }

    int n = 0;
    char* token = strtok(temp, delim);

    while (token != NULL) {
        if (n >= capacity) {
            capacity *= 2;
            result = realloc(result, capacity * sizeof(char*));
            if (!result) {
                free(temp);
                return NULL;
            }
        }

        result[n++] = strdup(token);
        token = strtok(NULL, delim);
    }

    free(temp);
    *count = n;
    return result;
} // This function was created using generative AI.

void free_tokens(char** tokens, int count) {
    if (!tokens) return;
    for (int i = 0; i < count; i++) { free(tokens[i]); }
    free(tokens);
}