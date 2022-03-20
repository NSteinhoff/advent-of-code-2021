#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wvla"

#ifdef TEST
#define LINE_LENGTH 6
#define N_LINES 12
#define TEST_INPUT true
#else
#define LINE_LENGTH 13
#define N_LINES 1000
#define TEST_INPUT false
#endif

static char *read_input(char *buffer) {
    FILE *file = fopen("3.txt", "r");
    size_t bytes_read =
        fread(buffer, sizeof(char), LINE_LENGTH * N_LINES, file);
    fclose(file);
    buffer[bytes_read] = '\0'; // Make sure it's terminated

    return buffer;
}

static char *test_input(char *buffer) {
    strcpy(buffer, "00100\n11110\n10110\n10111\n10101\n01111\n00111\n11100\n100"
                   "00\n11001\n00010\n01010\n\0");

    return buffer;
}

static int split_lines(char *input, char **lines) {
    int n_lines = 0;
    char *line;
    char *string = input;

    while ((line = strsep(&string, "\n")) != NULL) {
        // Add non-empty lines
        if (*line != '\0') {
            lines[n_lines] = line;
            n_lines++;
        }
    }

    return n_lines;
}

static int power_consumption(char *input) {
    char *lines[N_LINES];
    int n_lines = split_lines(input, lines);
    int n_bits = (int)strlen(lines[0]);
    int bit_counts[n_bits];

    for (int i = 0; i < (int)n_bits; i++)
        bit_counts[i] = 0;

    for (int i = 0; i < n_lines; i++) {
        for (int j = 0; j < (int)n_bits; j++) {
            char c = lines[i][j];
            if (c == '1')
                bit_counts[j]++;
            if (c == '0')
                bit_counts[j]--;
        }
    }

    int gamma = 0;
    for (int i = 0; i < n_bits; i++) {
        gamma = gamma << 1;
        gamma = gamma | (bit_counts[i] >= 0 ? 1 : 0);
    }

    int epsilon = 0;
    for (int i = 0; i < n_bits; i++) {
        epsilon = epsilon << 1;
        epsilon = epsilon | (bit_counts[i] < 0 ? 1 : 0);
    }

    return gamma * epsilon;
}

int main() {
    char input[LINE_LENGTH * N_LINES + 1];
    printf(
        "Power consumption: %d\n",
        power_consumption(TEST_INPUT ? test_input(input) : read_input(input)));
}
