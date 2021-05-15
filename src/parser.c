#include <errno.h>
#include <stdlib.h>
#include "parser.h"
#include "memory.h"

static bool isDigit(char c) {
    return isInRange('0', '9', c);
}

static bool is(const char * const c, char pattern) {
    return *c == pattern;
}

static bool hasPropperBrackets(char *str) {
    long bracketValue = 0;
    for  (; *str != '\0' && bracketValue >= 0; ++str) {
        if (*str == '(')
            bracketValue++;
        else if (*str == ')')
            bracketValue--;
    }

    return (bracketValue >= 0);
}

static bool canBeNumber(char *str, void *number, char **endPtr, NumberType numberType) {
    if (*str == '\0') {
        *endPtr = str;
        return false;
    }

    char *strPtr = str;
    *((long long *) number) = 0; // long long i unsigned long long zachowają się tak samo
    bool minus = false;
    errno = 0;

    if (is(strPtr, '-') && numberType == LONG) {
        minus = true;
        strPtr++;
    }

    if (isDigit(*strPtr)) {
        switch (numberType) {
            case LONG:
                *((long long *) number) =
                        strtoll(strPtr, &strPtr, 10);
                break;
            case ULONG:
                *((unsigned long long *) number) =
                        strtoull(strPtr, &strPtr, 10);
                break;
            default:
                exit(4321);
        }

        if (minus) {
            *((long long *) number) *= -1;
        }

        if (strPtr != str && errno == 0) {
            *endPtr = strPtr;
            return true;
        }
    }

    *endPtr = str;
    return false;
}

static bool canBeExp(char *str, poly_exp_t *number, char **endPtr) {
    unsigned long long tempNumber;
    bool toReturn = canBeNumber(str, &tempNumber, endPtr, ULONG);
    if (!toReturn)
        return toReturn;

    *number = (poly_exp_t) tempNumber;
    if (!(MIN_EXP <= tempNumber && tempNumber <= MAX_EXP)) {
        *endPtr = str;
        toReturn = false;
    }

    return toReturn;
}

static bool canBeMono(char *str, Mono *m, char **endPtr);

static void addSinleExtend(Mono m, Mono **tab, size_t *elems, size_t *memSize) {
    if (*elems == *memSize) {
        *memSize <<= 1;
        *tab = safeRealloc(*tab, *memSize * sizeof(Mono));
    }

    (*tab)[(*elems)++] = m;
}

bool canBeDeg(char *str, size_t *deg, char **endPtr) {
    unsigned long long tempNumber;
    bool toReturn = canBeNumber(str, &tempNumber, endPtr, ULONG);
    if (!toReturn)
        return toReturn;

    *deg = (size_t) tempNumber;
    if (!(MIN_DEG <= tempNumber && tempNumber <= MAX_DEG)) {
        *endPtr = str;
        toReturn = false;
    }

    return toReturn;
}

bool canBeCoeff(char *str, poly_coeff_t *number, char **endPtr) {
    long long tempNumber;
    bool toReturn = canBeNumber(str, &tempNumber, endPtr, LONG);
    if (!toReturn)
        return toReturn;

    *number = (poly_coeff_t) tempNumber;
    if (!(MIN_COEFF <= tempNumber && tempNumber <= MAX_COEFF)) {
        *endPtr = str;
        toReturn = false;
    }

    return toReturn;
}

static bool canBePoly(char *str, Poly *p, char **endPtr) {
    // sprawdzenie czy wielomian jest stały
    poly_coeff_t number;
    char *end;

    if (canBeCoeff(str, &number, &end)) {
        *endPtr = end;
        *p = PolyFromCoeff(number);
        return true;
    }

    // nie stały
    Mono *monos = safeMalloc(sizeof(Mono));
    char *strPtr = str;
    size_t monosCnt = 0;
    size_t memorySize = 1;
    Mono tempM;
    bool lastMonoCreated = true;

    if (canBeMono(strPtr, &tempM, &end)) {
        strPtr = end;
        addSinleExtend(tempM, &monos, &monosCnt, &memorySize);

        while (*strPtr != '\0' && is(strPtr, '+') &&
               (lastMonoCreated = canBeMono(strPtr + 1, &tempM, &end))) {
            addSinleExtend(tempM, &monos, &monosCnt, &memorySize);
            strPtr = end;
        }

        if (lastMonoCreated) {
            *p = PolyAddMonos(monosCnt, monos);
            safeFree((void **) &monos);
            *endPtr = end;
            return true;
        }
    }

    for (size_t i = 0; i < monosCnt; ++i)
        MonoDestroy(&monos[i]);

    safeFree((void **) &monos);
    *endPtr = str;
    return false;
}

static bool canBeMono(char *str, Mono *m, char **endPtr) {
    char *strPtr = str;
    poly_exp_t number;
    Poly tempP = PolyZero();

    if (is(strPtr, '(')) {
        if (canBePoly(str + 1, &tempP, &strPtr)) {
            if (is(strPtr, ',')) {
                if (canBeExp(strPtr + 1, &number, &strPtr)) {
                    if (is(strPtr, ')')) {
                        if (PolyIsZero(&tempP)) number = 0;
                        *m = MonoFromPoly(&tempP, number);
                        *endPtr = strPtr + 1;
                        return true;
                    }
                }
            }
        }
    }

    *endPtr = str;
    PolyDestroy(&tempP);
    return false;
}

bool CanBePoly(char *str, Poly *p) {

    if (!hasPropperBrackets(str))
        return false;

    char *end;
    bool canBe = canBePoly(str, p, &end);

    if (canBe && *end != '\0') {
        PolyDestroy(p);
        return false;
    }

    return canBe && *end == '\0';
}

bool isInRange(const char begin, const char end, const char c) {
    return begin <= c && c <= end;
}
