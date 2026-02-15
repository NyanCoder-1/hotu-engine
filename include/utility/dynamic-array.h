#ifndef __UTILITY_DYNAMIC_ARRAY_H__
#define __UTILITY_DYNAMIC_ARRAY_H__

#include <stddef.h>

#define DYNAMIC_ARRAY_WRAP_REALLOC(arr, func, onErr, ...)   \
    do {                                                    \
        void *_temp = func(arr __VA_OPT__(,) __VA_ARGS__);  \
        if (_temp) { arr = _temp; } else { onErr; }         \
    } while (0)

/* NOTE: All functions returning MightRealloc_t may reallocate */
typedef void *MightRealloc_t;

void *dynamicArrayCreate(size_t elementSize, size_t initialCapacity);
void dynamicArrayDelete(void **array);
MightRealloc_t dynamicArrayPush(void *self, const void *element);
MightRealloc_t dynamicArrayPushMany(void *self, const void *elements, size_t count);
MightRealloc_t dynamicArrayPop(void *self);
MightRealloc_t dynamicArrayPopMany(void *self, size_t count);
size_t dynamicArrayGetSize(const void *self);
size_t dynamicArrayGetDataSize(const void *self);
size_t dynamicArrayGetCapacity(const void *self);
size_t dynamicArrayGetElementSize(const void *self);
MightRealloc_t dynamicArrayResize(void *self, size_t newSize);
MightRealloc_t dynamicArrayReserve(void *self, size_t newCapacity);
void dynamicArrayClear(void *self);
MightRealloc_t dynamicArrayShrinkToFit(void *self);
void dynamicArraySetShrinkable(void *self);
void dynamicArrayUnsetShrinkable(void *self);

#endif
