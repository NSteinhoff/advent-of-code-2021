#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void one(FILE *file) {
	printf("--- Part One ---\n");
	char command[8];
	int magnitude;

	int depth = 0;
	int horiz = 0;
	while (fscanf(file, "%s %d\n", command, &magnitude) != EOF) {
		if (strcmp(command, "up") == 0)
			depth -= magnitude;
		if (strcmp(command, "down") == 0)
			depth += magnitude;
		if (strcmp(command, "forward") == 0)
			horiz += magnitude;
	}

	printf("depth = %d, horiz = %d\n", depth, horiz);
	printf("=> %d\n", depth * horiz);
}

void two(FILE *file) {
	printf("--- Part Two ---\n");
	char command[8];
	int magnitude;

	int depth = 0;
	int horiz = 0;
	int aim = 0;
	while (fscanf(file, "%s %d\n", command, &magnitude) != EOF) {
		if (strcmp(command, "up") == 0)
			aim -= magnitude;
		if (strcmp(command, "down") == 0)
			aim += magnitude;
		if (strcmp(command, "forward") == 0) {
			horiz += magnitude;
			depth += magnitude * aim;
		}
	}
	printf("depth = %d, horiz = %d, aim = %d\n", depth, horiz, aim);
	printf("=> %d\n", depth * horiz);
}

int main() {
	FILE *file = fopen("2.txt", "r");
	one(file);
	rewind(file);
	two(file);
	fclose(file);
}
