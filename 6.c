#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_TIMER 8
#define INPUT "6.txt"

static void one(FILE *file) {
	const int days = 80;
	int **generations = malloc(sizeof(int *) * days + 1);
	for (int g = 0; g <= days; g++) {
		generations[g] = malloc(sizeof(int) * MAX_TIMER + 1);
		for (int t = 0; t <= MAX_TIMER; t++)
			generations[g][t] = 0;
	}

	for (int i; fscanf(file, "%d,", &i) != EOF;)
		generations[0][i]++;

	for (int d = 1; d <= days; d++) {
		for (int i = 0; i < MAX_TIMER; i++)
			generations[d][i] += generations[d - 1][i + 1];

		generations[d][6] += generations[d - 1][0];
		generations[d][8] += generations[d - 1][0];
	}

	int total = 0;
	for (int i = 0; i <= MAX_TIMER; i++)
		total += generations[days][i];

	printf("Number of fish after %d days: %d\n", days, total);

	for (int g = 0; g <= days; g++)
		free(generations[g]);
	free(generations);
}

static void two(FILE *file) {
	const int days = 256;
	uint64_t **generations = malloc(sizeof(uint64_t *) * days + 1);
	for (int g = 0; g <= days; g++) {
		generations[g] = malloc(sizeof(uint64_t) * MAX_TIMER + 1);
		for (int t = 0; t <= MAX_TIMER; t++)
			generations[g][t] = 0;
	}

	for (int i; fscanf(file, "%d,", &i) != EOF;)
		generations[0][i]++;

	for (int d = 1; d <= days; d++) {
		for (int i = 0; i < MAX_TIMER; i++)
			generations[d][i] += generations[d - 1][i + 1];

		generations[d][6] += generations[d - 1][0];
		generations[d][8] += generations[d - 1][0];
	}

	uint64_t total = 0;
	for (int i = 0; i <= MAX_TIMER; i++)
		total += generations[days][i];

	printf("Number of fish after %d days: %llu\n", days, total);

	for (int g = 0; g <= days; g++)
		free(generations[g]);
	free(generations);
}

int main() {
	FILE *file = fopen(INPUT, "r");
	one(file);
	rewind(file);
	two(file);
	return 0;
}
