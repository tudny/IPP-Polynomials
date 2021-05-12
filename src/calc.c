#include <stdlib.h>
#include "memory.h"
#include "reader.h"
#include "input_handler.h"
#include "stack.h"

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

        printStack(stack);
    }

    return 0;
}