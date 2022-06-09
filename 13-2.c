#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "13.txt"

typedef struct {
	size_t cap;
	size_t len;
	char **lines;
} Lines;

Lines *lines_new() { return calloc(sizeof(Lines), 1); }

void lines_free(Lines *lines) {
	free(lines->lines);
	free(lines);
}

void lines_add(Lines *lines, char *line) {
	if (lines->len >= lines->cap)
		lines->lines = realloc(lines->lines, sizeof(char *) * lines->cap
		                                         ? (lines->cap *= 2)
		                                         : 8);

	lines->lines[lines->len++] = line;
}

void draw(char *map, size_t cols, size_t clamp_rows, size_t clamp_cols) {
	printf("\n");
	for (size_t row = 0; row < clamp_rows; row++) {
		for (size_t col = 0; col < clamp_cols; col++)
			printf("%c", map[row * cols + col]);
		printf("\n");
	}
	printf("\n");
}

int count(char *map, size_t rows, size_t cols) {
	int c = 0;

	for (size_t i = 0; i < rows * cols; i++)
		if (map[i] == '#')
			c++;

	return c;
}

int main(void) {
	FILE *file = fopen(INPUT, "r");
	if (!file) {
		perror(INPUT);
		exit(EXIT_FAILURE);
	}

	// Find the number of bytes in the file.
	fseek(file, 0L, SEEK_END);
	const size_t size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	// Allocate memory for the file contents + 1 for the null byte.
	char *content = malloc(size + 1);
	size_t bytes_read = fread(content, sizeof(char), size, file);
	content[bytes_read] = '\0';

	// Parse input into lines.
	Lines *lines = lines_new();
	for (char *line; (line = strsep(&content, "\n"));)
		if (line[0])
			lines_add(lines, line);

	// Determine the number of rows and columns from the first folds along
	// each axis
	size_t fold_x = 0;
	size_t fold_y = 0;
	for (size_t i = 0; i < lines->len; i++) {
		if (!fold_x)
			sscanf(lines->lines[i], "fold along x=%lu", &fold_x);
		if (!fold_y)
			sscanf(lines->lines[i], "fold along y=%lu", &fold_y);
		if (fold_x && fold_y)
			break;
	}
	// The paper is twice the size of the first fold plus the folded
	// line/column
	size_t rows = fold_y * 2 + 1;
	size_t cols = fold_x * 2 + 1;
	// Clamp the map output
	size_t clamp_rows = rows;
	size_t clamp_cols = cols;

	// Create a new empty map.
	char *map = malloc(sizeof(char) * rows * cols);
	memset(map, '.', sizeof(char) * rows * cols);

	// Remember at which line the fold instructions begin.
	size_t folds_start = 0;

	// Mark the points of the map
	for (size_t i = 0; i < lines->len; i++) {
		size_t x, y;
		if (!sscanf(lines->lines[i], "%lu,%lu", &x, &y)) {
			folds_start = i;
			break;
		}
		map[y * cols + x] = '#';
	}

	for (size_t i = folds_start; i < lines->len; i++) {
		char axis;
		size_t index;
		if (!sscanf(lines->lines[i], "fold along %c=%lu\n", &axis,
		            &index))
			break;

		switch (axis) {
		case 'x': {
			for (size_t row = 0; row < rows; row++) {
				for (size_t col = index + 1; col < cols; col++)
					if (map[row * cols + col] == '#') {
						size_t new_col =
						    index - (col - index);
						map[row * cols + new_col] = '#';
						map[row * cols + col] = '*';
					}
			}
			clamp_cols = index;

			break;
		}
		case 'y': {
			for (size_t row = index; row < rows; row++) {
				for (size_t col = 0; col < cols; col++)
					if (map[row * cols + col] == '#') {
						size_t new_row =
						    index - (row - index);
						map[new_row * cols + col] = '#';
						map[row * cols + col] = '*';
					}
			}
			clamp_rows = index;

			break;
		}
		default:
			printf("Unknown axis: %c\n", axis);
			return 1;
		}
	}

	draw(map, cols, clamp_rows, clamp_cols);

	free(map);
	lines_free(lines);
	return 0;
}
