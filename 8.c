/*--- Day 8: Seven Segment Search ---

You barely reach the safety of the cave when the whale smashes into the cave
mouth, collapsing it. Sensors indicate another exit to this cave at a much
greater depth, so you have no choice but to press on.

As your submarine slowly makes its way through the cave system, you notice that
the four-digit seven-segment displays in your submarine are malfunctioning; they
must have been damaged during the escape. You'll be in a lot of trouble without
them, so you'd better figure out what's wrong.

Each digit of a seven-segment display is rendered by turning on or off any of
seven segments named a through g:

  0:      1:      2:      3:      4:
 aaaa    ....    aaaa    aaaa    ....
b    c  .    c  .    c  .    c  b    c
b    c  .    c  .    c  .    c  b    c
 ....    ....    dddd    dddd    dddd
e    f  .    f  e    .  .    f  .    f
e    f  .    f  e    .  .    f  .    f
 gggg    ....    gggg    gggg    ....

  5:      6:      7:      8:      9:
 aaaa    aaaa    aaaa    aaaa    aaaa
b    .  b    .  .    c  b    c  b    c
b    .  b    .  .    c  b    c  b    c
 dddd    dddd    ....    dddd    dddd
.    f  e    f  .    f  e    f  .    f
.    f  e    f  .    f  e    f  .    f
 gggg    gggg    ....    gggg    gggg

So, to render a 1, only segments c and f would be turned on; the rest would be
off. To render a 7, only segments a, c, and f would be turned on.

The problem is that the signals which control the segments have been mixed up on
each display. The submarine is still trying to display numbers by producing
output on signal wires a through g, but those wires are connected to segments
randomly. Worse, the wire/segment connections are mixed up separately for each
four-digit display! (All of the digits within a display use the same
connections, though.)

So, you might know that only signal wires b and g are turned on, but that
doesn't mean segments b and g are turned on: the only digit that uses two
segments is 1, so it must mean segments c and f are meant to be on. With just
that information, you still can't tell which wire (b/g) goes to which segment
(c/f). For that, you'll need to collect more information.

For each display, you watch the changing signals for a while, make a note of all
ten unique signal patterns you see, and then write down a single four digit
output value (your puzzle input). Using the signal patterns, you should be able
to work out which pattern corresponds to which digit.

For example, here is what you might see in a single entry in your notes:

acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab |
cdfeb fcadb cdfeb cdbaf

(The entry is wrapped here to two lines so it fits; in your notes, it will all
be on a single line.)

Each entry consists of ten unique signal patterns, a | delimiter, and finally
the four digit output value. Within an entry, the same wire/segment connections
are used (but you don't know what the connections actually are). The unique
signal patterns correspond to the ten different ways the submarine tries to
render a digit using the current wire/segment connections. Because 7 is the only
digit that uses three segments, dab in the above example means that to render a
7, signal lines d, a, and b are on. Because 4 is the only digit that uses four
segments, eafb means that to render a 4, signal lines e, a, f, and b are on.

Using this information, you should be able to work out which combination of
signal wires corresponds to each of the ten digits. Then, you can decode the
four digit output value. Unfortunately, in the above example, all of the digits
in the output value (cdfeb fcadb cdfeb cdbaf) use five segments and are more
difficult to deduce.

For now, focus on the easy digits. Consider this larger example:

be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb |
fdgacbe cefdb cefbgd gcbe
edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec |
fcgedb cgb dgebacf gc
fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef |
cg cg fdcagb cbg
fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega |
efabcd cedba gadfec cb
aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga |
gecf egdcabf bgf bfgea
fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf |
gebdcfa ecba ca fadegcb
dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf |
cefg dcbef fcge gbcadfe
bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd |
ed bcgafe cdgba cbgef
egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg |
gbdfcae bgc cg cgb
gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc |
fgae cfgab fg bagce

Because the digits 1, 4, 7, and 8 each use a unique number of segments, you
should be able to tell which combinations of signals correspond to those digits.
Counting only digits in the output values (the part after | on each line), in
the above example, there are 26 instances of digits that use a unique number of
segments (highlighted above).

In the output values, how many times do digits 1, 4, 7, or 8 appear?

--- Part Two ---

Through a little deduction, you should now be able to determine the remaining
digits. Consider again the first example above:

acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab |
cdfeb fcadb cdfeb cdbaf

After some careful analysis, the mapping between signal wires and segments only
make sense in the following configuration:

 dddd
e    a
e    a
 ffff
g    b
g    b
 cccc

So, the unique signal patterns would correspond to the following digits:

    acedgfb: 8
    cdfbe: 5
    gcdfa: 2
    fbcad: 3
    dab: 7
    cefabd: 9
    cdfgeb: 6
    eafb: 4
    cagedb: 0
    ab: 1

Then, the four digits of the output value can be decoded:

    cdfeb: 5
    fcadb: 3
    cdfeb: 5
    cdbaf: 3

Therefore, the output value for this entry is 5353.

Following this same process for each entry in the second, larger example above,
the output value of each entry can be determined:

    fdgacbe cefdb cefbgd gcbe: 8394
    fcgedb cgb dgebacf gc: 9781
    cg cg fdcagb cbg: 1197
    efabcd cedba gadfec cb: 9361
    gecf egdcabf bgf bfgea: 4873
    gebdcfa ecba ca fadegcb: 8418
    cefg dcbef fcge gbcadfe: 4548
    ed bcgafe cdgba cbgef: 1625
    gbdfcae bgc cg cgb: 8717
    fgae cfgab fg bagce: 4315

Adding all of the output values in this larger example produces 61229.

For each entry, determine all of the wire/segment connections and decode the
four-digit output values. What do you get if you add up all of the output
values?
*/

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

