#pragma once

#include <stdlib.h>
#include <string.h>

typedef struct {
	size_t cap;
	size_t len;
	char *base;
	char **lines;
} Lines;

Lines *lines_from_string(char *content);
void lines_free(Lines *lines);

static Lines *lines_new(void) { return calloc(sizeof(Lines), 1); }
static void lines_add(Lines *lines, char *line) {
	if (lines->len >= lines->cap)
		lines->lines = realloc(lines->lines, sizeof(char *) * lines->cap
							     ? (lines->cap *= 2)
							     : 8);

	lines->lines[lines->len++] = line;
}

void lines_free(Lines *lines) {
	if (lines->lines) free(lines->lines);
	if (lines->base) free(lines->base);
	free(lines);
}

Lines *lines_from_string(char *base) {
	if (!base) return NULL;

	Lines *lines = lines_new();
	lines->base = base;
	for (char *line; (line = strsep(&base, "\n"));)
		if (strlen(line) > 0) lines_add(lines, line);

	return lines;
}
