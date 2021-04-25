/** @file
  Implementacja biblioteki podstawowych operacji na wielomianach rzadkich wielu zmiennych.

  @author Aleksander Tudruj <at429620@students.mimuw.edu.pl>
  @date 2021
 */

#include <stdio.h>
#include "poly.h"
#include "memory.h"

// Niezmienniki
// Potęgi posortowane malejąco
// Brak zbędnych x_i^0(x_{i+1}^0(p(x_{i+2})))
// Wielomiany zerowe zbite do wpółczynnika zera (jak wyżej) aby działała funkcja

// TODO
// PolyPrint    [+] n
// PolyDestroy  [+] t
// PolyClone    [+] n
// PolyAdd      [?] n
// PolyAddMonos [?] t
// PolyMul      [?] n
// PolyNeg      [?] n
// PolySub      [+] n
// PolyDegBy        n
// PolyDeg          n
// PolyIsEq         n
// PolyAt           n

bool isSorted(const Poly *p) {
    if (PolyIsCoeff(p))
        return true;

    for (size_t i = 1; i < p->size; ++i) {
        if (MonoGetExp(&p->arr[i - 1]) < MonoGetExp(&p->arr[i])) {
            return false;
        }
    }

    return true;
}

void printPoly(const Poly *p, int id);

void printMono(const Mono *m, int id) {
    printf("x_{%d}^{%d}(", id, MonoGetExp(m));
    printPoly(&m->p, id + 1);
    printf(")");
}

void printPoly(const Poly *p, int id) {
    if (PolyIsCoeff(p)) {
        printf("%ld", p->coeff);
    } else {
        for (size_t i = 0; i < p->size; ++i) {
            if (i != 0) printf(" + ");
            printMono(&p->arr[i], id);
        }
    }
}

int compareExps(poly_exp_t a, poly_exp_t b) {
    return (b - a);
}

int compareMonos(const void *a, const void *b) {
    Mono *x = (Mono *)a;
    Mono *y = (Mono *)b;

    return compareExps(MonoGetExp(x), MonoGetExp(y));
}

void sortMonosByExp(Mono *monos, size_t size) {
    qsort(monos, size, sizeof(Mono), compareMonos);
}

void PolyPrint(const Poly *p) {
    printPoly(p, 0);
    printf("\n");
}

void PrintPolyWithLabel(const Poly *p, char *label) {
    printf("%s: ", label);
    PolyPrint(p);
}

poly_coeff_t fastPower(poly_coeff_t a, poly_exp_t n) {
    if (n == 0)
        return 1;

    poly_coeff_t score = fastPower(a * a, n / 2);

    if (n % 2 == 1)
        score *= a;

    return score;
}

poly_exp_t max(poly_exp_t a, poly_exp_t b) {
    return (a > b) ? a : b;
}

void PolyDestroy(Poly *p) {
    if (PolyIsCoeff(p))
        return;

    for (size_t i = 0; i < p->size; ++i) {
        MonoDestroy(&p->arr[i]);
    }

    safeFree((void **) &p->arr);
}

Poly PolyClone(const Poly *p) {
    Poly poly;

    if (PolyIsCoeff(p)) {
        poly = PolyFromCoeff(p->coeff);
    }
    else {
        poly.size = p->size;
        poly.arr = safeCalloc(p->size, sizeof(Mono));

        for (size_t i = 0; i < p->size; ++i)
            poly.arr[i] = MonoClone(&p->arr[i]);
    }

    return poly;
}

void addSingleMonoUnsafe(Poly *p, Mono *m) {
    assert(!PolyIsCoeff(p) && "Bad type.");
    assert(isSorted(p) && "Poly isn't sorted.");
    assert(p->arr[p->size - 1].exp > m->exp && "Unsafe condition not granted");

    p->size++;
    p->arr = safeRealloc(p->arr, p->size * sizeof(Mono));
    p->arr[p->size - 1] = *m;
}

void removeLastMonoUnsafe(Poly *p) {
    assert(!PolyIsCoeff(p) && "Bad type.");
    assert(isSorted(p) && "Poly isn't sorted.");

    p->size--;
    p->arr = safeRealloc(p->arr, p->size * sizeof(Mono));
}

Poly addCoeffPoly(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && PolyIsCoeff(q));

    return PolyFromCoeff(p->coeff + q->coeff);
}

