#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "8.txt"
#define N_SEGMENTS 7
#define SIGNAL_LENGTH 8
#define N_OUTPUTS 4
#define N_SIGNALS 10
#define N_PERMUTATIONS 5040 // 7 segments mean 7! permutations

const char segments[SIGNAL_LENGTH] = "abcdefg";

/// Lookup table of the original singal strings. Each numeric index maps to the
/// corresponding signal.
const char *signals[N_SIGNALS] = {
	/* 0 */ "abcefg",
	/* 1 */ "cf",
	/* 2 */ "acdeg",
	/* 3 */ "acdfg",
	/* 4 */ "bcdf",
	/* 5 */ "abdfg",
	/* 6 */ "abdefg",
	/* 7 */ "acf",
	/* 8 */ "abcdefg",
	/* 9 */ "abcdfg",
};

/// Array of all permutations of the segments. It's constant, but we generate
/// it each time at the start of `main`.
char permutations[N_PERMUTATIONS][SIGNAL_LENGTH];
size_t n_permutations = 0;

/// Swap two characters in place.
static inline void swap(char *s, size_t a, size_t b) {
	if (a == b)
		return;
	char tmp = s[a];
	s[a] = s[b];
	s[b] = tmp;
}

/// Create all permutations of the first `n` characters of the input string.
/// abc -> bac -> cab -> acb -> bca -> cba
/// ^^     ^ ^    ^^     ^ ^    ^^
void permutate(char *pattern, size_t n) {
	if (n <= 1) {
		strcpy(permutations[n_permutations++], pattern);
		return;
	}

	for (size_t i = 0; i < n; i++) {
		permutate(pattern, n - 1);
		swap(pattern, n % 2 == 0 ? i : 0, n - 1);
	}
}

/// Return the signal, if any, which can be decoded from the pattern using the
/// mapping.
///
/// f         c          a
/// fbcdeag fbcdeag fbcdeag
/// abcdefg abcdefg abcdefg
/// a         c          f   -> acf -> 7
///
/// d         e          a
/// dbefcag dbefcag dbefcag
/// abcdefg abcdefg abcdefg
/// a         c          f   -> acf -> 7
///
///   e     d            a
/// dbefcag dbefcag dbefcag
/// abcdefg abcdefg abcdefg
///   c     a            f   -> caf -> acf -> 7
int pattern_to_signal(const char *pattern, const char *mapping) {
	// What is the length of the incoming pattern
	size_t len = strlen(pattern);

	// Find a signal that is matched by the pattern using the provided
	// mapping.
	for (int s = 0; s < N_SIGNALS; s++) {
		// The incoming pattern and the pattern for a given signal must
		// have equal length for them to match.
		if (strlen(signals[s]) != len)
			continue;

		// Go throught each letter in the pattern
		for (size_t i = 0; i < len; i++) {
			// Find the position of that letter in the mapping
			for (int j = 0; j < N_SEGMENTS; j++) {
				if (pattern[i] == mapping[j])
					// Find the original letter by the
					// position of our pattern's letter in
					// the mapping and check if it is part
					// of the signal.
					if (!strchr(signals[s], segments[j]))
						// This letter is not part of
						// this signal.
						// Check the next signal!
						goto next;
			}
		}

		// The pattern matches the signal given the mapping, so we
		// return the signal.
		return s;
	next:;
	}

	// The pattern does not match any of the signals given the mapping.
	return -1;
}

/// Check if the mapping can decode all signals
bool matches_all_signals(const char *mapping, char patterns[][SIGNAL_LENGTH]) {
	int matches[N_SIGNALS] = {0};

	for (int j = 0; j < N_SIGNALS; j++) {
		int signal = pattern_to_signal(patterns[j], mapping);
		if (signal == -1)
			// If we fail to match a signal, then this cannot be the
			// correct mapping
			return false;
		matches[signal]++;
	}

	for (int k = 0; k < N_SIGNALS; k++)
		// Every signal must match once and only once!
		if (matches[k] != 1)
			return false;

	return true;
}

int main(void) {
	FILE *file = fopen(INPUT, "r");

	// First we generate all the possible ways that the signal wires might
	// be scrambled, which means all the permutations of the character
	// sequence of segments. There are 7! of those, so we generate them.
	// `permutate` is not pure, so we need to pass it its own copy of the
	// segments.
	char *tmp = strdup(segments);
	permutate(tmp, N_SEGMENTS);
	free(tmp);

	char in[N_SIGNALS][SIGNAL_LENGTH];
	char out[N_OUTPUTS][SIGNAL_LENGTH];

	// For each line of input patterns, we find the permutation which
	// uniquely decodes all the patterns into the unique signals.
	// Using this permutation, we then decode the output patterns.
	int sum_of_outputs = 0;
	while (fscanf(file, "%s %s %s %s %s %s %s %s %s %s | %s %s %s %s",
		      in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7],
		      in[8], in[9], out[0], out[1], out[2], out[3]) != EOF) {
		// Go through all the possible permutations to find one that can
		// match all 10 signals in the given input.
		for (int i = 0; i < N_PERMUTATIONS; i++) {
			char *mapping = permutations[i];
			if (matches_all_signals(mapping, in)) {
				// Now that we've found the mapping, we can
				// start decoding the output patterns.
				int output = 0;
				for (int j = 0; j < N_OUTPUTS; j++) {
					int signal = pattern_to_signal(out[j],
								       mapping);
					output += signal *
						  pow(10, N_OUTPUTS - j - 1);
				}
				sum_of_outputs += output;

				break;
			}
		}
	}

	printf("Sum of outputs: %d\n", sum_of_outputs);

	fclose(file);

	return 0;
}
