/*--- Day 9: Smoke Basin ---

These caves seem to be lava tubes. Parts are even still volcanically active;
small hydrothermal vents release smoke into the caves that slowly settles like
rain.

If you can model how the smoke flows through the caves, you might be able to
avoid it and be that much safer. The submarine generates a heightmap of the
floor of the nearby caves for you (your puzzle input).

Smoke flows to the lowest point of the area it's in. For example, consider the
following heightmap:

2199943210
3987894921
9856789892
8767896789
9899965678

Each number corresponds to the height of a particular location, where 9 is the
highest and 0 is the lowest a location can be.

Your first goal is to find the low points - the locations that are lower than
any of its adjacent locations. Most locations have four adjacent locations (up,
down, left, and right); locations on the edge or corner of the map have three or
two adjacent locations, respectively. (Diagonal locations do not count as
adjacent.)

In the above example, there are four low points, all highlighted: two are in the
first row (a 1 and a 0), one is in the third row (a 5), and one is in the bottom
row (also a 5). All other locations on the heightmap have some lower adjacent
location, and so are not low points.

The risk level of a low point is 1 plus its height. In the above example, the
risk levels of the low points are 2, 1, 6, and 6. The sum of the risk levels of
all low points in the heightmap is therefore 15.

Find all of the low points on your heightmap. What is the sum of the risk levels
of all low points on your heightmap?

--- Part Two ---

Next, you need to find the largest basins so you know what areas are most
important to avoid.

A basin is all locations that eventually flow downward to a single low point.
Therefore, every low point has a basin, although some basins are very small.
Locations of height 9 do not count as being in any basin, and all other
locations will always be part of exactly one basin.

The size of a basin is the number of locations within the basin, including the
low point. The example above has four basins.

The top-left basin, size 3:

2199943210
3987894921
9856789892
8767896789
9899965678

The top-right basin, size 9:

2199943210
3987894921
9856789892
8767896789
9899965678

The middle basin, size 14:

2199943210
3987894921
9856789892
8767896789
9899965678

The bottom-right basin, size 9:

2199943210
3987894921
9856789892
8767896789
9899965678

Find the three largest basins and multiply their sizes together. In the above
example, this is 9 * 14 * 9 = 1134.

What do you get if you multiply together the sizes of the three largest basins?
*/
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
