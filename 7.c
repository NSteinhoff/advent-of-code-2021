/* --- Day 7: The Treachery of Whales ---

A giant whale has decided your submarine is its next meal, and it's much faster
than you are. There's nowhere to run!

Suddenly, a swarm of crabs (each in its own tiny submarine - it's too deep for
them otherwise) zooms in to rescue you! They seem to be preparing to blast a
hole in the ocean floor; sensors indicate a massive underground cave system
just beyond where they're aiming!

The crab submarines all need to be aligned before they'll have enough power to
blast a large enough hole for your submarine to get through. However, it
doesn't look like they'll be aligned before the whale catches you! Maybe you
can help?

There's one major catch - crab submarines can only move horizontally.

You quickly make a list of the horizontal position of each crab (your puzzle
input). Crab submarines have limited fuel, so you need to find a way to make
all of their horizontal positions match while requiring them to spend as little
fuel as possible.

For example, consider the following horizontal positions:

16,1,2,0,4,2,7,1,2,14

This means there's a crab with horizontal position 16, a crab with horizontal
position 1, and so on.

Each change of 1 step in horizontal position of a single crab costs 1 fuel. You
could choose any horizontal position to align them all on, but the one that
costs the least fuel is horizontal position 2:

    Move from 16 to 2: 14 fuel
    Move from 1 to 2: 1 fuel
    Move from 2 to 2: 0 fuel
    Move from 0 to 2: 2 fuel
    Move from 4 to 2: 2 fuel
    Move from 2 to 2: 0 fuel
    Move from 7 to 2: 5 fuel
    Move from 1 to 2: 1 fuel
    Move from 2 to 2: 0 fuel
    Move from 14 to 2: 12 fuel

This costs a total of 37 fuel. This is the cheapest possible outcome; more
expensive outcomes include aligning at position 1 (41 fuel), position 3 (39
fuel), or position 10 (71 fuel).

Determine the horizontal position that the crabs can align to using the least
fuel possible. How much fuel must they spend to align to that position?

--- Part Two ---

The crabs don't seem interested in your proposed solution. Perhaps you
misunderstand crab engineering?

As it turns out, crab submarine engines don't burn fuel at a constant rate.
Instead, each change of 1 step in horizontal position costs 1 more unit of fuel
than the last: the first step costs 1, the second step costs 2, the third step
costs 3, and so on.

As each crab moves, moving further becomes more expensive. This changes the best
horizontal position to align them all on; in the example above, this becomes 5:

    Move from 16 to 5: 66 fuel
    Move from 1 to 5: 10 fuel
    Move from 2 to 5: 6 fuel
    Move from 0 to 5: 15 fuel
    Move from 4 to 5: 1 fuel
    Move from 2 to 5: 6 fuel
    Move from 7 to 5: 3 fuel
    Move from 1 to 5: 10 fuel
    Move from 2 to 5: 6 fuel
    Move from 14 to 5: 45 fuel

This costs a total of 168 fuel. This is the new cheapest possible outcome; the
old alignment position (2) now costs 206 fuel instead.

Determine the horizontal position that the crabs can align to using the least
fuel possible so they can make you an escape route! How much fuel must they
spend to align to that position?
*/

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
