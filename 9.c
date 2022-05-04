#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT "9.txt"

static void one(FILE *file) {
	printf("--- Part One ---\n");
	fseek(file, 0L, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	// We need at least as many elements as bytes in the file.
	uint8_t *map = malloc(sizeof(uint8_t) * size);

	size_t n = 0;
	size_t rows = 0;
	for (char c; (c = getc(file)) != EOF;)
		if (c != '\n')
			map[n++] = c - '0';
		else
			rows++;
	size_t cols = n / rows;

	int risk = 0;
	for (size_t row = 0; row < rows; row++)
		for (size_t col = 0; col < cols; col++) {
			int height = map[row * cols + col];
			if ((row == 0 ||
			     map[(row - 1) * cols + col] > height) &&
			    (row == rows - 1 ||
			     map[(row + 1) * cols + col] > height) &&
			    (col == 0 || map[row * cols + col - 1] > height) &&
			    (col == cols - 1 ||
			     map[row * cols + col + 1] > height))
				risk += height + 1;
		}

	free(map);
	printf("Sum of risk levels: %d\n", risk);
}

static int walk_basin(uint8_t *map, size_t row, size_t col, size_t rows,
                      size_t cols) {
	// This tile is a peak or out of bounds, so stop here without counting
	if (row >= rows || col >= cols || map[row * cols + col] == 9)
		return 0;

	// Mark the current tile as 'visited' so it will be skipped on
	// subsequent visits
	map[row * cols + col] = 9;

	// Count basin tiles in all directions + 1 for the current tile
	return walk_basin(map, row - 1, col, rows, cols) +
	       walk_basin(map, row + 1, col, rows, cols) +
	       walk_basin(map, row, col - 1, rows, cols) +
	       walk_basin(map, row, col + 1, rows, cols) + 1;
}

static void two(FILE *file) {
	printf("--- Part Two ---\n");
	fseek(file, 0L, SEEK_END);
	size_t size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	// We need at least as many elements as bytes in the file.
	uint8_t *map = malloc(sizeof(uint8_t) * size);

	size_t n = 0;
	size_t rows = 0;
	for (char c; (c = getc(file)) != EOF;)
		if (c != '\n')
			map[n++] = c - '0';
		else
			rows++;
	size_t cols = n / rows;

	int basins[3] = {0};
	for (size_t row = 0; row < rows; row++)
		for (size_t col = 0; col < cols; col++) {
			if (map[row * cols + col] == 9)
				continue;

			int basin_size = walk_basin(map, row, col, rows, cols);

			size_t i_min = 0;
			for (size_t i = 0; i < 3; i++)
				if (basins[i] < basins[i_min])
					i_min = i;

			if (basins[i_min] < basin_size)
				basins[i_min] = basin_size;
		}

	free(map);
	printf("Product of the three largest basins: %d * %d * %d = %d\n",
	       basins[0], basins[1], basins[2],
	       basins[0] * basins[1] * basins[2]);
}

int main() {
	FILE *file = fopen(INPUT, "r");

	one(file);
	rewind(file);
	two(file);

	fclose(file);
	return 0;
}
