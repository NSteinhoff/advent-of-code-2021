#include <stdio.h>
#include <stdlib.h>

#define MAX_TIMER 8
#define INPUT "6.txt"


//   0  1  2  3  4  5  6  7  8
//  ---------------------------
//  [0, 1, 1, 2, 1, 0, 0, 0, 0]
//  [1, 1, 2, 1, 0, 0, 0, 0, 0]
//  [1, 2, 1, 0, 0, 0, 1, 0, 1]
//  [2, 1, 0, 0, 0, 1, 1, 1, 1]
//  [1, 0, 0, 0, 1, 1, 3, 1, 2]


int main(void) {
	FILE *file = fopen(INPUT, "r");
	const int days = 80;
	int **generations = malloc(sizeof(int *) * days + 1);
	for (int g = 0; g <= days; g++) {
		generations[g] = malloc(sizeof(int) * MAX_TIMER + 1);
		for (int t = 0; t <= MAX_TIMER; t++)
			generations[g][t] = 0;
	}

	for (int t; fscanf(file, "%d,", &t) != EOF;)
		generations[0][t]++;

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
	return 0;
}
