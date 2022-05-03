#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT "7.txt"

#define abs(a) ((a) > 0 ? (a) : -(a))

typedef struct Crab Crab;

struct Crab {
	int position;
	Crab *next;
};

Crab *Crab_add(Crab *next, int position) {
	Crab *crab = malloc(sizeof(Crab));
	crab->position = position;
	crab->next = next;

	return crab;
}

void Crab_free(Crab *crab) {
	Crab *next = crab->next;
	free(crab);
	if (next)
		Crab_free(next);
}

static int linear_cost(int distance) { return distance; }
static int scaling_cost(int distance) { return distance * (distance + 1) / 2; }

static void find_target(Crab *head_crab, int position_max,
                        int (*get_cost)(int)) {
	int best_target = 0;
	int best_cost = INT_MAX;
	for (int target = 0; target <= position_max; target++) {
		int cost = 0;
		for (Crab *crab = head_crab; crab; crab = crab->next) {
			int distance = abs(target - crab->position);
			cost += get_cost(distance);
		}

		if (cost < best_cost) {
			best_target = target;
			best_cost = cost;
		}
	}

	printf("Target: %2d\nCost: %d\n", best_target, best_cost);
}

int main() {
	FILE *fp = fopen(INPUT, "r");
	Crab *head_crab = NULL;
	int position_max = 0;
	for (int position; fscanf(fp, "%d,", &position) != EOF;) {
		position_max =
		    position > position_max ? position : position_max;
		head_crab = Crab_add(head_crab, position);
	}

	printf("--- Part One ---\n");
	find_target(head_crab, position_max, linear_cost);
	printf("--- Part Two ---\n");
	find_target(head_crab, position_max, scaling_cost);

	Crab_free(head_crab);
	return 0;
}
