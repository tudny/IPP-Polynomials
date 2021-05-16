/** @file
 * @brief Kalkulator operujący na wielomianach.
 * Plik główny kalkulatora działającego na wielomianach rzadkich wielu zmiennch
 * stosującego odwrotną notację polską.
 *
 * @author Aleksander Tudruj <at429630@students.mimuw.edu.pl>
 * @date 15.05.2021
 * */

#include <stdlib.h>
#include "memory.h"
#include "reader.h"
#include "input_handler.h"
#include "stack.h"

/**
 * Główna funkcja uruchomieniowa.
 * Zawiera główną część działania programu. Tworzy strukturę stosu, przetwarza
 * wiersze oraz czyście pamięć przed zamknęciem pliku.
 * @return kod wykonania programu; 0, gdy program zakończony się poprawnie;
 * 1, gdy program nie zakończy się poprawnie.
 * */
int main(void) {

    Stack *stack = createEmptyStack();

    char *line = safeMalloc(sizeof(char));
    size_t lineSize = 1;
    size_t lineStrLen;
    size_t lineNumber = 0;

    while (readLine(&line, &lineStrLen, &lineSize)) {
        ++lineNumber;

        if (isComment(line) || isEmpty(line))
            continue;

        if (pretendsToBeCommand(line))
            handleCommand(line, lineNumber, stack);
        else
            handlePoly(line, lineNumber, stack);
    }

    safeFree((void **) &line);
    destoryStack(stack);

    return 0;
}