#include <stdio.h>
#include <stdlib.h>

#define INPUT "13-example.txt"

int main(void) {
	FILE *file = fopen(INPUT, "r");
	if (!file) {
		perror(INPUT);
		exit(EXIT_FAILURE);
	}

	return 0;
}
