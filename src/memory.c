#include <stdlib.h>
#include "memory.h"

void *requireNotNull(void *ptr) {
    if (ptr == NULL) {
        exit(EXIT_FAILURE);
    }

    return ptr;
}

void *safeMalloc(size_t memoryBlockSize) {
    return requireNotNull(malloc(memoryBlockSize));
}

void *safeCalloc(size_t numberOfElements, size_t sizeOfElement) {
    return requireNotNull(calloc(numberOfElements, sizeOfElement));
}

void safeFree(void **ptr) {
    free(*ptr);
    *ptr = NULL;
}

