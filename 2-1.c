#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 32

int main() {
    FILE *file = fopen("2.txt", "r");
    char line[MAX_LINE_LENGTH];
    char command[16];
    int magnitude;

    int depth = 0;
    int horiz = 0;
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        sscanf(line, "%s %d", command, &magnitude);

        if (strcmp(command, "up") == 0)
            depth -= magnitude;
        if (strcmp(command, "down") == 0)
            depth += magnitude;
        if (strcmp(command, "forward") == 0)
            horiz += magnitude;
    }

    printf("depth = %d, horiz = %d\n", depth, horiz);
    printf("depth * horiz = %d\n", depth * horiz);
}
