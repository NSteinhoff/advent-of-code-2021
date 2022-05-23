#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

bool mark_boards(int *boards, int len, int number, Winner *ret) {
	int board = 0;

	for (int i = 0; i < len; i++) {
		if (boards[i] == number) {
			board = i / LEN_BOARD;
			boards[i] = -1;
			if (check_board(boards + LEN_BOARD * board)) {
				*ret = (Winner){.b = board, .n = number};
				return true;
			}
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
	// Read the input file into a dynamically allocated string buffer.
	FILE *file = fopen(INPUT, "r");
	fseek(file, 0L, SEEK_END);
	size_t size = (size_t)ftell(file);
	fseek(file, 0L, SEEK_SET);
	char *content = malloc(size + 1);
	char *tofree = content;
	size_t bytes_read = fread(content, sizeof(char), size, file);
	content[bytes_read] = '\0';
	fclose(file);

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

	Winner winner;
	for (int n = 0; n < n_numbers; n++) {
		for (int i = 0; i < n_boards; i++) {
			if (boards[i] == numbers[n]) {
				boards[i] = -1;
				if (check_board(boards +
				                (i / LEN_BOARD) * LEN_BOARD)) {
					winner = (Winner){.b = i / LEN_BOARD,
					                  .n = numbers[n]};
					goto finally;
				}
			}
		}
	}

finally:
	printf("1st: %2d / %2d => %d\n", winner.b, winner.n,
	       calculate_score(boards + winner.b * LEN_BOARD, winner.n));

	return 0;
}
