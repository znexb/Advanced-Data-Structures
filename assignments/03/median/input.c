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

int str_to_short(const char* str, signed short* out) {
    char* endptr;
    errno = 0;

    long value = strtol(str, &endptr, 10);

    // Check: no digits found
    if (endptr == str) return -1;

    // Check: extra characters
    if (*endptr != '\0') return -2;

    // Check: overflow from strtol
    if (errno == ERANGE) return -3;

    // Check: fits in signed short
    if (value < SHRT_MIN || value > SHRT_MAX) return -4;

    *out = (signed short)value;
    return 0;
} // Another AI generated function. The scope of this project is not string programming.