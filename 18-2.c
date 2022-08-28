#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lines.h"
#include "read.h"

#define INPUT "18.txt"

typedef struct Number Number;

typedef enum {
	VAL,
	PAIR,
} Type;

struct Number {
	Type t;
	union {
		int v;
		struct {
			Number *l, *r;
		};
	};
};

static Number *pair(Number *left, Number *right) {
	Number *number = malloc(sizeof(Number));
	number->t = PAIR;
	number->l = left;
	number->r = right;

	return number;
}

static Number *val(int val) {
	Number *number = malloc(sizeof(Number));
	number->t = VAL;
	number->v = val;

	return number;
}

static void free_all(Number *number) {
	if (!number) return;
	switch (number->t) {
	case VAL: break;
	case PAIR:
		free_all(number->l);
		free_all(number->r);
		break;
	}
	free(number);
}

static char consume(const char **s) { return *(*s)++; }

static Number *parse_(const char **s) {
	assert(s);
	assert(*s);
	assert(**s);

	Number *number = malloc(sizeof(Number));

	char c;
	while ((c = consume(s))) {
		switch (c) {
		case ' ': break;
		case '[': {
			number->t = PAIR;
			number->l = parse_(s);
			break;
		}
		case ',': {
			assert(number->t == PAIR);
			number->r = parse_(s);
			break;
		}
		case ']': {
			assert(number->t == PAIR);
			return number;
		}
		default: {
			return val(c - '0');
		}
		}
	}

	return NULL;
}

static Number *parse(const char *s) { return parse_(&s); }

static int depth(const Number *number) {
	switch (number->t) {
	case VAL: return 0;
	case PAIR: {
		int left = depth(number->l);
		int right = depth(number->r);

		return (left > right ? left : right) + 1;
	}
	}
}

static bool over(const Number *number) {
	switch (number->t) {
	case VAL: return number->v > 9;
	case PAIR: return over(number->l) || over(number->r);
	}
}

static bool isleaf(const Number *number) {
	return number->l->t == VAL && number->r->t == VAL;
}

static void addright(int v, Number *number) {
	if (!number) return;

	if (number->t == VAL) {
		number->v += v;
		return;
	}

	addright(v, number->r);
}

static void addleft(int v, Number *number) {
	if (!number) return;

	switch (number->t) {
	case VAL: number->v += v; break;
	case PAIR: addleft(v, number->l); break;
	}
}

static void explode_(Number *number, Number *left, Number *right) {
	assert(number->t != VAL);

	if (isleaf(number)) {
		addright(number->l->v, left);
		free_all(number->l);

		addleft(number->r->v, right);
		free_all(number->r);

		number->t = VAL;
		number->v = 0;

		return;
	}

	if (depth(number->l) >= depth(number->r))
		explode_(number->l, left, number->r);
	else
		explode_(number->r, number->l, right);
}

static Number *explode(Number *number) {
	explode_(number, NULL, NULL);

	return number;
}

static Number *split(Number *number) {
	assert(number);

	switch (number->t) {
	case VAL: {
		int v = number->v;
		assert(v > 9);
		number->t = PAIR;
		number->l = val(v / 2);
		number->r = val(v / 2 + v % 2);
		break;
	}
	case PAIR:
		if (over(number->l))
			split(number->l);
		else if (over(number->r))
			split(number->r);
		break;
	}

	return number;
}

static Number *reduce(Number *number) {
	if (depth(number) > 4) return reduce(explode(number));
	if (over(number)) return reduce(split(number));

	return number;
}

static Number *add(Number *left, Number *right) {
	return reduce(pair(left, right));
}

static int magnitude(Number *number) {
	assert(number);
	switch (number->t) {
	case VAL: return number->v;
	case PAIR: return magnitude(number->l) * 3 + magnitude(number->r) * 2;
	}
}

int main() {
	char *content = read_to_string(INPUT);
	Lines *lines = lines_from_string(content);

	if (!lines) return EXIT_FAILURE;

	int max = 0;
	for (size_t i = 0; i < lines->len; i++) {
		for (size_t j = 0; j < lines->len; j++) {
			if (i == j) continue;
			Number *left = parse(lines->lines[i]);
			Number *right = parse(lines->lines[j]);
			assert(left);
			assert(right);

			Number *sum = add(left, right);
			int mag = magnitude(sum);
			if (mag > max) max = mag;

			free_all(left);
			free_all(right);
		}
	}

	printf("Largest Sum: %d\n", max);

	lines_free(lines);
}
