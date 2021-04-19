/** @file
  Implementacja biblioteki podstawowych operacji na wielomianach rzadkich wielu zmiennych.

  @author Aleksander Tudruj <at429620@students.mimuw.edu.pl>
  @date 2021
 */

#include <stdlib.h>
#include <stdio.h>
#include "poly.h"

#define NOT_A_MONO -20

// Niezmienniki
// Potęgi posortowane malejąco
// Brak zbędnych x_i^0(x_{i+1}^0(...))
// Wielomiany zerowe zbite do wpółczynnika zera (jak wyżej) aby działała funkcja

// TODO
// PolyPrint    [+]
// PolyDestroy  [+]
// PolyClone    [+]
// PolyAdd      [?]
// PolyAddMonos [?]
// PolyMul
// PolyNeg      [?]
// PolySub      [+]
// PolyDegBy
// PolyDeg
// PolyIsEq
// PolyAt

void printPoly(const Poly *p, int id);

void printMono(const Mono *m, int id) {
    printf("x_{%d}^{%d}(", id, m->exp);
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

    return compareExps(x->exp, y->exp);
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

void PolyDestroy(Poly *p) {
    if (PolyIsCoeff(p))
        return;

    for (size_t i = 0; i < p->size; ++i) {
        MonoDestroy(&p->arr[i]);
    }
}

Poly PolyClone(const Poly *p) {
    Poly poly;

    if (PolyIsCoeff(p)) {
        poly = PolyFromCoeff(p->coeff);
    }
    else {
        poly.size = p->size;
        poly.arr = calloc(p->size, sizeof(Mono));

        for (size_t i = 0; i < p->size; ++i)
            poly.arr[i] = MonoClone(&p->arr[i]);
    }

    return poly;
}

void addToMonos(Poly *p, Mono *m) {
    p->size++;
    p->arr = realloc(p->arr, p->size * sizeof(Mono));
    p->arr[p->size - 1] = *m;
}

Poly addCoeffPoly(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && PolyIsCoeff(q));
    return PolyFromCoeff(p->coeff + q->coeff);
}

Poly addNonCoeffAndCoeffPoly(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && PolyIsCoeff(q));

    Poly s = PolyClone(p);
    size_t lastMono = p->size - 1;
    poly_exp_t lastExp = MonoGetExp(&p->arr[lastMono]);

    if (lastExp == 0) {
        Poly *newLastPoly = malloc(sizeof(Poly));
        *newLastPoly = PolyAdd(&p->arr[lastMono].p, q);

        MonoDestroy(&s.arr[lastMono]);
        s.arr[lastMono] = MonoFromPoly(newLastPoly, 0);
    } else {
        Poly *newPoly = malloc(sizeof(Poly));
        *newPoly = PolyFromCoeff(q->coeff);

        Mono newMono = MonoFromPoly(newPoly, 0);
        addToMonos(&s, &newMono);
    }

    return s;
}

Poly addTwoNonCoeffPolys(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && !PolyIsCoeff(q));

    size_t allSize = p->size + q->size;
    Mono *allMonos = malloc(sizeof(Mono) * allSize);

    for (size_t i = 0; i < p->size; i++) {
        allMonos[i] = MonoClone(&p->arr[i]);
    }

    for (size_t i = 0; i < q->size; i++) {
        allMonos[i + p->size] = MonoClone(&q->arr[i]);
    }

    Poly s = PolyAddMonos(allSize, allMonos);

    free(allMonos);

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

    printf("NEW===================\n");
    Mono *allMonos = malloc(sizeof(Mono) * count);
    for (size_t i = 0; i < count; i++) {
        allMonos[i] = MonoClone(&monos[i]);
        printMono(&allMonos[i], 987); printf("\n");
    }
    printf("END==================\n\n");

    sortMonosByExp(allMonos, count);

    size_t uniqueExp = count;
    for (size_t i = 1; i < count; ++i) {
        if (allMonos[i].exp == allMonos[i - 1].exp) {
            --uniqueExp;

            Poly newPoly = PolyAdd(&allMonos[i].p, &allMonos[i - 1].p);

            MonoDestroy(&allMonos[i - 1]);
            MonoDestroy(&allMonos[i]);

            if (!PolyIsCoeff(&newPoly) && newPoly.size == 1 && (newPoly.arr[0].exp == 0 || PolyIsZero(&newPoly.arr[0].p))) {
                allMonos[i].p = newPoly.arr[0].p;
            }
            else {
                allMonos[i].p = newPoly;
            }

            allMonos[i - 1].exp = NOT_A_MONO;

            if (PolyIsZero(&newPoly)) {
                --uniqueExp;
                allMonos[i].exp = NOT_A_MONO;
            }
        }
    }

    if (uniqueExp == 0) {
        return PolyZero();
    }
    else if (uniqueExp == 1) {
        for (size_t i = 0; i < count; ++i) {
            if (allMonos[i].exp != NOT_A_MONO && PolyIsCoeff(&allMonos[i].p) && allMonos[i].exp == 0) {
                return PolyFromCoeff(allMonos[i].p.coeff);
            }
        }
    }

    Poly s = {.size = uniqueExp, .arr = malloc(sizeof(Mono) * uniqueExp)};

    size_t ptr = 0;
    for (size_t i = 0; i < count; ++i) {
        if (allMonos[i].exp != NOT_A_MONO) {
            s.arr[ptr++] = allMonos[i];
        }
    }

    free(allMonos);

    return s;
}

Poly PolyNeg(const Poly *p) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff(-p->coeff);

    Poly cloned = PolyClone(p);
    for (size_t i = 0; i < p->size; i++) {
        cloned.arr[0].p = PolyNeg(&p->arr[0].p);
    }

    return cloned;
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly negatedQ = PolyNeg(q);
    return PolyAdd(p, &negatedQ);
}


