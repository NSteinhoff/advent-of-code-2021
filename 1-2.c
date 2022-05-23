#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_MAX 6 // 4 digits + newline + space for '\0' terminator

int main() {
	FILE *file = fopen("1.txt", "r");
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
	fclose(file);
}
