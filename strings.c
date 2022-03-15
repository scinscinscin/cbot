#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
typedef struct
{
    const char *start;
    size_t len;
} token;

char **str_split(const char *str, char *sep)
{
    char **array;
    unsigned int start = 0, stop, toks = 0, t;
    token *tokens = malloc((strlen(str) + 1) * sizeof(token));
    for (stop = 0; str[stop]; stop++)
    {
        if (str[stop] == *sep)
        {
            tokens[toks].start = str + start;
            tokens[toks].len = stop - start;
            toks++;
            start = stop + 1;
        }
    }
    /* Mop up the last token */
    tokens[toks].start = str + start;
    tokens[toks].len = stop - start;
    toks++;
    array = malloc((toks + 1) * sizeof(char *));
    for (t = 0; t < toks; t++)
    {
        /* Calloc makes it nul-terminated */
        char *token = calloc(tokens[t].len + 1, 1);
        memcpy(token, tokens[t].start, tokens[t].len);
        array[t] = token;
    }
    /* Add a sentinel */
    array[t] = NULL;
    free(tokens);
    return array;
}