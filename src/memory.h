#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdlib.h>
#include <stdio.h>

// Sprawdzenie czy wskaźnik jest NULL'em. W przypadku pozytywnym
// program zamykany jest z kodem błędu 1.
//  ptr - sprawdzany wskaźnik
void requireNotNull(void *ptr);

// Bezpieczna alternatywa malloc'a. W przypadku zwrócenia przez malloc'a
// NULL'a program zamykany jest z kodem błędu 1.
//  memoryBlockSize - malloc'owany rozmiar
void *safeMalloc(size_t memoryBlockSize);

// Bezpieczna alternatywa calloc'a. W przypadku zwrócenia przez calloc'a
// NULL'a program zamykany jest z kodem błędu 1.
//  numberOfElements - liczba elementów do alokacji
//  sizeOfElement    - rozmiar elementu
void *safeCalloc(size_t numberOfElements, size_t sizeOfElement);

// Bezpieczna alternatywa realloc'a. W przypadku Zwrócenia przez realloc'a
// NULL'a program zamykany jest z kodem błędu 1.
//  ptr             - wskaźnik na poprzednią pamięć
//  memoryBlockSize - nowy rozmiar
void *safeRealloc(void *ptr, size_t memoryBlockSize);

// Bezpieczna alternatywa free. Ustawia wskaxnik na NULL po zwolnieniu
// pamięci.
//  ptr - wskażnik na wskaźnik z pamięcią do zwolnienia
void safeFree(void **ptr);

#endif /* MEMORY_H */

