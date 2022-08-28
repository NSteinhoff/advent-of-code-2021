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
	if (number->t != VAL) {
		free_all(number->l);
		free_all(number->r);
	}
	free(number);
}

static void print_(const Number *number) {
	if (number->t == VAL) {
		printf("%d", number->v);
		return;
	}

	printf("[");
	print_(number->l);
	printf(",");
	print_(number->r);
	printf("]");
}

static void print(const Number *number) {
	if (!number) return;
	print_(number);
	printf("\n");
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

static bool equals(const Number *left, const Number *right) {
	if (left == right) return true;
	if (left->t != right->t) return false;
	if (left->t == VAL && right->t == VAL) return left->v == right->v;

	return equals(left->l, right->l) && equals(left->r, right->r);
}

static int depth(const Number *number) {
	if (number->t == VAL) return 0;
	int left = depth(number->l);
	int right = depth(number->r);

	return (left > right ? left : right) + 1;
}

static bool over(const Number *number) {
	if (number->t == VAL) return number->v > 9;
	return over(number->l) || over(number->r);
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

	if (number->t == VAL) {
		number->v += v;
		return;
	}

	addleft(v, number->l);
}

static void explode_(Number *number, Number *left, Number *right) {
	assert(number->t != VAL);

	if (isleaf(number)) {
		addright(number->l->v, left);
		free_all(number->l);

		addleft(number->r->v, right);
		free_all(number->r);

		number->l = NULL;
		number->r = NULL;
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

	if (number->t == VAL) {
		int v = number->v;
		assert(v > 9);
		number->t = PAIR;
		number->l = val(v / 2);
		number->r = val(v / 2 + v % 2);
	} else if (over(number->l)) {
		split(number->l);
	} else if (over(number->r)) {
		split(number->r);
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
	if (number->t == VAL) return number->v;

	return magnitude(number->l) * 3 + magnitude(number->r) * 2;
}

static void test_equals() {
	assert(equals(&(Number){.t = VAL, .v = 10},
		      &(Number){.t = VAL, .v = 10}));
	assert(!equals(&(Number){.t = VAL, .v = 1},
		       &(Number){.t = VAL, .v = 2}));

	Number *left = pair(val(10), val(20));
	Number *right = pair(val(10), val(20));
	assert(equals(left, right));
	free_all(left);
	free_all(right);

	left = pair(val(11), val(20));
	right = pair(val(10), val(20));
	assert(!equals(left, right));
	free_all(left);
	free_all(right);

	left = pair(pair(val(11), val(20)), val(10));
	right = pair(val(10), val(20));
	assert(!equals(left, right));
	free_all(left);
	free_all(right);

	left = pair(pair(val(11), val(20)), val(10));
	right = pair(pair(val(11), val(20)), val(10));
	assert(equals(left, right));
	free_all(left);
	free_all(right);
}

static void test_parse() {
	Number *n = NULL;

	n = parse("1");
	assert(equals(n, val(1)));

	n = parse("[1,2]");
	assert(equals(n, pair(val(1), val(2))));

	n = parse("[1,[[2,3],4]]");
	assert(equals(n, pair(val(1), pair(pair(val(2), val(3)), val(4)))));

	free_all(n);
}

static void test_depth() {
	Number *n = NULL;

	n = parse("10");
	assert(depth(n) == 0);
	free_all(n);

	n = parse("[1, 2]");
	assert(depth(n) == 1);
	free_all(n);

	n = parse("[[1, 2], 3]");
	assert(depth(n) == 2);
	free_all(n);

	n = parse("[[1, [2, 3]], 4]");
	assert(depth(n) == 3);
	free_all(n);
}

static void test_reduce() {
	Number *a, *b;

	// should not reduce
	a = parse("[[[1,[2,3]],4],5]");
	b = parse("[[[1,[2,3]],4],5]");
	assert(equals(reduce(a), b));
	free_all(a);
	free_all(b);

	a = parse("[[[[[9,8],1],2],3],4]");
	b = parse("[[[[0,9],2],3],4]");
	assert(equals(reduce(a), b));
	free_all(a);
	free_all(b);

	a = parse("[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]");
	b = parse("[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");
	assert(equals(reduce(a), b));
	free_all(a);
	free_all(b);
}

static void test() {
	test_equals();
	test_parse();
	test_depth();
	test_reduce();
}

int main() {
	test();
	char *content = read_to_string(INPUT);
	Lines *lines = lines_from_string(content);

	if (!lines) return EXIT_FAILURE;

	Number *sum = NULL;
	for (size_t i = 0; i < lines->len; i++) {
		Number *parsed = parse(lines->lines[i]);
		assert(parsed);

		if (i == 0)
			sum = parsed;
		else
			sum = add(sum, parsed);
	}

	printf("%-12s ", "SUM:");
	print(sum);
	printf("%-12s %d\n", "MAGNITUDE:", magnitude(sum));
	free_all(sum);

	lines_free(lines);
}
