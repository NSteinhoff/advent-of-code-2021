/** --- Day 4: Giant Squid ---
 * You're already almost 1.5km (almost a mile) below the surface of the ocean,
 * already so deep that you can't see any sunlight. What you can see, however,
 * is a giant squid that has attached itself to the outside of your submarine.
 *
 * Maybe it wants to play bingo?
 *
 * Bingo is played on a set of boards each consisting of a 5x5 grid of numbers.
 * Numbers are chosen at random, and the chosen number is marked on all boards
 * on which it appears. (Numbers may not appear on all boards.) If all numbers
 * in any row or any column of a board are marked, that board wins. (Diagonals
 * don't count.)
 *
 * The submarine has a bingo subsystem to help passengers (currently, you and
 * the giant squid) pass the time. It automatically generates a random order in
 * which to draw numbers and a random set of boards (your puzzle input). For
 * example:
 *
 * 7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1
 *
 * 22 13 17 11  0
 *  8  2 23  4 24
 * 21  9 14 16  7
 *  6 10  3 18  5
 *  1 12 20 15 19
 *
 *  3 15  0  2 22
 *  9 18 13 17  5
 * 19  8  7 25 23
 * 20 11 10 24  4
 * 14 21 16 12  6
 *
 * 14 21 17 24  4
 * 10 16 15  9 19
 * 18  8 23 26 20
 * 22 11 13  6  5
 *  2  0 12  3  7
 *
 * After the first five numbers are drawn (7, 4, 9, 5, and 11), there are no
 * winners, but the boards are marked as follows (shown here adjacent to each
 * other to save space):
 *
 * 22 13 17 11  0         3 15  0  2 22        14 21 17 24  4
 *  8  2 23  4 24         9 18 13 17  5        10 16 15  9 19
 * 21  9 14 16  7        19  8  7 25 23        18  8 23 26 20
 *  6 10  3 18  5        20 11 10 24  4        22 11 13  6  5
 *  1 12 20 15 19        14 21 16 12  6         2  0 12  3  7
 *
 * After the next six numbers are drawn (17, 23, 2, 0, 14, and 21), there are
 * still no winners:
 *
 * 22 13 17 11  0         3 15  0  2 22        14 21 17 24  4
 *  8  2 23  4 24         9 18 13 17  5        10 16 15  9 19
 * 21  9 14 16  7        19  8  7 25 23        18  8 23 26 20
 *  6 10  3 18  5        20 11 10 24  4        22 11 13  6  5
 *  1 12 20 15 19        14 21 16 12  6         2  0 12  3  7
 *
 * Finally, 24 is drawn:
 *
 * 22 13 17 11  0         3 15  0  2 22        14 21 17 24  4
 *  8  2 23  4 24         9 18 13 17  5        10 16 15  9 19
 * 21  9 14 16  7        19  8  7 25 23        18  8 23 26 20
 *  6 10  3 18  5        20 11 10 24  4        22 11 13  6  5
 *  1 12 20 15 19        14 21 16 12  6         2  0 12  3  7
 *
 * At this point, the third board wins because it has at least one complete row
 * or column of marked numbers (in this case, the entire top row is marked: 14
 * 21 17 24 4).
 *
 * The score of the winning board can now be calculated. Start by finding the
 * sum of all unmarked numbers on that board; in this case, the sum is 188.
 * Then, multiply that sum by the number that was just called when the board
 * won, 24, to get the final score, 188 * 24 = 4512.
 *
 * To guarantee victory against the giant squid, figure out which board will win
 * first. What will your final score be if you choose that board?
 *
 * --- Part Two ---
 *
 * On the other hand, it might be wise to try a different strategy: let the
 * giant squid win.
 *
 * You aren't sure how many bingo boards a giant squid could play at once, so
 * rather than waste time counting its arms, the safe thing to do is to figure
 * out which board will win last and choose that one. That way, no matter which
 * boards it picks, it will win for sure.
 *
 * In the above example, the second board is the last to win, which happens
 * after 13 is eventually called and its middle column is completely marked. If
 * you were to keep playing until this point, the second board would have a sum
 * of unmarked numbers equal to 148 for a final score of 148 * 13 = 1924.
 *
 * Figure out which board will win last. Once it wins, what would its final
 * score be?
 */
#include <assert.h>
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

void one(const int *numbers, int n_numbers, const int *boards, int n_boards) {
	printf("--- Part One ---\n");
	int *boards_cpy = malloc(sizeof(int) * n_boards);
	memcpy(boards_cpy, boards, sizeof(int) * n_boards);

	Winner winner;
	for (int n = 0; n < n_numbers; n++) {
		for (int i = 0; i < n_boards; i++) {
			if (boards_cpy[i] == numbers[n]) {
				boards_cpy[i] = -1;
				if (check_board(boards_cpy +
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
	       calculate_score(boards_cpy + winner.b * LEN_BOARD, winner.n));

	free(boards_cpy);
}

void two(const int *numbers, int n_numbers, const int *boards, int n_boards) {
	printf("--- Part Two ---\n");
	int *boards_cpy = malloc(sizeof(int) * n_boards);
	memcpy(boards_cpy, boards, sizeof(int) * n_boards);

	int n_winners = 0;
	Winner winners[MAX_BOARDS] = {0};

	for (int n = 0; n < n_numbers; n++) {
		for (int i = 0; i < n_boards; i++) {
			for (int w = 0; w < n_winners; w++)
				if (winners[w].b == i / LEN_BOARD)
					goto next;
			if (boards_cpy[i] == numbers[n]) {
				boards_cpy[i] = -1;
				if (check_board(boards_cpy +
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
	       calculate_score(boards_cpy + first.b * LEN_BOARD, first.n));
	printf("nth: %2d / %2d => %d\n", last.b, last.n,
	       calculate_score(boards_cpy + last.b * LEN_BOARD, last.n));

	free(boards_cpy);
}

int main() {
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

	one(numbers, n_numbers, boards, n_boards);
	two(numbers, n_numbers, boards, n_boards);

	free(tofree);
	return 0;
}
