#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define INPUT "15-example.txt"
// Convert milliseconds to nanoseconds
#define MS(A) (A) * 1000000
#define WATCH 0

// Are we connected to a terminal? Set this at the start of `main()` as it does
// not change over the course of the program.
static bool tty;

typedef struct Cell {
	struct Cell *came_from;
	struct Cell *next_in_queue;
	size_t x, y;
	size_t cost;
	size_t total;
} Cell;

typedef struct {
	size_t x, y;
} Coord;

typedef struct {
	Coord dimensions;
	Cell *cells;
} Map;

typedef struct Queue {
	Cell *head;
} Queue;

static Cell *queue_pop(Queue *queue) {
	if (!queue->head) {
		return NULL;
	}

	Cell *head = queue->head;
	queue->head = head->next_in_queue;

	return head;
}

static Cell *queue_remove(Queue *queue, Cell *cell) {
	if (!queue->head) {
		return NULL;
	}

	if (queue->head == cell) {
		queue->head = cell->next_in_queue;

		return cell;
	}

	for (Cell *head = queue->head; head->next_in_queue;
	     head = head->next_in_queue) {
		if (head->next_in_queue == cell) {
			head->next_in_queue = cell->next_in_queue;
			return cell;
		}
	}

	return NULL;
}

static void queue_push(Queue *queue, Cell *cell) {
	assert(queue);
	assert(cell);

	if (!queue->head) {
		queue->head = cell;
		return;
	}

	if (cell->total < queue->head->total) {
		cell->next_in_queue = queue->head;
		queue->head = cell;

		return;
	}

	for (Cell *head = queue->head; head; head = head->next_in_queue) {
		if (!head->next_in_queue ||
		    cell->total < head->next_in_queue->total) {
			cell->next_in_queue = head->next_in_queue;
			head->next_in_queue = cell;

			return;
		}
	}
}

static void test_queue() {
	Queue q = {0};
	Cell c0 = {.total = 9};
	Cell c1 = {.total = 0};
	Cell c2 = {.total = 1};
	Cell c3 = {.total = 2};
	Cell c4 = {.total = 0};
	Cell c5 = {.total = 2};

	assert(!q.head);
	queue_push(&q, &c1);
	assert(q.head == &c1);
	assert(queue_pop(&q) == &c1);
	assert(!q.head);

	queue_push(&q, &c1);
	queue_push(&q, &c2);
	assert(q.head == &c1);
	assert(queue_pop(&q) == &c1);
	assert(q.head == &c2);
	assert(queue_pop(&q) == &c2);
	assert(!q.head);

	queue_push(&q, &c0);
	queue_push(&q, &c1);
	queue_push(&q, &c2);
	queue_push(&q, &c3);
	queue_push(&q, &c4);
	queue_push(&q, &c5);

	assert(queue_pop(&q) == &c1);
	assert(queue_pop(&q) == &c4);
	assert(queue_pop(&q) == &c2);
	assert(queue_pop(&q) == &c3);
	assert(queue_pop(&q) == &c5);
	assert(queue_pop(&q) == &c0);
	assert(!queue_pop(&q));

	queue_push(&q, &c0);
	queue_push(&q, &c1);
	queue_push(&q, &c2);
	queue_push(&q, &c3);
	queue_push(&q, &c4);
	queue_push(&q, &c5);

	queue_remove(&q, &c1);
	queue_remove(&q, &c3);
	assert(queue_pop(&q) == &c4);
	assert(queue_pop(&q) == &c2);
	assert(queue_pop(&q) == &c5);
	assert(queue_pop(&q) == &c0);
	assert(!queue_pop(&q));
}

/// Pretty print a digit
static void pprint(size_t cost, bool is_path, bool is_candidate) {
	if (tty && is_path)
		printf("\033[1;%sm", is_candidate ? "34" : "35");

	printf("%zu", cost);

	if (tty && is_path)
		printf("\033[m");
}

/// Return the map dimensions
static Coord get_map_dimensions(FILE *file) {
	assert(file);

	size_t rows = 0;
	size_t chars = 0;

	for (int c = getc(file); c != EOF; c = getc(file)) {
		if (c == '\n')
			rows++;
		else
			chars++;
	}

	return (Coord){.x = (chars / rows) * 5, .y = rows * 5};
}

/// Check if the path includes a point
static bool path_includes(const Cell *head, const Cell *cell) {
	assert(head);
	assert(cell);

	for (const Cell *h = head; h; h = h->came_from)
		if (h == cell)
			return true;
	return false;
}