const char segments[8] = "abcdefg";
const char *signals[N_SIGNALS] = {
    "abcefg", "cf",     "acdeg", "acdfg",   "bcdf",
    "abdfg",  "abdefg", "acf",   "abcdefg", "abcdfg",
};

static char permutations[N_PERMUTATIONS][SIGNAL_LENGTH];

static inline void swap(char *s, size_t a, size_t b) {
	char tmp = s[a];
	s[a] = s[b];
	s[b] = tmp;
}

static void permutate(char *pattern, size_t size, size_t n) {
	static size_t count = 0;
	if (size <= 1) {
		strcpy(permutations[count++], pattern);
		return;
	}

	for (size_t i = 0; i < size; i++) {
		permutate(pattern, size - 1, n);
		swap(pattern, size % 2 == 0 ? i : 0, size - 1);
	}
}

static int pattern_to_signal(const char *pattern, const char *mapping) {
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

static bool matches_all_signals(const char *mapping,
                                char patterns[][SIGNAL_LENGTH]) {
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

static void one(FILE *file) {
	printf("--- Part One ---\n");

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
}

static void two(FILE *file) {
	printf("--- Part Two ---\n");

	// First we generate all the possible ways that the signal wires might
	// be scrambled, which means all the permutations of the character
	// sequence "abcdfg".
	char tmp[8];
	permutate(strcpy(tmp, "abcdefg"), 7, 7);

	char in[N_SIGNALS][SIGNAL_LENGTH];
	char out[N_OUTPUTS][SIGNAL_LENGTH];

	// For each line of input patterns, we find the permutation which
	// uniquely decodes all the patterns into signals.
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
					int signal =
					    pattern_to_signal(out[j], mapping);
					output +=
					    signal * pow(10, N_OUTPUTS - j - 1);
				}
				sum_of_outputs += output;

				break;
			}
		}
	}

	printf("Sum of outputs: %d\n", sum_of_outputs);
}

int main() {
	FILE *file = fopen(INPUT, "r");

	one(file);
	rewind(file);
	two(file);

	fclose(file);

	return 0;
}
