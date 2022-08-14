#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"

#define INPUT "14-example.txt"
#define N 26
#define ITERATIONS 10

int main() {
	char *input = read_to_string(INPUT);
	char *freeme = input;
	char *sequence = strsep(&input, "\n");
	char table[N][N] = {0};

	char i1, i2, o;

	char *line;
	while ((line = strsep(&input, "\n"))) {
		if (strcmp(line, "") == 0)
			continue;
		sscanf(line, "%c%c -> %c", &i1, &i2, &o);
		size_t row = (size_t)(i1 - 'A');
		size_t col = (size_t)(i2 - 'A');
		table[row][col] = o;
	}

	char *prev = strdup(sequence);
	char *next = prev;

	for (int i = 0; i < ITERATIONS; i++) {
		size_t this_size = strlen(prev);
		size_t next_size = this_size * 2; // includes '\0' terminator
		next = malloc(next_size);
		for (size_t j = 0; j < this_size - 1; j++) {
			char first = prev[j];
			char second = prev[j + 1];
			char insert = table[first - 'A'][second - 'A'];
			next[j + j] = first;
			next[j + j + 1] = insert;
			next[j + j + 2] = second;
		}

		free(prev);
		prev = next;
	}

	size_t counts[N] = {0};
	for (char *c = prev; *c; c++)
		counts[*c - 'A']++;

	for (int i = 0; i < N; i++)
		if (counts[i])
			printf("%c: %zu\n", i + 'A', counts[i]);

	size_t min = 0;
	size_t max = 0;
	for (int i = 0; i < N; i++) {
		size_t count = counts[i];
		if (count > max) max = count;
		if (min == 0 || (count > 0 && count < min)) min = count;
	}
	printf("%lu - %lu = %lu\n", max, min, max - min);


	free(next);
	free(freeme);

	return 0;
}
