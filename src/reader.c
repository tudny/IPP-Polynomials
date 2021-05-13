#include <stdio.h>
#include <stdbool.h>
#include "reader.h"
#include "memory.h"

// Tablica białych znaków.
static char *whiteChars = "\t\n\v\f\r";
static size_t whiteCharsNumber = 6;

// Sprawdzenie czy znak [c] jest znakiem białym (wg treścia polecenia).
//  c - sprawdany znak
static bool isWhiteCharacter(char c) {
    for (size_t i = 0; i < whiteCharsNumber; ++i)
        if (c == whiteChars[i])
            return true;

    return false;
}

// Dwukrotne rozszerzenie stringa [string] o długości [actualSize].
//  str        - wskaźnik na rozszerzanego stringa
//  actualSize - wskażnik na aktualny rozmiar
static void extendString(char **str, size_t *actualSize) {
    size_t newSize = *actualSize << 1;

    *str = safeRealloc(*str, sizeof(char) * newSize);
    *actualSize = newSize;
}

// Dodatnie znaku [c] do stringa pod wskaźnikiem [str]. Zmiana rozmiaru
// o ile to konieczne. Pominięcie jeżeli pierwszy znak to '#', gdyż wówczas
// linia jest komentarzem.
//  str   - string do którego dodajemy
//  id    - indeks do którego wstawiamy
//  size  - wskaźnik rozmiar stringa
//  c     - wstawiany znak
//  force - czy powinniśmy wstawić niezależnie od komentarza
static void addChar(char **str, size_t *id, size_t *size, char c, bool force) {
    if (*id + 1 == *size)
        extendString(str, size);

    if ((*str)[0] != '#' || force)
        (*str)[(*id)++] = c;
}

// Wczytanie linii z stdin do zmiennej [line].
// W przypadku pustej linii na końcu pliku lub końca pliku zwracane
// jest false. W przeciwnym przypadku zwracane jest true.
//  line - wskaźnik na linię do wczytania danych
//  len  - wskaźnik na zmienną z długością linii
bool readLine(char **line, size_t *len, size_t *lastLineLength) {
    char c;
    bool isNotEOF;
    size_t id = 0, size = *lastLineLength;

    (*line)[0] = 0;
    while ((isNotEOF = (scanf("%c", &c) != EOF)) && c != EOL) {
        if (isWhiteCharacter(c)) c = '$';
        addChar(line, &id, &size, c, false);
    }

    if (isNotEOF || id != 0) {
        addChar(line, &id, &size, '\0', true);
        *len = id - 1;
        *lastLineLength = size;
        return true;
    }
    else {
        return false;
    }
}
