#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "14.txt"
#define N 26
#define ITERATIONS 40

static char *read() {
	FILE *file = fopen(INPUT, "r");
	fseek(file, 0L, SEEK_END);
	size_t bytes = (size_t)ftell(file);
	fseek(file, 0L, SEEK_SET);
	char *content = malloc(bytes + 1);
	fread(content, 1, bytes, file);
	fclose(file);

	return content;
}

int main() {
	char *input = read();
	char *freeme = input;
	char *sequence = strsep(&input, "\n");
	printf("%s\n\n", sequence);

	int table[N][N] = {0};
	char i1, i2, o;

	char *line;
	while ((line = strsep(&input, "\n"))) {
		if (strcmp(line, "") == 0)
			continue;
		printf("%s\n", line);
		sscanf(line, "%c%c -> %c", &i1, &i2, &o);
		// normalize all letters to [0, 26]
		size_t row = (size_t)(i1 - 'A');
		size_t col = (size_t)(i2 - 'A');
		table[row][col] = (int)(o - 'A');
	}
	printf("\n");

	// Count the of initial pairs
	uint64_t pairs[N][N] = {0};
	char *s = sequence;
	char c1, c2;
	for (c1 = *(s++), c2 = *(s++); c2; c1 = c2, c2 = *(s++)) {
		pairs[c1 - 'A'][c2 - 'A']++;
	}

	uint64_t next_pairs[N][N];
	for (int iter = 0; iter < ITERATIONS; iter++) {
		memset(next_pairs, 0, sizeof(next_pairs));

		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++) {
				int out = table[i][j];
				size_t count = pairs[i][j];
				next_pairs[i][out] += count;
				next_pairs[out][j] += count;
			}

		memcpy(pairs, next_pairs, sizeof(pairs));
	}

	uint64_t counts[N] = {0};
	// We always count the second element in each pair. The very first
	// element of the input sequence is never the second element, so we
	// need to count it separately.
	counts[*sequence - 'A']++;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			if (pairs[i][j]) {
				counts[j] += pairs[i][j];
				printf("%c%c: %llu\n", i + 'A', j + 'A',
				       pairs[i][j]);
			}
		}
	printf("\n");

	for (int i = 0; i < N; i++)
		if (counts[i])
			printf("%c: %llu\n", i + 'A', counts[i]);

	size_t min = 0;
	size_t max = 0;
	for (int i = 0; i < N; i++) {
		size_t count = counts[i];
		if (count > max)
			max = count;
		if (min == 0 || (count > 0 && count < min))
			min = count;
	}
	printf("%lu - %lu = %lu\n", max, min, max - min);

	free(freeme);

	return 0;
}
