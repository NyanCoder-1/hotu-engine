#include "utility/dynamic-array.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct DynamicArrayHeader {
	size_t elementSize;
	size_t capacity;
	size_t size;
	bool shrinkable : 1;
	max_align_t _align;
} DynamicArrayHeader, *DynamicArrayHeader_t;
typedef const DynamicArrayHeader *DynamicArrayHeaderConst_t;

static void *grow(void *array, const size_t deltaElementsSize);
static void *shrink(void *array);
static DynamicArrayHeader_t getHeader(const void *array);
static void *getArray(const DynamicArrayHeader *header);
static size_t ensureMinCapacity(const size_t cap);
static size_t growthByOneAndHalf(const size_t from);
static int mulOverflow_size_t(const size_t a, const size_t b, size_t *out);
#define DYNAMIC_ARRAY_SHRINK_DIVISOR 4
#define DYNAMIC_ARRAY_HEADER_SIZE sizeof(DynamicArrayHeader)

void *dynamicArrayCreate(size_t elementSize, size_t initialCapacity) {
	if (elementSize == 0) return NULL;
	initialCapacity = ensureMinCapacity(initialCapacity);
	// overflow check
	size_t dataBytes = 0;
	if (mulOverflow_size_t(elementSize, initialCapacity, &dataBytes) || SIZE_MAX - DYNAMIC_ARRAY_HEADER_SIZE < dataBytes) { return NULL; }
	const size_t size = DYNAMIC_ARRAY_HEADER_SIZE + dataBytes;
	DynamicArrayHeader_t header = (DynamicArrayHeader_t)malloc(size);
	if (!header) return NULL;
	memset(header, 0, size);
	header->elementSize = elementSize;
	header->capacity = initialCapacity;
	// default header->size is 0;
	// default header->shrinkable is false
	return getArray(header);
}
void dynamicArrayDelete(void **array) {
	if (!array || !*array) return;

	DynamicArrayHeader_t header = (DynamicArrayHeader_t)((uint8_t*)*array - DYNAMIC_ARRAY_HEADER_SIZE);
	const size_t amountToSetZero = DYNAMIC_ARRAY_HEADER_SIZE;
	memset(header, 0, amountToSetZero);
	free(header);
	*array = NULL;
}
MightRealloc_t dynamicArrayPush(void *self, const void *element) {
	return dynamicArrayPushMany(self, element, 1);
}
MightRealloc_t dynamicArrayPushMany(void *self, const void *elements, size_t count) {
	if (!self) return NULL;
	if (!count) return self;
	if (!elements) return NULL;
	self = grow(self, count);
	if (!self) return NULL; /* can't grow */
	
	DynamicArrayHeader_t header = getHeader(self);
	memcpy((char*)self + header->size * header->elementSize, elements, count * header->elementSize);
	header->size += count;

	return self;
}
MightRealloc_t dynamicArrayPop(void *self) {
	return dynamicArrayPopMany(self, 1);
}
MightRealloc_t dynamicArrayPopMany(void *self, size_t count) {
	if (!self || !count) return self; /* no-op */

	DynamicArrayHeader_t header = getHeader(self);
	if (header->size < count) {
		header->size = 0;
	}
	else {
		header->size -= count;
	}

	if (header->shrinkable) {
		return shrink(self);
	}

	return self;
}
size_t dynamicArrayGetSize(const void *self) {
	if (!self) return 0;
	DynamicArrayHeaderConst_t header = getHeader(self);
	return header->size;
}
size_t dynamicArrayGetDataSize(const void *self) {
	if (!self) return 0;
	DynamicArrayHeaderConst_t header = getHeader(self);
	return header->size * header->elementSize;
}
size_t dynamicArrayGetCapacity(const void *self) {
	if (!self) return 0;
	DynamicArrayHeaderConst_t header = getHeader(self);
	return header->capacity;
}
size_t dynamicArrayGetElementSize(const void *self) {
	if (!self) return 0;
	DynamicArrayHeaderConst_t header = getHeader(self);
	return header->elementSize;
}
MightRealloc_t dynamicArrayResize(void *self, size_t newSize) {
	if (!self) return NULL;
	DynamicArrayHeader_t header = getHeader(self);
	if (newSize == header->size) return self;

	const bool isGrowing = newSize > header->size;
	if (isGrowing) {
		const size_t deltaSize = newSize - header->size;

		self = grow(self, deltaSize);
		if (!self) return NULL;

		header = getHeader(self);
		memset((char*)self + header->size * header->elementSize, 0, deltaSize * header->elementSize);
		header->size += deltaSize;
		return self;
	}

	const size_t deltaSize = header->size - newSize;
	return dynamicArrayPopMany(self, deltaSize);
}
MightRealloc_t dynamicArrayReserve(void *self, size_t newCapacity) {
	if (!self) return NULL;
	DynamicArrayHeader_t header = getHeader(self);
	if (newCapacity <= header->capacity) return self;
	/* Overflow check */
	size_t dataBytes = 0;
	if (mulOverflow_size_t(newCapacity, header->elementSize, &dataBytes) || SIZE_MAX - DYNAMIC_ARRAY_HEADER_SIZE < dataBytes) return NULL;
	const size_t totalSize = DYNAMIC_ARRAY_HEADER_SIZE + dataBytes;
	header = (DynamicArrayHeader_t)realloc(header, totalSize);
	if (!header) return NULL;
	header->capacity = ensureMinCapacity(newCapacity);
	return getArray(header);
}
void dynamicArrayClear(void *self) {
	if (!self) return;
	DynamicArrayHeader_t header = getHeader(self);
	header->size = 0;
}
MightRealloc_t dynamicArrayShrinkToFit(void *self) {
	if (!self) return NULL;
	DynamicArrayHeader_t header = getHeader(self);
	if (header->capacity == header->size) return self;
	/* Overflow check */
	const size_t newCapacity = ensureMinCapacity(header->size);
	size_t dataBytes = 0;
	if (mulOverflow_size_t(newCapacity, header->elementSize, &dataBytes)) { return NULL; }
	const size_t totalSize = DYNAMIC_ARRAY_HEADER_SIZE + dataBytes;
	header = (DynamicArrayHeader_t)realloc(header, totalSize);
	if (!header) return NULL;
	header->capacity = newCapacity;
	return getArray(header);
}
void dynamicArraySetShrinkable(void *self) {
	if (!self) return;
	DynamicArrayHeader_t header = getHeader(self);
	header->shrinkable = true;
}
void dynamicArrayUnsetShrinkable(void *self) {
	if (!self) return;
	DynamicArrayHeader_t header = getHeader(self);
	header->shrinkable = false;
}

