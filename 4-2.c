#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "read.h"

#define INPUT "4.txt"
#define N_ROWS 5
#define N_COLS 5
#define LEN_BOARD 25
#define MAX_BOARDS 100
#define MAX_NUMBERS 100

typedef struct {
	int b, n;
} Winner;

static bool check_board(int *board) {
	int rows[N_ROWS] = {0};
	int cols[N_COLS] = {0};

	for (int row = 0; row < N_ROWS; row++) {
		for (int col = 0; col < N_COLS; col++)
			if (board[row * 5 + col] == -1) {
				rows[row]++;
				cols[col]++;
				if (rows[row] == 5 || cols[col] == 5)
					return true;
			}
	}
	return false;
}

static int calculate_score(const int *board, int number) {
	int sum_unmarked = 0;
	for (int i = 0; i < LEN_BOARD; i++) {
		if (board[i] != -1)
			sum_unmarked += board[i];
	}

	return sum_unmarked * number;
}

int main(void) {
	char *content = read_to_string(INPUT);
	char *tofree = content;

	// Setup the input data
	int n_numbers = 0;
	int numbers[MAX_NUMBERS];
	char *number_line = strsep(&content, "\n");
	for (char *num; (num = strsep(&number_line, ", "));)
		numbers[n_numbers++] = atoi(num);

	int n_boards = 0;
	int boards[LEN_BOARD * MAX_BOARDS] = {0};
	for (char *num; (num = strsep(&content, " \n"));)
		if (strcmp(num, "") != 0)
			boards[n_boards++] = atoi(num);

	free(tofree);

	int n_winners = 0;
	Winner winners[MAX_BOARDS] = {0};

	for (int n = 0; n < n_numbers; n++) {
		for (int i = 0; i < n_boards; i++) {
			for (int w = 0; w < n_winners; w++)
				if (winners[w].b == i / LEN_BOARD)
					goto next;
			if (boards[i] == numbers[n]) {
				boards[i] = -1;
				if (check_board(boards +
				                (i / LEN_BOARD) * LEN_BOARD)) {
					winners[n_winners++] =
					    (Winner){.b = i / LEN_BOARD,
					             .n = numbers[n]};
					continue;
				}
			}
		next:;
		}
	}

	Winner first = winners[0];
	Winner last = winners[n_winners - 1];

	printf("1st: %2d / %2d => %d\n", first.b, first.n,
	       calculate_score(boards + first.b * LEN_BOARD, first.n));
	printf("nth: %2d / %2d => %d\n", last.b, last.n,
	       calculate_score(boards + last.b * LEN_BOARD, last.n));

	return 0;
}
