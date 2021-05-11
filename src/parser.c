#include <errno.h>
#include <stdlib.h>
#include "parser.h"

bool isInRange(char begin, char end, char c) {
    return begin <= c && c <= end;
}

bool isDigit(char c) {
    return isInRange('0', '9', c);
}

bool isNonZeroDigit(char c) {
    return isDigit(c) && c != '0';
}

bool is(const char *c, char pattern) {
    return *c == pattern;
}

bool hasPropperBrackets(string str) {
    size_t bracketValue = 0;
    for  (; *str != '\0' && bracketValue >= 0; ++str) {
        if (*str == '(')
            bracketValue++;
        else if (*str == ')')
            bracketValue--;
    }

    return (bracketValue >= 0);
}

bool canBeNumberOverflowSafe(string str, void *number, char **endPtr, NumberType numberType) {
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

    // program nie uznaje wiodących zer
    if (is(strPtr, '0')) {
        *endPtr = strPtr + 1;
        return true;
    }
    else if (isNonZeroDigit(*strPtr)) {
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

bool canBeCoeffOverflowSafe(string str, poly_coeff_t *number, char **endPtr) {
    long long tempNumber;
    bool toReturn = canBeNumberOverflowSafe(str, &tempNumber, endPtr, LONG);

    *number = (poly_coeff_t) tempNumber;
    if (!(MIN_COEFF <= tempNumber && tempNumber <= MAX_COEFF)) {
        *endPtr = str;
        toReturn = false;
    }

    return toReturn;
}

bool canBeExpOverflowSafe(string str, poly_exp_t *number, char **endPtr) {
    unsigned long long tempNumber;
    bool toReturn = canBeNumberOverflowSafe(str, &tempNumber, endPtr, ULONG);

    *number = (poly_exp_t) tempNumber;
    if (!(MIN_EXP <= tempNumber && tempNumber <= MAX_EXP)) {
        *endPtr = str;
        toReturn = false;
    }

    return toReturn;
}

bool canBeDegOverflowSafe(string str, size_t *deg, char **endPtr) {
    unsigned long long tempNumber;
    bool toReturn = canBeNumberOverflowSafe(str, &tempNumber, endPtr, ULONG);

    *deg = (size_t) tempNumber;
    if (!(MIN_DEG <= tempNumber && tempNumber <= MAX_DEG)) {
        *endPtr = str;
        toReturn = false;
    }

    return toReturn;
}

bool canBeMono(string str, Mono *m, char **endPtr);

void addSingle(Mono m, Mono **tab, int size) {
    *tab = realloc(*tab, sizeof(Mono) * (size + 1));
    (*tab)[size] = m;
}

bool canBePoly(string str, Poly *p, char **endPtr) {
    // sprawdzenie czy wielomian jest stały
    poly_coeff_t number;
    char *end;

    if (canBeCoeffOverflowSafe(str, &number, &end)) {
        *endPtr = end;
        *p = PolyFromCoeff(number);
        return true;
    }

    // nie stały
    Mono *monos = malloc(sizeof(Mono) * 0);
    char *strPtr = str;
    poly_exp_t monosCnt = 0;
    Mono tempM;
    bool lastMonoCreated = true;

    if (canBeMono(strPtr, &tempM, &end)) {
        strPtr = end;
        addSingle(tempM, &monos, monosCnt++);

        while (*strPtr != '\0' && is(strPtr, '+') &&
               (lastMonoCreated = canBeMono(strPtr + 1, &tempM, &end))) {
            addSingle(tempM, &monos, monosCnt++);
            strPtr = end;
        }

        if (lastMonoCreated) {
            *p = PolyAddMonos(monosCnt, monos);
            // printf("Liczba monosow: %d\n", monosCnt);
            *endPtr = end;
            return true;
        }
    }

    *endPtr = str;
    return false;
}

bool canBeMono(string str, Mono *m, char **endPtr) {
    char *strPtr = str;
    poly_exp_t number;
    Poly tempP;

    if (is(strPtr, '(')) {
        if (canBePoly(str + 1, &tempP, &strPtr)) {
            if (is(strPtr, ',')) {
                if (canBeExpOverflowSafe(strPtr + 1, &number, &strPtr)) {
                    if (is(strPtr, ')')) {
                        *m = MonoFromPoly(&tempP, number);
                        *endPtr = strPtr + 1;
                        return true;
                    }
                }
            }
        }
    }

    *endPtr = str;
    return false;
}

bool CanBePoly(string str, Poly *p) {

    if (!hasPropperBrackets(str))
        return false;

    char *end;
    return canBePoly(str, p, &end) && *end == '\0';
}
