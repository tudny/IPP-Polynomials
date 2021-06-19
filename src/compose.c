#include <stdio.h>
#include "compose.h"
#include "memory.h"

static const poly_exp_t MAX_POWER_OF_TWO = 30;

static poly_exp_t lastLeqPowerOfTwo(poly_exp_t a) {
    for (poly_exp_t k = MAX_POWER_OF_TWO; k > 0; k--) {
        if ((1 << k) & a)
            return k;
    }

    return 0;
}

static Poly* polyPowersOfTwo(Poly *p, poly_exp_t n) {
    Poly *powers = safeCalloc(n + 1, sizeof(Poly));

    powers[0] = PolyClone(p);

    for (poly_exp_t i = 1; i <= n; ++i) {
        powers[i] = PolyMul(&powers[i - 1], &powers[i - 1]);
    }

    return powers;
}

static void clearPowers(size_t count, Poly *powers) {
    for (size_t i = 0; i <= count; ++i) {
        PolyDestroy(&powers[i]);
    }

    safeFree((void **) &powers);
}

static Poly fastPowerPoly(Poly *polyBiPowers, poly_coeff_t exponent, poly_coeff_t maxPowerOfTwo) {
    Poly result = PolyFromCoeff(1);

    for (poly_coeff_t k = 0; k <= maxPowerOfTwo; ++k) {
        if (exponent & (1 << k)) {
//            printf("polyBiPowers[k]: "); PrintPolyNormalized(&polyBiPowers[k]); printf("\n");
//            printf("result: "); PrintPolyNormalized(&result); printf("\n");
            Poly newRes = PolyMul(&result, &polyBiPowers[k]);
            PolyDestroy(&result);
            result = newRes;
        }
    }

    return result;
}

static poly_exp_t max(poly_exp_t a, poly_exp_t b) {
    return a > b ? a : b;
}

static Poly polyCompose(Poly *base, size_t depth, Poly *substitutes) {
    if (PolyIsCoeff(base))
        return PolyClone(base);

    Poly substitute = (depth <= 0) ? PolyZero() : substitutes[0];

    Poly res = PolyZero();

    poly_exp_t maxPower = 0;
    for (size_t k = 0; k < base->size; k++) {
        maxPower = max(base->arr[k].exp, maxPower);
    }

    poly_exp_t maxPowerOfTwo = lastLeqPowerOfTwo(maxPower);
    Poly *substitutePowers = polyPowersOfTwo(&substitute, maxPowerOfTwo);

    for (size_t k = 0; k < base->size; k++) {
        Poly xPower = fastPowerPoly(substitutePowers, base->arr[k].exp, maxPowerOfTwo);
        Poly sub = polyCompose(&base->arr[k].p, depth - 1, substitutes + 1);
        Poly new = PolyMul(&sub, &xPower);
        PolyDestroy(&xPower);
        PolyDestroy(&sub);

        res = PolyAddProperty(&res, &new);
    }

    clearPowers(maxPowerOfTwo, substitutePowers);

    return res;
}

Poly PolyComposeProperty(Poly *base, size_t depth, Poly *substitutes) {
    return polyCompose(base, depth, substitutes);
}
