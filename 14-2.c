#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read-to-string.h"

#define INPUT "14.txt"
#define N 26
#define ITERATIONS 40
#define C2I(C) (C) - 'A'
#define I2C(C) (C) + 'A'

int main() {
	char *input = read_to_string(INPUT);
	char *freeme = input;
	char *sequence = strsep(&input, "\n");

	int in_to_out[N][N] = {0};
	char left, right, out;

	char *line;
	while ((line = strsep(&input, "\n"))) {
		if (strcmp(line, "") == 0)
			continue;
		sscanf(line, "%c%c -> %c", &left, &right, &out);
		// normalize all letters to [0, 26]
		in_to_out[C2I(left)][C2I(right)] = C2I(out);
	}

	// Count initial pairs
	uint64_t pair_counts[N][N] = {0};
	char *s = sequence;
	for (left = *s++, right = *s++; left && right;
	     left = right, right = *s++) {
		pair_counts[C2I(left)][C2I(right)]++;
	}

	uint64_t next_pair_counts[N][N];
	for (int iter = 0; iter < ITERATIONS; iter++) {
		// Need to start fresh, so as not to re-use an updated count in
		// the same iteration.
		memset(next_pair_counts, 0, sizeof(next_pair_counts));

		for (int left = 0; left < N; left++)
			for (int right = 0; right < N; right++) {
				int out = in_to_out[left][right];
				size_t count = pair_counts[left][right];
				// Each pair create two new pairs
				// HH -> N -> HNH
				// lr    o    lo
				//             or
				next_pair_counts[left][out] += count;
				next_pair_counts[out][right] += count;
			}

		memcpy(pair_counts, next_pair_counts, sizeof(pair_counts));
	}

	uint64_t counts[N] = {0};

	// We always count the second element in each pair. The very first
	// element of the input sequence is never the second element, so we
	// need to count it separately.
	counts[C2I(*sequence)]++;

	for (int left = 0; left < N; left++)
		for (int right = 0; right < N; right++) {
			if (pair_counts[left][right]) {
				counts[right] += pair_counts[left][right];
			}
		}

	printf("Counts:\n");
	for (int i = 0; i < N; i++)
		if (counts[i])
			printf("%c:\t%16llu\n", I2C(i), counts[i]);

	size_t min = 0;
	size_t max = 0;
	for (int i = 0; i < N; i++) {
		size_t count = counts[i];
		if (count > max)
			max = count;
		if (min == 0 || (count > 0 && count < min))
			min = count;
	}
	printf("\nMax:\t  %14lu\nMin:\t- %14lu\n", max, min);
	printf("\nResult:\t%16lu\n", max - min);

	free(freeme);

	return 0;
}
