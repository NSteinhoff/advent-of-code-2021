#include <stdio.h>
#include <stdlib.h>

#define INPUT "10.txt"
#define MAX_STACK_SIZE 500
#define MAX_LINES 500

static char match(char c) {
	switch (c) {
	case '(':
		return ')';
	case '[':
		return ']';
	case '{':
		return '}';
	case '<':
		return '>';
	}

	return 0;
}

static int points(char c) {
	switch (c) {
	case ')':
		return 1;
	case ']':
		return 2;
	case '}':
		return 3;
	case '>':
		return 4;
	}

	return 0;
}

static int compare(const void *left, const void *right) {
	int64_t delta = *(uint64_t *)left - *(uint64_t *)right;

	// We need to return an integer!
	return delta > 0 ? 1 : delta < 0 ? -1 : delta;
}

int main() {
	FILE *file = fopen(INPUT, "r");
	printf("--- Part Two ---\n");

	char stack[MAX_STACK_SIZE];
	// Alias the stack to make * arithmetic clearer
	char *bottom = stack;
	char *top = stack;

	// Collect scores to be sorted at the end
	uint64_t scores[MAX_LINES];
	size_t n_score = 0;

	for (int c; (c = getc(file)) != EOF;) {
		switch (c) {
		case '(':
		case '[':
		case '{':
		case '<':
			*top++ = match(c);
			break;
		case ')':
		case ']':
		case '}':
		case '>':
			if (top - bottom <= 0 || *--top != c) {
				top = bottom;
				// Discard invalid line
				while (getc(file) != '\n')
					;
			}
			break;
		case '\n': {
			uint64_t score = 0;
			while (top - bottom > 0) {
				char c = *--top;
				score *= 5;
				score += points(c);
			}
			scores[n_score++] = score;
			break;
		}
		}
	}

	qsort(scores, n_score, sizeof(int64_t), compare);

	printf("Score: %llu\n", scores[n_score / 2]);
	fclose(file);
}