bool canMonoBeCut(const Mono *m) {
    return MonoGetExp(m) == 0 && PolyIsCoeff(&m->p);
}

bool isPolySingleMonoAndZeroExpCoeff(const Poly *p) {
    if (PolyIsCoeff(p))
        return false;

    return p->size == 1 && canMonoBeCut(&p->arr[0]);
}

Poly addNonCoeffAndCoeffPoly(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && PolyIsCoeff(q) && "Wrong Poly type.");
    assert(isSorted(p) && "Poly isn't sorted.");

    Poly s = PolyClone(p);
    size_t lastMono = p->size - 1;
    poly_exp_t lastExp = MonoGetExp(&p->arr[lastMono]);

    if (lastExp == 0) {
        Poly newLastPoly = PolyAdd(&p->arr[lastMono].p, q);

        MonoDestroy(&s.arr[lastMono]);
        s.arr[lastMono] = MonoFromPoly(&newLastPoly, 0);
    }
    else {
        Poly newPoly = PolyFromCoeff(q->coeff);

        Mono newMono = MonoFromPoly(&newPoly, 0);
        addSingleMonoUnsafe(&s, &newMono);
        ++lastMono;
    }

    if (PolyIsZero(&s.arr[lastMono].p)) {
        removeLastMonoUnsafe(&s);
    }

    if (isPolySingleMonoAndZeroExpCoeff(&s)) {
        Poly newPoly = s.arr[0].p;
        PolyDestroy(&s);
        return newPoly;
    }

    if (s.size == 0) {
        PolyDestroy(&s);
        return PolyZero();
    }

    return s;
}

Poly addTwoNonCoeffPolys(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && !PolyIsCoeff(q));
    assert(isSorted(p) && isSorted(q) && "Polys not sorted!");

    size_t allSize = p->size + q->size;
    Mono allMonos[allSize];

    for (size_t i = 0; i < p->size; i++) {
        allMonos[i] = MonoClone(&p->arr[i]);
    }

    for (size_t i = 0; i < q->size; i++) {
        allMonos[i + p->size] = MonoClone(&q->arr[i]);
    }

    Poly s = PolyAddMonos(allSize, allMonos);

    return s;
}

// Niech p, q będą miały posortowane tablice po współczynnikach malejąco.
Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return addCoeffPoly(p, q);
    }
    else if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        return addNonCoeffAndCoeffPoly(q, p);
    }
    else if (PolyIsCoeff(q) && !PolyIsCoeff(p)) {
        return addNonCoeffAndCoeffPoly(p, q);
    }
    else {
        return addTwoNonCoeffPolys(p, q);
    }
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {

    Mono allMonos[count];
    for (size_t i = 0; i < count; i++) {
        allMonos[i] = monos[i];
    }

    sortMonosByExp(allMonos, count);

    size_t uniqueExp = count;
    bool isProper[count];
    for (size_t i = 0; i < count; ++i) {
        isProper[i] = true;

        if (PolyIsZero(&allMonos[i].p)) {
            isProper[i] = false;
        }
    }

    for (size_t i = 1; i < count; ++i) {
        if (MonoGetExp(&allMonos[i]) == MonoGetExp(&allMonos[i - 1]) && isProper[i - 1]) { // TODO line length
            Poly newPoly = PolyAdd(&allMonos[i].p, &allMonos[i - 1].p);

            MonoDestroy(&allMonos[i - 1]);
            MonoDestroy(&allMonos[i]);

            allMonos[i].p = newPoly;

            --uniqueExp;
            isProper[i - 1] = false;

            if (PolyIsZero(&newPoly)) {
                --uniqueExp;
                isProper[i] = false;
            }
        }
    }

    if (uniqueExp == 0) {
        return PolyZero();
    }
    else if (uniqueExp == 1) {
        for (size_t i = 0; i < count; ++i) {
            if (isProper[i] && canMonoBeCut(&allMonos[i])) {
                return allMonos[i].p;
            }
        }
    }

    Poly s = {.size = uniqueExp, .arr = safeMalloc(sizeof(Mono) * uniqueExp)};

    size_t ptr = 0;
    for (size_t i = 0; i < count; ++i) {
        if (isProper[i]) {
            s.arr[ptr++] = allMonos[i];
        }
    }

    return s;
}

