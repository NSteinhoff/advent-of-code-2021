#include <stdio.h>

static const char input[] = "17.txt";

typedef struct {
	int x1, x2, y1, y2;
} Rect;

static int sum(int n) { return (n + 1) * n / 2; }
static int toreach(int n) {
	int s = 0;
	while (sum(s) < n)
		s++;

	return s;
}

int main() {
	FILE *file = fopen(input, "r");
	Rect target;

	fscanf(file, "target area: x=%d..%d, y=%d..%d", &target.x1, &target.x2,
	       &target.y1, &target.y2);

	fclose(file);

	printf("Target (%d,%d) -> (%d,%d)\n", target.x1, target.x2, target.y1,
	       target.y2);

	int max_y = -target.y1 - 1;
	int min_y = target.y1;
	int max_x = target.x2;
	int min_x = toreach(target.x1);

	printf("x [%d, %d] y [%d, %d]\n", min_x, max_x, min_y, max_y);

	int num_solutions = 0;
	for (int x = min_x; x <= max_x; x++)
		for (int y = min_y; y <= max_y; y++) {
			int dx = x;
			int dy = y;
			int xx, yy;

			for (xx = yy = 0; xx <= target.x2 && yy >= target.y1;
			     xx += dx, dx = dx > 0 ? dx - 1 : 0, yy += dy--) {
				if (xx >= target.x1 && yy <= target.y2) {
					num_solutions++;
					break;
				}
			}
		}

	printf("There are %d possible solutions.\n", num_solutions);
}
