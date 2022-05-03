#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_MAX 6 // 4 digits + newline + space for '\0' terminator

void one(FILE *file) {
	printf("--- Part One ---\n");
	char line[BUF_MAX];

	int current = 0;
	int previous = INT_MAX;
	int count = 0;

	while (fgets(line, BUF_MAX, file)) {
		current = atoi(line);

		if (current > previous)
			count++;

		previous = current;
	}

	printf("Count: %d\n", count);
}

void two(FILE *file) {
	printf("--- Part One ---\n");
	char line[BUF_MAX];

	int current = 0;
	int previous = INT_MAX;
	int count = 0;

	int window[3] = {0};
	int l = 0;
	while (fgets(line, BUF_MAX, file)) {
		window[l % 3] = atoi(line);
		l++;

		if (l >= 3) {
			current = 0;
			for (int i = 0; i < 3; i++) {
				current += window[i];
			}

			if (current > previous)
				count++;

			previous = current;
		}
	}
	printf("Count: %d\n", count);
}

int main() {
	FILE *file = fopen("1.txt", "r");
	one(file);
	rewind(file);
	two(file);
	fclose(file);
}
