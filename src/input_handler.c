#include <string.h>
#include <stdio.h>
#include "input_handler.h"
#include "parser.h"
#include "command_handler.h"

#define SIZE(x) (sizeof (x) / sizeof (x)[0])

typedef struct {
    const char* name;
    void (*command)(const char *, size_t, Stack *);
} Command;

Command commands[] = {
        {"ZERO", handleZero},
        {"IS_COEFF", handleIsCoeff},
        {"IS_ZERO", handleIsZero},
        {"CLONE", handleClone},
        {"ADD", handleAdd},
        {"MUL", handleMul},
        {"NEG", handleNeg},
        {"SUB", handleSub},
        {"IS_EQ", handleIsEq},
        {"DEG_BY", handleDegBy},
        {"DEG", handleDeg},
        {"AT", handleAt},
        {"PRINT", handlePrint},
        {"POP", handlePop}
};

static bool startsWith(const char *start, const char *str) {
    size_t startSize = strlen(start);
    size_t strLen = strlen(str);

    if (startSize > strLen)
        return false;

    for (size_t i = 0; i < startSize; ++i) {
        if (start[i] != str[i])
            return false;
    }

    return true;
}

static void wrongPoly(size_t lineNumber) {
    printError(lineNumber, "WRONG POLY");
}

bool isComment(const char *str) {
    return *str == '#';
}

bool isEmpty(const char *str, size_t lineLen) {
//    for (size_t i = 0; i < lineLen; ++i) {
//        if (str[i] != ' ')
//            return false;
//    }
    return *str == '\0';

//    return true;
}

void printError(size_t lineNumber, char *error) {
    fprintf(stderr, "ERROR %zu %s\n", lineNumber, error);
}

void wrongCommand(size_t lineNumber) {
    printError(lineNumber, "WRONG COMMAND");
}

void stackUnderflow(size_t lineNumber) {
    printError(lineNumber, "STACK UNDERFLOW");
}

bool pretendsToBeCommand(const char *str) {
    assert(str != NULL);

    return isInRange('A', 'Z', *str) ||
           isInRange('a', 'z', *str);
}

void handleCommand(char *str, size_t lineNumber, Stack *stack) {
    for (size_t i = 0; i < (size_t) SIZE(commands); ++i) {
        if (startsWith(commands[i].name, str)) {
            (*commands[i].command)(str, lineNumber, stack);
            return;
        }
    }

    wrongCommand(lineNumber);
}

void handlePoly(char *str, size_t lineNumber, Stack *stack) {
    Poly p;
    if (CanBePoly(str, &p)) {
        pushStack(stack, p);
    }
    else {
        wrongPoly(lineNumber);
    }
}
