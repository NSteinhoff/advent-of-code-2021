#include <stddef.h>
#include <stdint.h>

uint32_t len(const char *s) {
	size_t l = 0;
	while (*(s++)) {
		l++;
	}
	return l;
}

uint32_t process(const char *input) {
	return len(input);
}
