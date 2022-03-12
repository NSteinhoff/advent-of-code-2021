#include <stdio.h>
#include <stdlib.h>

#define BUF_MAX 6 // 4 digits + newline + space for '\0' terminator

int main() {
    char line[BUF_MAX];
    FILE *file = fopen("1.txt", "r");

    int current = 0;
    int previous = 0;
    int count = 0;

    while (fgets(line, BUF_MAX, file)) {
        current = atoi(line);

        if (previous)
            if (current > previous)
                count++;

        previous = current;
    }
    fclose(file);

    printf("Count: %d\n", count);
}
