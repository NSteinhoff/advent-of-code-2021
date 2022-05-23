#include <stdio.h>
#include <stdint.h>

#define STEPS 100
#define ROWS 10
#define COLS 10
#define INPUT "11.txt"

static void propagate(uint8_t map[ROWS][COLS], size_t row, size_t col) {
	if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
		// Out of bounds
		return;

	if (map[row][col] > 9)
		// It's already gonna to blow
		return;

	if (map[row][col] <= 9) {
		// Increase the energy level
		map[row][col]++;

		if (map[row][col] > 9) {
			// Propagate to all neighbors
			propagate(map, row - 1, col - 1);
			propagate(map, row - 1, col + 0);
			propagate(map, row - 1, col + 1);
			propagate(map, row + 0, col + 1);
			propagate(map, row + 1, col + 1);
			propagate(map, row + 1, col + 0);
			propagate(map, row + 1, col - 1);
			propagate(map, row + 0, col - 1);
		}
	}
}

int main() {
	FILE *file = fopen(INPUT, "r");
	uint8_t map[ROWS][COLS] = {0};

	int c;
	size_t n = 0;
	while ((c = getc(file)) != EOF)
		if (c != '\n') {
			map[n / COLS][n % COLS] = c - '0';
			n++;
		}

	int flashes = 0;
	for (int step = 0; step < STEPS; step++) {
		for (int i = 0; i < ROWS; i++)
			for (int j = 0; j < COLS; j++)
				propagate(map, i, j);

		for (int i = 0; i < ROWS; i++)
			for (int j = 0; j < COLS; j++)
				if (map[i][j] > 9) {
					flashes++;
					map[i][j] = 0;
				}
	}

	printf("Flashes: %u\n", flashes);
	fclose(file);
}
