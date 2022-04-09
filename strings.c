#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
typedef struct
{
	const char *start;
	size_t len;
} token;

char **str_split(const char *str, char *sep) {
	char **array;
	unsigned int start = 0, stop, toks = 0, t;
	token *tokens = malloc((strlen(str) + 1) * sizeof(token));
	for (stop = 0; str[stop]; stop++) {
		if (str[stop] == *sep) {
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
	for (t = 0; t < toks; t++) {
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

char *scin_string_copy_append(char *first, const char *second) {
	char *ret = malloc(strlen(first) + strlen(second) + 2);
	strcpy(ret, first);
	strcat(ret, second);
	return ret;
}

char *scin_string_append(char *dest, const char *appendee) {
	dest = realloc(dest, strlen(dest) + strlen(appendee) + 2);
	strcat(dest, appendee);
	return dest;
}

char *convert_int_to_str(int number) {
	int strlen = snprintf(NULL, 0, "%d", number) + 1;
	char *ret = malloc(strlen);
	snprintf(ret, strlen, "%d", number);

	return ret;
}