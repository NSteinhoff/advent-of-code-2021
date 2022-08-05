#include <stdio.h>

static const char input[] = "17.txt";

typedef struct {
	int x1, x2, y1, y2;
} Rect;

static int sum(int n) { return (n + 1) * n / 2; }

static void list(int n) {
	int speed = n;
	int height = 0;
	const int w = 7;

	printf("\n%*s\t%*s\n", w, "height", w, "speed");
	printf("%*s\t%*s\n", w, "---", w, "---");
	while (height >= -n - 1) {
		printf("%*d\t%*d\n", w, height, w, speed);
		height += speed;
		speed--;
	}
}

int main() {
	FILE *file = fopen(input, "r");
	Rect target;
	fscanf(file, "target area: x=%d..%d, y=%d..%d", &target.x1, &target.x2,
	       &target.y1, &target.y2);
	fclose(file);

	printf("Target (%d,%d) -> (%d,%d)\n", target.x1, target.x2, target.y1,
	       target.y2);

	int n = -target.y1 - 1;
	list(n);

	printf("\nWith a starting velocity of %d we reach a maximum height of "
	       "%d.\n",
	       n, sum(n));
}
