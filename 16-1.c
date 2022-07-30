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
	LIT,
	OP,
	TYPE_END,
} Type;

static char *type_names[TYPE_END] = {
	[LIT] = "Literal",
	[OP] = "Operator",
};

typedef struct Packet {
	size_t version;
	Type type;
	size_t bits;
	union {
		uint64_t value;
		struct {
			size_t nsub;
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
		return (Packet){.type = LIT,
				.version = version,
				.bits = packet_bits,
				.value = value};
	}

	size_t lenid = consume(s, 1, &packet_bits);
	if (lenid) {
		size_t nsub = (size_t)consume(s, 11, &packet_bits);
		Packet *sub = malloc(sizeof(Packet) * nsub);

		size_t sub_bits_read = 0;
		for (size_t i = 0; i < nsub; i++) {
			sub[i] = read_packet(s, &sub_bits_read);
		}
		packet_bits += sub_bits_read;

		*bits_read += packet_bits;
		return (Packet){.type = OP,
				.version = version,
				.bits = sub_bits_read,
				.nsub = nsub,
				.sub = sub};
	} else {
		size_t nbits = consume(s, 15, &packet_bits);
		size_t capacity = 1;
		Packet *sub = malloc(sizeof(Packet) * capacity);
		size_t nsub = 0;

		size_t sub_bits_read = 0;
		while (sub_bits_read < nbits) {
			if (nsub + 1 > capacity) {
				capacity *= 2;
				sub = realloc(sub, sizeof(Packet) * capacity);
			}
			sub[nsub++] = read_packet(s, &sub_bits_read);
		}
		packet_bits += sub_bits_read;

		*bits_read += packet_bits;
		return (Packet){.type = OP,
				.version = version,
				.bits = packet_bits,
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
	if (packet.type == LIT)
		printf(": %llu\n", packet.value);
	else {
		printf(": [%zu subpackets]\n", packet.nsub);
		if (packet.sub)
			for (size_t i = 0; i < packet.nsub; i++)
				print_packet(packet.sub[i], depth + 1);
	}
}

static void free_packet(Packet packet) {
	if (packet.type == LIT) return;

	for (size_t i = 0; i < packet.nsub; i++)
		free_packet(packet.sub[i]);
	free(packet.sub);
}

static size_t accumulate_versions(Packet packet) {
	if (packet.type == LIT) {
		return packet.version;
	}
	assert(packet.sub);

	size_t version = packet.version;
	for (size_t i = 0; i < packet.nsub; i++)
		version += accumulate_versions(packet.sub[i]);

	return version;
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

	printf("\nSum of all Version fields: %zu\n",
	       accumulate_versions(packet));
	free_packet(packet);
}
