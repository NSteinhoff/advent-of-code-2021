#include <stddef.h>
#include <stdint.h>

// Linker defined global variable whose address is the start of the heap
extern uint32_t __heap_base;

// Create a byte sized pointer to the start of the heap section of the linear
// memory.  We will increment this pointer when allocating memory.
uint8_t *heap_ptr = (uint8_t *)&__heap_base;

void *walloc(const size_t size) {
	uint8_t *p = heap_ptr;
	heap_ptr += size;
	return (void *)p;
}

uint32_t next() {
	return (size_t)heap_ptr;
}

uint32_t fill(size_t size, int32_t value) {
	int32_t *a = walloc(sizeof(int32_t) * size);

	for (size_t i = 0; i < size; i++)
		a[i] = value;

	return (size_t)a;
}
