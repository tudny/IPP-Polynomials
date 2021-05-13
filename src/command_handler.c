#include <string.h>
#include <stdio.h>
#include "command_handler.h"
#include "input_handler.h"
#include "parser.h"

static bool hasPropername(const char *str,
                          const char *name,
                          size_t lineNumber) {
    if (strcmp(name, str) != 0) {
        wrongCommand(lineNumber);
        return false;
    }

    return true;
}

static bool stackHasXPolys(size_t lineNumber, Stack *stack, size_t x) {
    if (sizeStack(stack) < x) {
        stackUnderflow(lineNumber);
        return false;
    }

    return true;
}

static bool hasProperNameAndHasXPolys(const char *str,
                                       const char *name,
                                       size_t lineNumber,
                                       Stack *stack,
                                       size_t x) {
    if (!hasPropername(str, name, lineNumber))
        return false;

    return stackHasXPolys(lineNumber, stack, x);
}

void handleZero(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "ZERO", lineNumber, stack, 0))
        return;

    pushStack(stack, PolyZero());
}

void handleIsCoeff(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "IS_COEFF", lineNumber, stack, 1))
        return;

    Poly top = topStack(stack);
    printf("%d\n", PolyIsCoeff(&top) ? 1 : 0);
}

void handleIsZero(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "IS_ZERO", lineNumber, stack, 1))
        return;

    Poly top = topStack(stack);
    printf("%d\n", PolyIsZero(&top) ? 1 : 0);
}

void handleClone(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "CLONE", lineNumber, stack, 1))
        return;

    Poly top = topStack(stack);
    Poly cloned = PolyClone(&top);
    pushStack(stack, cloned);
}

void handleAdd(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "ADD", lineNumber, stack, 2))
        return;

    Poly a = takeStack(stack);
    Poly b = takeStack(stack);
    Poly sum = PolyAdd(&a, &b);

    PolyDestroy(&a);
    PolyDestroy(&b);

    pushStack(stack, sum);
}

void handleMul(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "MUL", lineNumber, stack, 2))
        return;

    Poly a = takeStack(stack);
    Poly b = takeStack(stack);
    Poly sum = PolyMul(&a, &b);

    PolyDestroy(&a);
    PolyDestroy(&b);

    pushStack(stack, sum);
}

void handleNeg(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "NEG", lineNumber, stack, 1))
        return;

    Poly top = takeStack(stack);
    Poly negated = PolyNeg(&top);
    PolyDestroy(&top);

    pushStack(stack, negated);
}

void handleSub(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "SUB", lineNumber, stack, 2))
        return;

    Poly a = takeStack(stack);
    Poly b = takeStack(stack);
    Poly sum = PolySub(&a, &b);

    PolyDestroy(&a);
    PolyDestroy(&b);

    pushStack(stack, sum);
}

void handleIsEq(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "IS_EQ", lineNumber, stack, 2))
        return;

    Poly a = topStack(stack);
    Poly b = secondTopStack(stack);

    printf("%d\n", PolyIsEq(&a, &b) ? 1 : 0);
}

void handleDeg(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "DEG", lineNumber, stack, 1))
        return;

    Poly a = topStack(stack);

    printf("%d\n", PolyDeg(&a));
}

void handleDegBy(const char *str, size_t lineNumber, Stack *stack) {
    char *name = "DEG_BY";
    char *spaceAndArgument = (char *) str + strlen(name);
    size_t argument;
    char *endPtr;

    if (*spaceAndArgument != ' ' ||
            !canBeDegOverflowSafe(spaceAndArgument + 1, &argument, &endPtr) ||
            *endPtr != '\0') {
        printError(lineNumber, "DEG BY WRONG VARIABLE");
        return;
    }

    if (!stackHasXPolys(lineNumber, stack, argument))
        return;

    Poly a = topStack(stack);
    poly_exp_t degBy = PolyDegBy(&a, argument);
    printf("%d\n", degBy);
}

void handleAt(const char *str, size_t lineNumber, Stack *stack) {
    char *name = "AT";
    char *spaceAndArgument = ((char *) str) + strlen(name);
    poly_coeff_t argument;
    char *endPtr;

    if (*spaceAndArgument != ' ' || *(spaceAndArgument + 1) == '\0' ||
            !canBeCoeffOverflowSafe(spaceAndArgument + 1, &argument, &endPtr) ||
            *endPtr != '\0'){
        printError(lineNumber, "AT WRONG VALUE");
        return;
    }

    if (!stackHasXPolys(lineNumber, stack, 1))
        return;

    Poly a = takeStack(stack);
    Poly at = PolyAt(&a, argument);

    PolyDestroy(&a);

    pushStack(stack, at);
}

void handlePrint(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "PRINT", lineNumber, stack, 1))
        return;

    Poly top = topStack(stack);
    PrintPolyNormalized(&top);
    printf("\n");
}

void handlePop(const char *str, size_t lineNumber, Stack *stack) {
    if (!hasProperNameAndHasXPolys(str, "POP", lineNumber, stack, 1))
        return;

    popStack(stack);
}


