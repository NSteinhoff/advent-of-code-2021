#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT "9.txt"

int main() {
	FILE *file = fopen(INPUT, "r");

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

	fclose(file);
	return 0;
}
