#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define BUF_MAX 6 // 4 digits + newline + space for '\0' terminator

int main() {
    char line[BUF_MAX];
    FILE *file = fopen("1.txt", "r");

    int current = 0;
    int previous = INT_MAX;
    int count = 0;

    int window[3] = {0};
    int l = 0;
    while (fgets(line, BUF_MAX, file)) {
        window[l % 3] = atoi(line);
        l++;

        if (l >= 3) {
            current = 0;
            for (int i = 0; i < 3; i++) {
                current += window[i];
            }

            if (current > previous)
                count++;

            previous = current;
        }
    }
    fclose(file);

    printf("Count: %d\n", count);
}
