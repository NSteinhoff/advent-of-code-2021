#include <stddef.h>
#include <stdint.h>

uint32_t sum(const uint32_t *p, size_t n) {
	uint32_t sum = 0;
	for (size_t i = 0; i < n; i++)
		sum += p[i];
	return sum;
}
