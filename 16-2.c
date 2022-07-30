#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT "16.txt"
#define MAX_HEX 2000
#define MAX_BITS MAX_HEX * 4

static char hex[MAX_HEX];
static char binary[MAX_BITS];

typedef enum {
	LITERAL,
	OPERATOR,
	TYPE_END,
} Type;

static char *type_names[TYPE_END] = {
	[LITERAL] = "Literal",
	[OPERATOR] = "Operator",
};

typedef enum {
	SUM,
	PROD,
	MIN,
	MAX,
	CONST,
	GREATER,
	LESS,
	EQUAL,
	OPERATOR_END,
} Operator;

// clang-format off
static char *op_names[OPERATOR_END] = {
	[SUM] = "SUM",
	[PROD] = "PROD",
	[MIN] = "MIN",
	[MAX] = "MAX",
	[CONST] = "CONST",
	[GREATER] = "GREATER",
	[LESS] = "LESS",
	[EQUAL] = "EQUAL",
};
// clang-format on

typedef struct Packet {
	size_t version;
	Type type;
	size_t bits;
	union {
		uint64_t value;
		struct {
			size_t nsub;
			Operator op;
			struct Packet *sub;
		};
	};
} Packet;

// clang-format off
static const char *hex2binary[] = {
	['0'] = "0000",
	['1'] = "0001",
	['2'] = "0010",
	['3'] = "0011",
	['4'] = "0100",
	['5'] = "0101",
	['6'] = "0110",
	['7'] = "0111",
	['8'] = "1000",
	['9'] = "1001",
	['A'] = "1010",
	['B'] = "1011",
	['C'] = "1100",
	['D'] = "1101",
	['E'] = "1110",
	['F'] = "1111",
};
// clang-format on

/// Consume `n` bits from the string and return the value.
static uint64_t consume(char **s, int n, size_t *bits_read) {
	assert(s);
	assert(*s);
	assert(bits_read);
	uint64_t value = 0;

	for (int i = 0; i < n; i++) {
		if (*(*s)++ == '1') value++;
		if (i < n - 1) value <<= 1;
	}

	*bits_read += (size_t)n;
	return value;
}

static uint64_t read_value(char **s, size_t *bits_read) {
	assert(s);
	assert(*s);
	assert(bits_read);
	uint64_t value = 0;

	size_t cont = 1;
	while (cont) {
		cont = consume(s, 1, bits_read);
		if (value) value <<= 4;
		value += consume(s, 4, bits_read);
	}

	return value;
}

static Packet read_packet(char **s, size_t *bits_read) {
	assert(s);
	assert(*s);
	size_t packet_bits = 0;
	if (!bits_read) bits_read = &packet_bits;

	size_t version = consume(s, 3, &packet_bits);
	size_t type = consume(s, 3, &packet_bits);
	if (type == 4) {
		uint64_t value = read_value(s, &packet_bits);

		*bits_read += packet_bits;
		return (Packet){.type = LITERAL,
				.version = version,
				.bits = packet_bits,
				.value = value};
	}

	size_t lenid = consume(s, 1, &packet_bits);
	if (lenid) {
		size_t nsub = (size_t)consume(s, 11, &packet_bits);
		Packet *sub = malloc(sizeof(Packet) * nsub);
		assert(sub);

		size_t sub_bits_read = 0;
		for (size_t i = 0; i < nsub; i++) {
			sub[i] = read_packet(s, &sub_bits_read);
		}
		packet_bits += sub_bits_read;

		*bits_read += packet_bits;
		return (Packet){.type = OPERATOR,
				.version = version,
				.bits = sub_bits_read,
				.op = (Operator)type,
				.nsub = nsub,
				.sub = sub};
	} else {
		size_t nbits = consume(s, 15, &packet_bits);
		size_t capacity = 1;
		Packet *sub = malloc(sizeof(Packet) * capacity);
		assert(sub);
		size_t nsub = 0;

		size_t sub_bits_read = 0;
		while (sub_bits_read < nbits) {
			if (nsub + 1 > capacity) {
				capacity *= 2;
				sub = realloc(sub, sizeof(Packet) * capacity);
				assert(sub);
			}
			sub[nsub++] = read_packet(s, &sub_bits_read);
		}
		packet_bits += sub_bits_read;

		*bits_read += packet_bits;
		return (Packet){.type = OPERATOR,
				.version = version,
				.bits = packet_bits,
				.op = (Operator)type,
				.nsub = nsub,
				.sub = sub};
	}
}

static void indent(int depth) {
	if (depth) printf("%*s", depth * 4, " ");
}

static void print_packet(Packet packet, int depth) {
	indent(depth);
	printf("%s v%zu (%zu bits)", type_names[packet.type], packet.version,
	       packet.bits);
	if (packet.type == LITERAL)
		printf(": %llu\n", packet.value);
	else {
		printf(": [%s of %zu subpackets]\n", op_names[packet.op],
		       packet.nsub);
		if (packet.sub)
			for (size_t i = 0; i < packet.nsub; i++)
				print_packet(packet.sub[i], depth + 1);
	}
}

static void free_packet(Packet packet) {
	if (packet.type == LITERAL) return;

	for (size_t i = 0; i < packet.nsub; i++)
		free_packet(packet.sub[i]);
	free(packet.sub);
}

static uint64_t evaluate_packet(Packet packet) {
	if (packet.type == LITERAL) {
		return packet.value;
	}
	assert(packet.sub);

	// Recursively evaluate all subpackets of this operator packet.
	uint64_t *values = malloc(sizeof(uint64_t) * packet.nsub);
	assert(values);
	for (size_t i = 0; i < packet.nsub; i++)
		values[i] = evaluate_packet(packet.sub[i]);

	uint64_t value = 0;
	switch (packet.op) {
	case SUM:
		value = 0;
		for (size_t i = 0; i < packet.nsub; i++)
			value += values[i];
		break;
	case PROD:
		value = 1;
		for (size_t i = 0; i < packet.nsub; i++)
			value *= values[i];
		break;
	case MIN:
		assert(packet.nsub > 0);
		value = values[0];
		for (size_t i = 1; i < packet.nsub; i++)
			if (values[i] < value) value = values[i];
		break;
	case MAX:
		assert(packet.nsub > 0);
		value = values[0];
		for (size_t i = 1; i < packet.nsub; i++)
			if (values[i] > value) value = values[i];
		break;
	case GREATER:
		assert(packet.nsub == 2);
		value = values[0] > values[1];
		break;
	case LESS:
		assert(packet.nsub == 2);
		value = values[0] < values[1];
		break;
	case EQUAL:
		assert(packet.nsub == 2);
		value = values[0] == values[1];
		break;
	case CONST:
	case OPERATOR_END: assert(0 && "Unreachable!");
	}

	free(values);
	return value;
}

int main() {
	FILE *file = fopen(INPUT, "r");
	size_t n = 0;
	for (int c; (c = fgetc(file)) != EOF && c != '\n'; n++) {
		hex[n] = (char)c;
		memcpy(binary + n * 4, hex2binary[c], 4);
	}
	fclose(file);

	printf("HEX: %s\n", hex);
	printf("BIN: %s\n", binary);

	char *s = binary; // Grab a pointer to the start of the array. This
			  // pointer will be advanced as the parse the packet.

	Packet packet = read_packet(&s, 0);
	print_packet(packet, 0);

	printf("\nPacket value: %llu\n", evaluate_packet(packet));
	free_packet(packet);
}
