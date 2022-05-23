#include <stdio.h>
#include <string.h>

#define INPUT "8.txt"
#define SIGNAL_LENGTH 8
#define N_OUTPUTS 4
#define N_SIGNALS 10


int main(void) {
	FILE *file = fopen(INPUT, "r");

	char in[N_SIGNALS][SIGNAL_LENGTH];
	char out[N_OUTPUTS][SIGNAL_LENGTH];

	int count = 0;
	while (fscanf(file, "%s %s %s %s %s %s %s %s %s %s | %s %s %s %s",
	              in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7],
	              in[8], in[9], out[0], out[1], out[2], out[3]) != EOF)
		for (int i = 0; i < N_OUTPUTS; i++) {
			int len = strlen(out[i]);
			if (len == 2 || len == 3 || len == 4 || len == 7)
				count++;
		}

	printf("Count: %d\n", count);

	fclose(file);

	return 0;
}