static size_t nextPowerOfTwo(const size_t x);
static size_t max(const size_t a, const size_t b);

static void *grow(void *array, const size_t deltaElementsSize) {
	if (!array) return NULL;
	DynamicArrayHeader_t header = getHeader(array);
	const size_t requiredArraySize = header->size + deltaElementsSize;

	if (requiredArraySize <= header->capacity) {
		// Array doesn't need to grow
		return array;
	}
	const size_t normalGrow = growthByOneAndHalf(header->capacity);
	const size_t contentAwareGrow = nextPowerOfTwo(requiredArraySize);
	const size_t chosenGrow = ensureMinCapacity(max(normalGrow, contentAwareGrow));
	/* overflow check */
	size_t dataBytes = 0;
	if (mulOverflow_size_t(chosenGrow, header->elementSize, &dataBytes) || SIZE_MAX - DYNAMIC_ARRAY_HEADER_SIZE < dataBytes) { return NULL; }
	const size_t totalSize = DYNAMIC_ARRAY_HEADER_SIZE + dataBytes;
	header = (DynamicArrayHeader_t)realloc(header, totalSize);
	if (!header) return NULL;
	header->capacity = chosenGrow;
	return getArray(header);
}
static void *shrink(void *array) {
	if (!array) return NULL;
	DynamicArrayHeader_t header = getHeader(array);
	if (header->size > header->capacity / DYNAMIC_ARRAY_SHRINK_DIVISOR) return array;

	const size_t targetCapacity = ensureMinCapacity(growthByOneAndHalf(header->size));
	 /* overflow check */
	size_t dataBytes = 0;
	if (mulOverflow_size_t(targetCapacity, header->elementSize, &dataBytes) || SIZE_MAX - DYNAMIC_ARRAY_HEADER_SIZE < dataBytes) { return getArray(header); /* better keep old */ }
	const size_t totalSize = DYNAMIC_ARRAY_HEADER_SIZE + dataBytes;
	header = (DynamicArrayHeader_t)realloc(header, totalSize);
	if (!header) return NULL;
	header->capacity = targetCapacity;
	return getArray(header);
}
static DynamicArrayHeader_t getHeader(const void *array) {
	DynamicArrayHeader_t header = (DynamicArrayHeader_t)((uint8_t*)array - DYNAMIC_ARRAY_HEADER_SIZE);
	return header;
}
static void *getArray(const DynamicArrayHeader *header) {
	void *array = (DynamicArrayHeader_t)((uint8_t*)header + DYNAMIC_ARRAY_HEADER_SIZE);
	return array;
}
static size_t ensureMinCapacity(const size_t cap) {
	return cap ? cap : 1;
}
static size_t growthByOneAndHalf(const size_t from) {
	return from + (from >> 1);
}
static int mulOverflow_size_t(const size_t a, const size_t b, size_t *out) {
	if (b && a > SIZE_MAX / b) return 1;
	*out = a * b;
	return 0;
}

static size_t max(const size_t a, const size_t b) {
	return a > b ? a : b;
}
static bool isPowerOfTwo(const size_t x) {
	return x && !(x & (x - 1));
}
static size_t nextPowerOfTwo(const size_t x) {
	if (x <= 1) return 1;
	if (x > SIZE_MAX >> 1) return SIZE_MAX;
	size_t num = x - 1;
	// logarithmic shift, so loop is shorter
	size_t shift = 1;
	while (!isPowerOfTwo(num + 1)) { // check if x + 1 is a power of two
		num |= num >> shift;
		shift <<= 1;
	}
	return num + 1;
}
