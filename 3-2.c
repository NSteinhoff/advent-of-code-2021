#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    return strcpy(buffer,
                  "00100\n11110\n10110\n10111\n10101\n01111\n00111\n11100\n100"
                  "00\n11001\n00010\n01010\n\0");
}

static int split_lines(char *input, char **lines) {
    int n_lines = 0;
    char *line;
    char *string = input;
    while ((line = strsep(&string, "\n")) != NULL)
        if (*line != '\0') {
            lines[n_lines] = line;
            n_lines++;
        }
    return n_lines;
}

static int filter_lines(char **lines, char ***ret, char bit, int pos,
                        int size) {
    int n_filtered = 0;
    char **filtered = malloc(sizeof(char *) * (size_t)size);
    for (int line = 0; line < size; line++)
        if (lines[line][pos] == bit) {
            filtered[n_filtered] = lines[line];
            n_filtered++;
        }
    free(lines);
    if (n_filtered < size)
        filtered = realloc(filtered, sizeof(char *) * (size_t)n_filtered);
    *ret = filtered;
    return n_filtered;
}

static char *find_bit_pattern(char **lines, int size, bool most) {
    char **remaining = malloc(sizeof(char *) * (size_t)size);
    int n_remaining = 0;
    for (int line = 0; line < size; line++) {
        remaining[line] = lines[line];
        n_remaining++;
    }
    int pos = 0;
    while (n_remaining > 1) {
        int count = 0;
        for (int line = 0; line < n_remaining; line++) {
            count += remaining[line][pos] == '1' ? 1 : -1;
        }
        char bit = (most && count >= 0) || (!most && count < 0) ? '1' : '0';
        n_remaining =
            filter_lines(remaining, &remaining, bit, pos, n_remaining);
        pos++;
    }
    char *result = remaining[0];
    free(remaining);
    return result;
}

static int bits_to_number(char *bits) {
    int number = 0;
    for (int i = 0; i < (int)strlen(bits); i++) {
        number <<= 1;
        number |= bits[i] == '1';
    }
    return number;
}

static int life_support(char *input) {
    char *lines[N_LINES];
    int n_lines = split_lines(input, lines);
    char *oxy = find_bit_pattern(lines, n_lines, true);
    char *co2 = find_bit_pattern(lines, n_lines, false);
    return bits_to_number(oxy) * bits_to_number(co2);
}

int main() {
    char input[LINE_LENGTH * N_LINES + 1];
    printf("Life support rating: %d\n",
           life_support(TEST_INPUT ? test_input(input) : read_input(input)));
}
