#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define LEN 6 // 4 digits + newline + space for '\0' terminator

int main() {
	FILE *file = fopen("1.txt", "r");
	char line[LEN];

	int current = 0;
	int previous = INT_MAX;
	int count = 0;

	while (fgets(line, LEN, file)) {
		current = atoi(line);

		if (current > previous)
			count++;

		previous = current;
	}

	printf("Count: %d\n", count);
	fclose(file);
}
