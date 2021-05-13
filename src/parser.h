#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include <stdbool.h>
#include <limits.h>
#include "poly.h"

static const long long MIN_COEFF = LONG_MIN;
static const long long MAX_COEFF = LONG_MAX;

static const unsigned long long MIN_EXP = 0;
static const unsigned long long MAX_EXP = 2147483647;

static const unsigned long long MIN_DEG = 0;
static const unsigned long long MAX_DEG = 18446744073709551615ULL;

typedef char * string;

typedef enum {
    LONG,
    ULONG
} NumberType;

bool CanBePoly(string str, Poly *p);

bool isInRange(char begin, char end, char c);

bool canBeDegOverflowSafe(string str, size_t *deg, char **endPtr);

bool canBeCoeffOverflowSafe(string str, poly_coeff_t *number, char **endPtr);

#endif //PARSER_PARSER_H