/// Create the map as an array of `Cell`s
static Map create_map(FILE *file) {
	assert(file);

	Coord dimensions = get_map_dimensions(file);
	rewind(file);

	Cell *cells = calloc(dimensions.x * dimensions.y, sizeof(Cell));
	assert(cells);

	const size_t y_offset = dimensions.y / 5;
	const size_t x_offset = dimensions.x / 5;

	int c;
	for (size_t y = 0; y < y_offset; y++)
		for (size_t x = 0; x < x_offset; x++) {
			if (y > 0 && x == 0)
				assert((c = getc(file)) == '\n');
			c = getc(file);
			cells[y * dimensions.x + x] = (Cell){
				.cost = (size_t)(c - '0'),
				.came_from = NULL,
				.x = x,
				.y = y,
				.total = SIZE_MAX,
			};
		}

	for (size_t y = 0; y < dimensions.y; y++)
		for (size_t x = 0; x < dimensions.x; x++) {
			if (y < y_offset && x < x_offset)
				continue;

			size_t index =
				(y >= y_offset && x < x_offset ? y - y_offset
							       : y) *
					dimensions.x +
				(x >= x_offset ? x - x_offset : x);

			const Cell *base = &cells[index];
			cells[y * dimensions.x + x] = (Cell){
				.cost = base->cost + 1 > 9 ? 1 : base->cost + 1,
				.came_from = NULL,
				.x = x,
				.y = y,
				.total = SIZE_MAX,
			};
		}

	printf("Map: %zu x %zu\n", dimensions.y, dimensions.x);
	return (Map){.cells = cells, .dimensions = dimensions};
}

static void map_clear(const Map *map) {
	assert(map);

	// Move cursor up
	printf("\033[%zu", map->dimensions.y +
				   /* borders */ 2 +
				   /* results */ 1);

	// Clear to the end of the screen
	printf("F\033[J");
}

/// Print map and highlight path when supported.
static void map_print(const Map *map, const Cell *head, const Cell *candidate) {
	assert(head);
	static size_t n = 0;

	if (n++ > 0 && tty)
		map_clear(map);

	for (size_t i = 0; i < map->dimensions.y * map->dimensions.x; i++) {
		if (i == 0) {
			// top border
			printf("+");
			for (size_t i = 0; i < map->dimensions.x; i++)
				printf("-");
			printf("+\n");
		}

		const Cell *cell = &map->cells[i];
		assert(cell);

		if (i % map->dimensions.x == 0)
			printf("|"); // left border
		bool is_candidate = cell == candidate;
		bool on_path = path_includes(head, cell) || is_candidate;
		pprint(cell->cost, on_path, is_candidate);
		if (i % map->dimensions.x == map->dimensions.x - 1)
			printf("|\n"); // right border
	}

	// bottom border
	printf("+");
	for (size_t i = 0; i < map->dimensions.x; i++)
		printf("-");
	printf("+\n");

	printf("Risk(n=%zu): %zu\n", n, head ? head->total : 0);

	if (WATCH && tty) {
		struct timespec req = {.tv_nsec = MS(50), .tv_sec = 0};
		nanosleep(&req, NULL);
	}
}

/// Return the next cell to check
static Cell *next(const Map *map, Cell *head, Queue *queue) {
	assert(map);
	assert(head);
	assert(queue);

	// For all neighbors still in the queue
	for (size_t y = head->y > 0 ? head->y - 1 : head->y;
	     y <= (head->y < map->dimensions.y - 1 ? head->y + 1 : head->y);
	     y++)
		for (size_t x = head->x > 0 ? head->x - 1 : head->x;
		     x <=
		     (head->x < map->dimensions.x - 1 ? head->x + 1 : head->x);
		     x++) {
			if ((x == head->x && y == head->y) || // not head itself
			    (x != head->x && y != head->y))   // no diagonals
				continue;

			Cell *cell = queue_remove(
				queue, &map->cells[y * map->dimensions.x + x]);
			if (!cell)
				continue;

			if (WATCH && tty)
				map_print(map, head, cell);

			assert(cell->came_from != head);

			size_t new_total = head->total + cell->cost;
			if (new_total < cell->total) {
				cell->total = new_total;
				cell->came_from = head;
			}

			queue_push(queue, cell);
		}

	return queue_pop(queue);
}

int main() {
	// Run some tests on the priority queue implementation
	test_queue();

	// Check if we are connected to a terminal, in which case we can use
	// colors and update the screen.
	tty = isatty(1);

	// Read the file and create a map from it. The file is no longer needed
	// afterwards.
	FILE *file = fopen(INPUT, "r");
	Map map = create_map(file);
	fclose(file);

	// We start in the top left corner which is the first cell in our map.
	Cell *start = &map.cells[0];
	start->total = 0;

	Queue queue = {0};
	for (size_t i = 0; i < map.dimensions.x * map.dimensions.y; i++) {
		queue_push(&queue, &map.cells[i]);
	}

	// The goal is to reach the bottom right corner, which so happens to be
	// the last element in our array.
	Cell *target = &map.cells[map.dimensions.y * map.dimensions.y - 1];

	// The current head of our path. This should be the next cell to visit,
	// i.e. the cell with the highest priority in our queue.
	Cell *head = next(&map, queue_pop(&queue), &queue);

	// Iterate until the `head` is `target` or the queue is empty.
	while (head != target && queue.head) {
		if (WATCH && tty)
			map_print(&map, head, NULL);

		head = next(&map, head, &queue);
	}
	map_print(&map, head, NULL);

	// Cleanup is quite simple, because all the cells are contained in our
	// array. The path and the queue are assembled from references to those
	// cells, so no additional memory allocation is required.
	free(map.cells);

	return 0;
}
