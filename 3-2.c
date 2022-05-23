#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 1000
#define MAX_BITS 12
#define INPUT "3.txt"

static int bits_to_number(const char *bits) {
	int number = 0;
	for (int i = 0; i < (int)strlen(bits); i++) {
		number <<= 1;
		number |= bits[i] == '1';
	}

	return number;
}

static bool get_bit(int counts[2], bool most) {
	bool most_common = counts[1] >= counts[0];

	return most ? most_common : !most_common;
}

static const char *find_pattern(const int init_counts[2],
                                const char *init_matches[][MAX_LINES],
                                bool most) {
	int counts[2] = {0};
	const char *matches[2][MAX_LINES] = {0};

	for (int bit = 0; bit <= 1; bit++) {
		counts[bit] = init_counts[bit];
		for (int p = 0; p < init_counts[bit]; p++)
			matches[bit][p] = init_matches[bit][p];
	}

	int pos = 0;
	bool bit;
	int n;
	const char **lines;
	while (counts[0] > 1 || counts[1] > 1) {
		pos++;

		bit = get_bit(counts, most);
		lines = matches[bit];
		n = counts[bit];
		counts[0] = 0;
		counts[1] = 0;

		for (int i = 0; i < n; i++) {
			bit = lines[i][pos] == '1';
			matches[bit][counts[bit]++] = lines[i];
		}
	}

	return matches[get_bit(counts, most)][0];
}

int main() {
	FILE *file = fopen(INPUT, "r");
	fseek(file, 0L, SEEK_END);
	const size_t size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	char *content = malloc(size + 1);
	char *freeme = content;
	const size_t bytes_read = fread(content, sizeof(char), size, file);
	content[bytes_read] = '\0';

	int init_counts[2] = {0};
	const char *init_matches[2][MAX_LINES] = {0};
	bool bit;
	for (char *line; (line = strsep(&content, "\n"));) {
		if (line[0] == '\0')
			continue;
		bit = line[0] == '1';
		init_matches[bit][init_counts[bit]++] = line;
	}

	printf(
	    "=> %d\n",
	    bits_to_number(find_pattern(init_counts, init_matches, true)) *
	        bits_to_number(find_pattern(init_counts, init_matches, false)));

	free(freeme);
	fclose(file);
}
