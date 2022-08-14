#include <stdio.h>
#include "read.h"

#define INPUT "18-example.txt"

int main() {
	char *content = read_to_string(INPUT);
	if (!content) return EXIT_FAILURE;

	printf("%s", content);

	free(content);
}
