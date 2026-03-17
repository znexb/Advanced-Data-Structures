#ifndef INPUT_H
#define INPUT_H

char** split(const char* str, const char* delim, int* count);

void free_tokens(char** tokens, int count);

int str_to_short(const char* str, signed short* out);

#endif