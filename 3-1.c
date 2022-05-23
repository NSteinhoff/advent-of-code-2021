#include <stdio.h>

#define MAX_BITS 12
#define INPUT "3.txt"

int main() {
	FILE *file = fopen(INPUT, "r");
	int counts[MAX_BITS] = {0};
	int pos = 0;
	int n_bits = 0;

	for (char c; (c = fgetc(file)) != EOF;) {
		switch (c) {
		case '\n':
			if (!n_bits)
				n_bits = pos;
			pos = 0;
			continue;
		case '1':
			counts[pos]++;
			break;
		case '0':
			counts[pos]--;
			break;
		}
		pos++;
	}

	int g = 0;
	int e = 0;
	for (pos = 0; pos < n_bits; pos++) {
		g = g << 1 | (counts[pos] >= 0 ? 1 : 0);
		e = e << 1 | (counts[pos] >= 0 ? 0 : 1);
	}

	printf("=> %d\n", g * e);
	fclose(file);
}
