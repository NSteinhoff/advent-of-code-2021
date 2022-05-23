#include <stdio.h>

#define INPUT "5.txt"
#define MAX_ROWS 1000
#define MAX_COLS 1000

#define max(a, b) ((a) >= (b) ? (a) : (b))
#define min(a, b) ((a) <= (b) ? (a) : (b))
#define abs(a) ((a) > 0 ? (a) : -(a))

int main() {
	FILE *file = fopen(INPUT, "r");
	int map[MAX_ROWS][MAX_COLS] = {0};

	int x1, y1, x2, y2;
	while (fscanf(file, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2) != EOF)
		for (int y = min(y1, y2); y <= max(y1, y2); y++)
			for (int x = min(x1, x2); x <= max(x1, x2); x++)
				if (x1 == x2 || y1 == y2)
					map[y][x]++;

	int count_overlap = 0;
	for (size_t y = 0; y < MAX_ROWS; y++)
		for (size_t x = 0; x < MAX_COLS; x++)
			if (map[y][x] > 1)
				count_overlap++;

	printf("=> %d\n", count_overlap);
	fclose(file);
}
