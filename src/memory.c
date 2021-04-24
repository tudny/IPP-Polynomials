#include <stdlib.h>
#include "memory.h"

// Sprawdzenie czy wskaźnik jest NULL'em. W przypadku pozytywnym
// program zamykany jest z kodem błędu 1.
//  ptr - sprawdzany wskaźnik
void requireNotNull(void *ptr) {
    if (ptr == NULL) {
        exit(EXIT_FAILURE);
    }
}

// Bezpieczna alternatywa malloc'a. W przypadku zwrócenia przez malloc'a
// NULL'a program zamykany jest z kodem błędu 1.
//  memoryBlockSize - malloc'owany rozmiar
void *safeMalloc(size_t memoryBlockSize) {
    void *pointer = malloc(memoryBlockSize);
    requireNotNull(pointer);

    return pointer;
}

// Bezpieczna alternatywa calloc'a. W przypadku zwrócenia przez calloc'a
// NULL'a program zamykany jest z kodem błędu 1.
//  numberOfElements - liczba elementów do alokacji
//  sizeOfElement    - rozmiar elementu
void *safeCalloc(size_t numberOfElements, size_t sizeOfElement) {
    void *pointer = calloc(numberOfElements, sizeOfElement);
    requireNotNull(pointer);

    return pointer;
}

// Bezpieczna alternatywa realloc'a. W przypadku Zwrócenia przez realloc'a
// NULL'a program zamykany jest z kodem błędu 1.
//  ptr             - wskaźnik na poprzednią pamięć
//  memoryBlockSize - nowy rozmiar
void *safeRealloc(void *ptr, size_t memoryBlockSize) {
    void *pointer = realloc(ptr, memoryBlockSize);
    requireNotNull(pointer);

    return pointer;
}