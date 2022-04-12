#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "4.txt"
#define N_ROWS 5
#define N_COLS 5
#define LEN_BOARD 25
#define INIT_CAPACITY 64
#define GROW_CAPACITY 2

typedef struct {
	size_t len;
	size_t capacity;
	int *elements;
} Array;

static Array array_new() {
	return (Array){
	    .len = 0,
	    .capacity = INIT_CAPACITY,
	    .elements = malloc(sizeof(int) * INIT_CAPACITY),
	};
}

static void array_push(Array *a, int number) {
	if (a->len < a->capacity) {
		a->elements[a->len++] = number;
		return;
	}

	size_t new_capacity = a->capacity * GROW_CAPACITY;
	a->elements =
	    realloc(a->elements, sizeof(int) * new_capacity);
	a->capacity = new_capacity;
	array_push(a, number);
}

static size_t file_size(FILE *fp) {
	fseek(fp, 0L, SEEK_END);
	size_t file_size = (size_t)ftell(fp);
	rewind(fp);

	return file_size;
}

static char *read(FILE *fp) {
	size_t size = file_size(fp);
	char *s = malloc(size + 1);
	size_t bytes_read = fread(s, sizeof(char), size, fp);
	fclose(fp);

	s[bytes_read] = '\0';
	return s;
}

static Array parse_numbers(char *s, const char *sep) {
	Array a = array_new();
	char *num;
	while ((num = strsep(&s, sep)))
		if (strcmp(num, "") != 0)
			array_push(&a, atoi(num));

	return a;
}

static bool is_winner(int *board) {
	for (int row = 0; row < N_ROWS; ++row) {
		int marked = 0;
		for (int col = 0; col < N_COLS; ++col) {
			if (board[row * 5 + col] == -1)
				++marked;
		}
		if (marked == 5)
			return true;
	}

	for (int col = 0; col < N_COLS; ++col) {
		int marked = 0;
		for (int row = 0; row < N_COLS; ++row) {
			if (board[row * 5 + col] == -1)
				++marked;
		}
		if (marked == 5)
			return true;
	}

	return false;
}

static int calculate_score(int winner, int winning_number, Array *boards) {
	int sum_unmarked = 0;
	for (size_t i = 0; i < LEN_BOARD; ++i) {
		int number = boards->elements[(size_t)winner * LEN_BOARD + i];
		if (number != -1)
			sum_unmarked += number;
	}
	return sum_unmarked * winning_number;
}

int main() {
	char *input = read(fopen(INPUT, "r"));
	char *next = input;

	Array numbers = parse_numbers(strsep(&next, "\n"), ",");
	Array boards = parse_numbers(next, " \n");
	free(input);
	Array winners = array_new();
	Array winning_numbers = array_new();

	for (size_t i = 0; i < numbers.len; ++i) {
		int number = numbers.elements[i];
		int board = 0;
		bool has_won = false;
		for (size_t j = 0; j < boards.len; ++j) {
			if (j % LEN_BOARD == 0) {
				board = (int)(j / LEN_BOARD);
			}
			for (size_t winner = 0; winner < winners.len; ++winner)
				if (board == winners.elements[winner])
					has_won = true;
			if (has_won)
				break;
			if (boards.elements[j] == number) {
				boards.elements[j] = -1;
				if (is_winner(
				        &boards.elements[board * LEN_BOARD])) {
					array_push(&winners, board);
					array_push(&winning_numbers, number);
				}
			}
		}
	}
	free(numbers.elements);

	int winner = winners.elements[0];
	int winning_number = winning_numbers.elements[0];
	printf("First: #%-3d with %3d    => %9d\n", winner + 1, winning_number,
	       calculate_score(winner, winning_number, &boards));

	winner = winners.elements[winners.len - 1];
	winning_number = winning_numbers.elements[winners.len - 1];
	printf("Last:  #%-3d with %3d    => %9d\n", winner + 1, winning_number,
	       calculate_score(winner, winning_number, &boards));

	free(boards.elements);

	return 0;
}