Poly multPolyByConst(const Poly *p, poly_coeff_t c) {

    // Special case - much faster then normal computation and makes less trash.
    if (c == 0)
        return PolyZero();

    if (PolyIsCoeff(p))
        return PolyFromCoeff(c * p->coeff);

    Poly cloned = {.arr = safeMalloc(sizeof(Mono) * p->size), .size = p->size};
    for (size_t i = 0; i < p->size; i++) {
        cloned.arr[i].exp = MonoGetExp(&p->arr[i]);
        cloned.arr[i].p = multPolyByConst(&p->arr[i].p, c);
    }

    return cloned;
}

Poly PolyNeg(const Poly *p) {
    return multPolyByConst(p, -1);
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly negatedQ = PolyNeg(q);
    Poly added = PolyAdd(p, &negatedQ);
    PolyDestroy(&negatedQ);
    return added;
}

Poly mulCoeffPoly(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && PolyIsCoeff(q));
    return PolyFromCoeff(p->coeff * q->coeff);
}

Poly mulNonCoeffAndCoeffPoly(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && PolyIsCoeff(q));
    return multPolyByConst(p, q->coeff);
}

Poly mulTwoNonCoeffPoly(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && !PolyIsCoeff(q));
    assert(isSorted(p) && isSorted(q) && "Polys not sorted!");

    size_t ws = 0, allSize = p->size * q->size;
    Mono allMonos[allSize];

    for (size_t i = 0; i < p->size; i++) {
        for (size_t j = 0; j < q->size; j++) {
            allMonos[ws].p = PolyMul(&p->arr[i].p, &q->arr[j].p);
            allMonos[ws].exp = p->arr[i].exp + q->arr[j].exp;
            ws++;
        }
    }

    return PolyAddMonos(allSize, allMonos);
}

Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return mulCoeffPoly(p, q);
    }
    else if (PolyIsCoeff(p) && !PolyIsCoeff(q)) {
        return mulNonCoeffAndCoeffPoly(q, p);
    }
    else if (PolyIsCoeff(q) && !PolyIsCoeff(p)) {
        return mulNonCoeffAndCoeffPoly(p, q);
    }
    else {
        return mulTwoNonCoeffPoly(p, q);
    }
}

bool PolyIsEq(const Poly *p, const Poly *q) {

    assert(isSorted(p) && isSorted(q));

    if (PolyIsCoeff(p) ^ PolyIsCoeff(q))
        return false;

    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return p->coeff == q->coeff;
    }
    else {
        if (p->size != q->size)
            return false;

        for (size_t i = 0; i < p->size; i++) {
            if (MonoGetExp(&p->arr[i]) != MonoGetExp(&q->arr[i]))
                return false;

            if (!PolyIsEq(&p->arr[i].p, &q->arr[i].p))
                return false;
        }
    }

    return true;
}

Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (PolyIsCoeff(p))
        return PolyClone(p);

    Poly res = PolyZero();
    for (size_t i = 0; i < p->size; ++i) {
        poly_exp_t exp = MonoGetExp(&p->arr[i]);
        poly_coeff_t substitution = fastPower(x, exp);

        Poly multiplied = multPolyByConst(&p->arr[i].p, substitution);
        Poly temp = PolyAdd(&res, &multiplied);

        PolyDestroy(&multiplied);
        PolyDestroy(&res);
        res = temp;
    }

    return res;
}

void degBy(const Poly *p, size_t actIdx, size_t varIdx, poly_exp_t *acc) {
    if (PolyIsCoeff(p))
        return;

    if (actIdx == varIdx) {
        for (size_t i = 0; i < p->size; ++i) {
            *acc = max(*acc, MonoGetExp(&p->arr[i]));
        }
    }
    else {
        for (size_t i = 0; i < p->size; ++i) {
            degBy(&p->arr[i].p, actIdx + 1, varIdx, acc);
        }
    }
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (PolyIsZero(p))
        return -1;

    int cnt = 0;
    degBy(p, 0, var_idx, &cnt);

    return cnt;
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsZero(p))
        return -1;

    if (PolyIsCoeff(p))
        return 0;

    poly_exp_t cnt = 0;
    for (size_t i = 0; i < p->size; ++i) {
        poly_exp_t deg = PolyDeg(&p->arr[i].p) + MonoGetExp(&p->arr[i]);
        cnt = max(cnt, deg);
    }

    return cnt;
}
