#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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
		return 3;
	case ']':
		return 57;
	case '}':
		return 1197;
	case '>':
		return 25137;
	}

	return 0;
}

int main() {
	FILE *file = fopen(INPUT, "r");
	char stack[MAX_STACK_SIZE];
	char *sp = stack;
	int score = 0;

	for (int c; (c = getc(file)) != EOF;) {
		switch (c) {
		case '(':
		case '[':
		case '{':
		case '<':
			// For each opening character, we push the matching
			// closing character onto the stack
			*sp++ = match(c);
			break;
		case ')':
		case ']':
		case '}':
		case '>':
			// For each closing character, we pop the stack and
			// check if it matches the current character
			// matches. If not, score the line and advance to the
			// next.
			if (sp - stack <= 0 || *--sp != c) {
				score += points(c);
				sp = stack;
				// Consume the rest of the line
				while (getc(file) != '\n')
					;
			}
			break;
		case '\n':
			// Reset the stack to process the next line
			sp = stack;
			break;
		}
	}

	printf("Score: %d\n", score);
	fclose(file);
}
