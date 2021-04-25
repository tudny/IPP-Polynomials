/** @file
  Implementacja podstawowych operacji na wielomianach rzadkich wielu zmiennych

  @author Aleksander Tudruj <at429620@students.mimuw.edu.pl>
  @date 04/25/2021
 */

#include <stdlib.h>
#include "poly.h"

// ====================
// ====== MEMORY ======
// ====================


// Niezmienniki
// Potęgi posortowane malejąco
// Brak zbędnych x_i^0(x_{i+1}^0(p(x_{i+2})))
// Wielomiany zerowe zbite do wpółczynnika zera (jak wyżej) aby działała funkcja

// Sprawdzenie czy wskaźnik jest NULL'em. W przypadku pozytywnym
// program zamykany jest z kodem błędu 1.
//  ptr - sprawdzany wskaźnik
void requireNotNull(void *ptr) {
    if (ptr == NULL) {
        exit(EXIT_FAILURE);
    }
}

// Bezpieczna alternatywa malloc'a. W przypadku zwrócenia przez malloc'a
// NULL'a program zamykany jest z kodem błędu 1.
//  memoryBlockSize - malloc'owany rozmiar
void *safeMalloc(size_t memoryBlockSize) {
    void *pointer = malloc(memoryBlockSize);
    requireNotNull(pointer);

    return pointer;
}

// Bezpieczna alternatywa calloc'a. W przypadku zwrócenia przez calloc'a
// NULL'a program zamykany jest z kodem błędu 1.
//  numberOfElements - liczba elementów do alokacji
//  sizeOfElement    - rozmiar elementu
void *safeCalloc(size_t numberOfElements, size_t sizeOfElement) {
    void *pointer = calloc(numberOfElements, sizeOfElement);
    requireNotNull(pointer);

    return pointer;
}

// Bezpieczna alternatywa realloc'a. W przypadku Zwrócenia przez realloc'a
// NULL'a program zamykany jest z kodem błędu 1.
//  ptr             - wskaźnik na poprzednią pamięć
//  memoryBlockSize - nowy rozmiar
void *safeRealloc(void *ptr, size_t memoryBlockSize) {
    void *pointer = realloc(ptr, memoryBlockSize);
    requireNotNull(pointer);

    return pointer;
}

// Bezpieczna alternatywa free. Ustawia wskaxnik na NULL po zwolnieniu
// pamięci.
//  ptr - wskażnik na wskaźnik z pamięcią do zwolnienia
void safeFree(void **ptr) {
    free(*ptr);
    *ptr = NULL;
}

// =====================
// ======   END   ======
// =====================

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

bool canMonoBeCut(const Mono *m) {
    return MonoGetExp(m) == 0 && PolyIsCoeff(&m->p);
}

bool isPolySingleMonoAndZeroExpCoeff(const Poly *p) {
    if (PolyIsCoeff(p))
        return false;

    return p->size == 1 && canMonoBeCut(&p->arr[0]);
}

bool hasProperForm(const Poly *p) {
    if (PolyIsCoeff(p))
        return true;

    if (p->size < 1)
        return false;

    if (isPolySingleMonoAndZeroExpCoeff(p))
        return false;

    if (!isSorted(p))
        return false;

    for (size_t i = 0; i < p->size; ++i) {
        if (!hasProperForm(&p->arr[i].p))
            return false;
    }

    return true;
}

int compareExps(poly_exp_t a, poly_exp_t b) {
    return (b - a);
}

int compareMonosByExp(const void *a, const void *b) {
    Mono *x = (Mono *)a;
    Mono *y = (Mono *)b;

    return compareExps(MonoGetExp(x), MonoGetExp(y));
}

void sortMonosByExp(Mono *monos, size_t size) {
    qsort(monos, size, sizeof(Mono), compareMonosByExp);
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

void addSingleMonoUnsafe(Poly *p, Mono *m) {
    assert(!PolyIsCoeff(p) && "Bad type.");
    assert(isSorted(p) && "Poly isn't sorted.");
    assert(hasProperForm(p) && "Form isn't proper.");
    assert(p->arr[p->size - 1].exp > m->exp && "Unsafe condition not granted.");

    p->size++;
    p->arr = safeRealloc(p->arr, p->size * sizeof(Mono));
    p->arr[p->size - 1] = *m;
}

void removeLastMonoUnsafe(Poly *p) {
    assert(!PolyIsCoeff(p) && "Bad type.");
    assert(isSorted(p) && "Poly isn't sorted.");
    assert(hasProperForm(p) && "Form isn't proper.");
    assert(p->size > 0 && "Poly is empty.");

    p->size--;
    p->arr = safeRealloc(p->arr, p->size * sizeof(Mono));
}

Poly addCoeffPoly(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && PolyIsCoeff(q));

    return PolyFromCoeff(p->coeff + q->coeff);
}

Poly addNonCoeffAndCoeffPoly(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && PolyIsCoeff(q) && "Wrong Poly type.");

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

// Niech p, q będą miały posortowane tablice po współczynnikach malejąco.
Poly PolyAdd(const Poly *p, const Poly *q) {
    assert(isSorted(p) && isSorted(q) && "Polys not sorted.");
    assert(hasProperForm(p) && hasProperForm(q) && "Form isn't proper.");

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

    if (count == 0)
        return PolyZero();

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
            --uniqueExp;
            MonoDestroy(&allMonos[i]);
        }
    }

    for (size_t i = 1; i < count; ++i) {
        if (isProper[i] && isProper[i - 1]) {
            if (MonoGetExp(&allMonos[i]) == MonoGetExp(&allMonos[i - 1])) {
                Poly newPoly = PolyAdd(&allMonos[i].p, &allMonos[i - 1].p);

                MonoDestroy(&allMonos[i - 1]);
                MonoDestroy(&allMonos[i]);

                allMonos[i].p = newPoly;

                isProper[i - 1] = false;
                --uniqueExp;

                if (PolyIsZero(&newPoly)) {
                    isProper[i] = false;
                    --uniqueExp;
                    PolyDestroy(&newPoly);
                }
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

    Mono monos[p->size];
    size_t toTakeCounter = 0;

    for (size_t i = 0; i < p->size; i++) {

        Poly newPoly = multPolyByConst(&p->arr[i].p, c);
        poly_exp_t newExp = MonoGetExp(&p->arr[i]);

        if (!PolyIsZero(&newPoly)) {
            monos[toTakeCounter++] = MonoFromPoly(&newPoly, newExp);
        }
    }

    return PolyAddMonos(toTakeCounter, monos);
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
