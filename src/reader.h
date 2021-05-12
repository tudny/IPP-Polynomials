#ifndef READER_H
#define READER_H

#include <stdbool.h>

// Znak końca linii
#define EOL '\n'

// Wczytanie linii z stdin do zmiennej [line].
// W przypadku pustej linii na końcu pliku lub końca pliku zwracane
// jest false. W przeciwnym przypadku zwracane jest true.
//  line - wskaźnik na linię do wczytania danych
//  len  - wskaźnik na zmienną z długością linii
bool readLine(char **line, size_t *len, size_t *lastLineLength);

#endif // READER_H