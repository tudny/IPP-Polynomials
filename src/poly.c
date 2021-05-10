/** @file
  Implementacja podstawowych operacji na wielomianach rzadkich wielu zmiennych

  Niezmienniki zachowane w każdym wielomianie to:
  - posortowane jednomiany po potęgach malejąco
  - wielomiany są maksymalnie uproszczone
  - w szczególności nie występują jednomiany o zerowych wielomianach

  @author Aleksander Tudruj <at429620@students.mimuw.edu.pl>
  @date 05/02/2021
 */

#include <stdlib.h>
#include <stdio.h>
#include "poly.h"

/**
 * Bezpieczne zamknięcie programu w przypadku wskaźnika na NULL.
 * Sprawdzenie czy wskaźnik wskazuje na NULL. Jeżeli tak, program kończy
 * się kodem błędu EXIT_FAILURE (1).
 * @param[in] ptr : sprawdzany wskaźnik
 * */
static void *requireNotNull(void *ptr) {
    if (ptr == NULL) {
        exit(EXIT_FAILURE);
    }

    return ptr;
}

/**
 * Bezpieczna alternatywa malloc'a.
 * Funkcja działa podobnie do standardowej funkcji malloc, lecz w przypadku
 * braku pamięci program zakończony zostaje kodem błędu EXIT_FAILURE (1).
 * @param[in] memoryBlockSize : rezerwowany rozmiar w pamięci w bajtach
 * */
static void *safeMalloc(size_t memoryBlockSize) {
    return requireNotNull(malloc(memoryBlockSize));
}

/**
 * Bezpieczna alternatywa calloc'a.
 * Funkcja działa podobnie do standardowej funkcji calloc, lecz w przypadku
 * braku pamięci program zakończony zostaje kodem błędu EXIT_FAILURE (1).
 * @param[in] numberOfElements : liczba rezerwowanych elementów
 * @param[in] sizeOfElement : rozmiar elementu w bajtach
 * */
static void *safeCalloc(size_t numberOfElements, size_t sizeOfElement) {
    return requireNotNull(calloc(numberOfElements, sizeOfElement));
}

/**
 * Bezpieczna alternatywa free'a.
 * Funkcja działa podobnie do standardowej funkcji free, lecz po zwolnieniu
 * pamięci wskaźnik zostanie ustawiony na NULL.
 * @param[in,out] ptr : wskaźnik na czyszczony wskaźnik
 * */
static void safeFree(void **ptr) {
    free(*ptr);
    *ptr = NULL;
}

/**
 * Sprawdzenie czy wielomian @f$p@f$ ma posortowaną tablicę jednomianów
 * po wykładnikach malejąco. Funkcja przydatna do asercji.
 * @param[in] p : sprawdzany wielomian @f$p@f$
 * @return czy wielomian @f$p@f$ ma dobrze posortowane jednomiany
 * */
static bool isSorted(const Poly *p) {
    if (PolyIsCoeff(p))
        return true;

    for (size_t i = 1; i < p->size; ++i) {
        if (MonoGetExp(&p->arr[i - 1]) < MonoGetExp(&p->arr[i])) {
            return false;
        }
    }

    return true;
}

/**
 * Sprawdzenie czy jednomian może być skrócony.
 * Funkcja sprawdza czy wprowadzony jednomian może zostać skrócony,
 * to znaczy czy jest postaci @f$p_{i}^0\cdot C@f$, gdzie @f$C\in\mathbb{Z}@f$.
 * @param[in] m : sprawdzany jednomian
 * @return czy jednomian może być skrócony
 * */
static inline bool canMonoBeCut(const Mono *m) {
    return MonoGetExp(m) == 0 && PolyIsCoeff(&m->p);
}

/**
 * Sprawdzenie czy wielomian zawiera dokładnie jeden skracalny jednomian.
 * Funkcja sprawdza czy wielomian zawiera dokładnie jeden jednomian, który
 * spełnia warunek opisany w canMonoBeCut(). Jeżeli tak, można ten wielomian
 * skrócić z postaci @f$P(x_0) = p_{0}^0\cdot C@f$ do @f$P(x_0)=C@f$,
 * gdzie @f$C\in\mathbb{Z}@f$.
 * @param[in] p : sprawdzany wielomian
 * @return czy wielomian może być uproszczony
 * */
static inline bool isPolySingleMonoAndZeroExpCoeff(const Poly *p) {
    if (PolyIsCoeff(p))
        return false;

    return p->size == 1 && canMonoBeCut(&p->arr[0]);
}

/**
 * Sprawdzenie poprawności formy zapisu wielomianu.
 * Sprawdzenie czy przekazany wielomian jest poprawnie zapisany ze względu
 * na zapisane powyżej niezmienniki.
 * Sprawdzenie obejmuje czynności takie jak:
 * - niezerowość tablicy jednomianów
 * - nieskracalność wielomianu
 * - posortowanie jednomianów
 * @param[in] p : sprawdzany wielomian
 * @return czy wielomian jest poprawnie zapisany
 * */
static inline bool hasProperForm(const Poly *p) {
    if (PolyIsCoeff(p))
        return true;

    if (p->size < 1)
        return false;

    if (isPolySingleMonoAndZeroExpCoeff(p))
        return false;

    if (!isSorted(p))
        return false;

    return true;
}

static void printMonoLaTeX(const Mono *m, int idx);
static void printPolyLaTeX(const Poly *p, int idx);

static void printMonoNormalized(const Mono *m, int idx);
static void printPolyNormalized(const Poly *p, int idx);

/**
 * Wypisanie jednomianu LaTeXowo.
 * Wypisanie jednomianu w czytelnej postaci.
 * @param[in] m : wypisywany jednomian.
 * @param[in] idx : identyfikator zmiennej @f$x@f$
 * */
static void printMonoLaTeX(const Mono *m, poly_exp_t idx) {
    printf("x_{%d}^{%d}(", idx, m->exp);
    printPolyLaTeX(&m->p, idx + 1);
    printf(")");
}

/**
 * Wypisanie jednomianu znormalizowanego.
 * Wypisanie jednomianu w czytelnej postaci.
 * @param[in] m : wypisywany jednomian.
 * @param[in] idx : identyfikator zmiennej @f$x@f$
 * */
static void printMonoNormalized(const Mono *m, poly_exp_t idx) {
    printf("(");
    printPolyNormalized(&m->p, idx + 1);
    printf(",%d)", m->exp);
}

/**
 * Wypisanie wielomianu LaTeXowo.
 * Wypisanie wielomianu w czytelnej postaci.
 * @param[in] p : wypisywany wielomian.
 * @param[in] idx : identyfikator zmiennej @f$x@f$
 * */
static void printPolyLaTeX(const Poly *p, poly_exp_t idx) {
    if (PolyIsCoeff(p)) {
        printf("%ld", p->coeff);
    }
    else {
        for (size_t i = 0; i < p->size; ++i) {
            if (i != 0) printf(" + ");
            printMonoLaTeX(&p->arr[i], idx);
        }
    }
}

/**
 * Wypisanie wielomianu znormalizowanego.
 * Wypisanie wielomianu w czytelnej postaci.
 * @param[in] p : wypisywany wielomian.
 * @param[in] idx : identyfikator zmiennej @f$x@f$
 * */
static void printPolyNormalized(const Poly *p, int idx) {
    if (PolyIsCoeff(p)) {
        printf("%ld", p->coeff);
    }
    else {
        // Zmienna musi być przesunięta. W przeciwnym razie wystąpi underflow.
        for (size_t i = p->size; i > 0; --i) {
            printMonoNormalized(&p->arr[i - 1], idx);
            if (i - 1 != 0) printf("+");
        }
    }
}

/**
 * Porównanie wykładników.
 * Porównanie wykładników co do wartości. Zwracana wartość @f$x@f$ spełnia
 * - @f$x<0@f$ gdy @f$a<b@f$
 * - @f$x=0@f$ gdy @f$a=b@f$
 * - @f$x>0@f$ gdy @f$a>b@f$
 * @param[in] a : porównywany wykładnik
 * @param[in] b : porównywany wykładnik
 * @return wyżej opisany @f$x@f$
 * */
static int compareExps(poly_exp_t a, poly_exp_t b) {
    if (a < b) return -1;
    return a > b;
}

/**
 * Porównanie jednomianów w porządku malejącym po wykładnikach.
 * Funkcja zwraca wartość @f$x@f$ spełniającą warunek
 * - @f$x<0@f$ gdy @f$a@f$ przed @f$b@f$
 * - @f$x=0@f$ gdy @f$a@f$ na równi z @f$b@f$
 * - @f$x>0@f$ gdy @f$a@f$ za @f$b@f$
 * @param[in] a : wskaźnik na porównywany jednomian
 * @param[in] b : wskaźnik na porównywany jednomian
 * @return wyżej opisany @f$x@f$
 * */
static int compareMonosByExp(const void *a, const void *b) {
    Mono *x = (Mono *)a;
    Mono *y = (Mono *)b;

    return -compareExps(MonoGetExp(x), MonoGetExp(y));
}

/**
 * Sortowanie tablicy jednomianów w porządku zadanym przez compareMonosByExp().
 * @param[in] monos : sortowana tablica
 * @param[in] size : rozmiar sortowanej tablicy
 * */
static void sortMonosByExp(Mono *monos, size_t size) {
    qsort(monos, size, sizeof(Mono), compareMonosByExp);
}

/**
 * Algorytm szybkiego potęgowania.
 * Szybkie wyznaczenie liczby @f$a^n@f$.
 * @param[in] a : podstawa
 * @param[in] n : wykładnik
 * @return @f$a^n@f$
 * */
static inline poly_coeff_t fastPower(poly_coeff_t a, poly_exp_t n) {
    if (n == 0)
        return 1;

    poly_coeff_t score = fastPower(a * a, n / 2);

    if (n % 2 == 1)
        score *= a;

    return score;
}

/**
 * Wyznaczenie maksimum z dwóch liczb.
 * Funkcja zwraca maksimum z dwóch liczb typu poly_exp_t.
 * @param[in] a : pierwsza liczba
 * @param[in] b : druga liczba
 * @return max@f$(a, b)@f$
 * */
static inline poly_exp_t max(poly_exp_t a, poly_exp_t b) {
    return (a > b) ? a : b;
}

/**
 * Dodanie dwóch jednomianów stałych.
 * Dla dwóch wielomianów stałych zwracana jest ich suma w postaci wielomianu.
 * @param[in] p : wielomian @f$p(x_0)=c_1@f$
 * @param[in] q : wielomian @f$q(x_0)=c_2@f$
 * @return @f$s(x_0) = c_1 + c_2@f$
 * */
static Poly addCoeffPolys(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && PolyIsCoeff(q));

    return PolyFromCoeff(p->coeff + q->coeff);
}

/**
 * Dodanie dwóch wielomianów niestałych.
 * Dodanie wielomianu
 * @f$p(x_0) = \sum\limits_{i\in\mathbb{I}} x_0^i\cdot p_{0,i}(x_1)@f$
 * oraz wielomianu
 * @f$q(x_0) = \sum\limits_{i\in\mathbb{I}} x_0^i\cdot q_{0,i}(x_1)@f$.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p+q@f$
 * */
static Poly addTwoNonCoeffPolys(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && !PolyIsCoeff(q));

    size_t allSize = p->size + q->size;
    Mono *allMonos = safeCalloc(allSize, sizeof(Mono));

    for (size_t i = 0; i < p->size; i++) {
        allMonos[i] = MonoClone(&p->arr[i]);
    }

    for (size_t i = 0; i < q->size; i++) {
        allMonos[i + p->size] = MonoClone(&q->arr[i]);
    }

    Poly s = PolyAddMonos(allSize, allMonos);

    safeFree((void **) &allMonos);
    return s;
}

/**
 * Dodanie wielomianu niestałego i stałego.
 * Dodanie wielomianu
 * @f$p(x_0) = \sum\limits_{i\in\mathbb{I}} x_0^i\cdot p_{0,i}(x_1)@f$
 * oraz wielomianu @f$q(x_0)=c@f$, gdzie @f$c\in\mathbb{Z}@f$.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] q : wielomian @f$q@f$
 * @return @f$p+q@f$
 * */
static Poly addNonCoeffAndCoeffPoly(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && PolyIsCoeff(q));

    Poly tempPoly = { .size = 1, .arr = safeMalloc(sizeof(Mono)) };
    tempPoly.arr[0] = MonoFromPoly(q, 0);

    Poly s = addTwoNonCoeffPolys(p, &tempPoly);
    PolyDestroy(&tempPoly);

    return s;
}

/**
 * Pomnożenie wielomianu przez stałą.
 * Funkcja mnoży wielomian przez stałą zachowując przy tym konstrolę
 * nad przekręceniem się liczby całkowitej i normalizacją wielomianu.
 * @param[in] p : wielomian @f$p@f$
 * @param[in] c : mnożona stała
 * @return @f$p\cdot c@f$
 * */
static Poly multPolyByConst(const Poly *p, poly_coeff_t c) {

    // Special case - much faster then normal computation and makes less trash.
    if (c == 0)
        return PolyZero();
    if (PolyIsCoeff(p))
        return PolyFromCoeff(c * p->coeff);

    Mono *monos = safeCalloc(p->size, sizeof(Mono));

    for (size_t i = 0; i < p->size; i++) {
        Poly newPoly = multPolyByConst(&p->arr[i].p, c);
        poly_exp_t newExp = MonoGetExp(&p->arr[i]);

        // Stworzony na tę chwilę jednomian może być zerowy, ale
        // mieć przed sobą x'a w niezerowej potędze. Użytkownik
        // nie powinien móc doprowadzić do takiej sytuacji, lecz
        // następna funkcja pozbędzie się niepoprawnie stworzonych
        // jednomianów zerowych.
        monos[i].exp = newExp;
        monos[i].p = newPoly;
    }

    Poly r = PolyAddMonos(p->size, monos);

    safeFree((void **) &monos);
    return r;
}

/**
 * Mnożenie dwóch wielomianów stałych.
 * @param[in] p : wielomian stały @f$p(x_0)=c_1@f$
 * @param[in] q : wielomian stały @f$q(x_0)=c_2@f$
 * @return @f$p\cdot q@f$
 * */
static Poly mulCoeffPoly(const Poly *p, const Poly *q) {
    assert(PolyIsCoeff(p) && PolyIsCoeff(q));
    return PolyFromCoeff(p->coeff * q->coeff);
}

/**
 * Mnożenie wielomianu stałego i niestałego.
 * @param[in] p : wielomian niestały @f$p@f$
 * @param[in] q : wielomian stały @f$q@f$
 * @return @f$p\cdot q@f$
 * */
static Poly mulNonCoeffAndCoeffPoly(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && PolyIsCoeff(q));
    return multPolyByConst(p, q->coeff);
}

/**
 * Mnożenie dwóch wielomianów niestałych.
 * @param[in] p : wielomian niestały @f$p@f$
 * @param[in] q : wielomian niestały @f$q@f$
 * @return @f$p\cdot q@f$
 * */
static Poly mulTwoNonCoeffPoly(const Poly *p, const Poly *q) {
    assert(!PolyIsCoeff(p) && !PolyIsCoeff(q));
    assert(isSorted(p) && isSorted(q));

    size_t ws = 0, allSize = p->size * q->size;
    Mono *allMonos = safeCalloc(allSize, sizeof(Mono));

    for (size_t i = 0; i < p->size; i++) {
        for (size_t j = 0; j < q->size; j++) {
            allMonos[ws].p = PolyMul(&p->arr[i].p, &q->arr[j].p);
            allMonos[ws].exp = p->arr[i].exp + q->arr[j].exp;
            ws++;
        }
    }

    Poly s = PolyAddMonos(allSize, allMonos);

    safeFree((void **) &allMonos);
    return s;
}

/**
 * Pomocnicza funkcja do obliczania stopnia wielomianu.
 * Funkcja wywołuje się rekurencyjnie, aż do odpowiedniego poziomu,
 * gdzie poprawia wynik zapisany w zmiennej.
 * @param[in] p : wielomian @f$p@f$, którego stopień sprawdzamy
 * @param[in] actIdx : aktualna zmienna @f$x_{actIdx}@f$
 * @param[in] varIdx : szukana zmienna
 * @param[out] acc : wskaźnik na zmienną z wynikiem
 * @return stopień wielomianu @f$p@f$ po zmiennej @f$x_{varIdx}@f$
 * */
static void degBy(const Poly *p, size_t actIdx, size_t varIdx, poly_exp_t *acc) {
    assert(hasProperForm(p));

    if (PolyIsCoeff(p))
        return;

    if (actIdx == varIdx) {
        // jednomiany są posortowane, więc pierwszy na największy wykładnik
        *acc = max(*acc, MonoGetExp(&p->arr[0]));
    }
    else {
        for (size_t i = 0; i < p->size; ++i) {
            degBy(&p->arr[i].p, actIdx + 1, varIdx, acc);
        }
    }
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
    assert(hasProperForm(p) && hasProperForm(q));

    if (PolyIsCoeff(p) && PolyIsCoeff(q)) {
        return addCoeffPolys(p, q);
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

    Mono *allMonos = safeCalloc(count, sizeof(Mono));
    size_t allSize = 0;

    for (size_t i = 0; i < count; i++) {
        if (!PolyIsZero(&monos[i].p)) {
            allMonos[allSize++] = monos[i];
        }
    }

    sortMonosByExp(allMonos, allSize);

    size_t uniqueExp = allSize;
    bool *isProper = safeCalloc(allSize, sizeof(bool));
    for (size_t i = 0; i < allSize; ++i) {
        isProper[i] = true;
    }

    for (size_t i = 1; i < allSize; ++i) {
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

    Poly result;
    bool isResultSet = false;

    if (uniqueExp == 0) {
        result = PolyZero();
        isResultSet = true;
    }
    else if (uniqueExp == 1) {
        for (size_t i = 0; i < allSize; ++i) {
            if (isProper[i] && canMonoBeCut(&allMonos[i])) {
                result = allMonos[i].p;
                isResultSet = true;
            }
        }
    }

    if (!isResultSet) {
        result = (Poly) {.size = uniqueExp, .arr = safeMalloc(
                sizeof(Mono) * uniqueExp)};

        size_t ptr = 0;
        for (size_t i = 0; i < allSize; ++i) {
            if (isProper[i]) {
                result.arr[ptr++] = allMonos[i];
            }
        }
    }

    safeFree((void **) &allMonos);
    safeFree((void **) &isProper);
    return result;
}

Poly PolyMul(const Poly *p, const Poly *q) {
    assert(hasProperForm(p) && hasProperForm(q));

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

Poly PolyNeg(const Poly *p) {
    assert(hasProperForm(p));
    return multPolyByConst(p, -1);
}

Poly PolySub(const Poly *p, const Poly *q) {
    assert(hasProperForm(p) && hasProperForm(q));

    Poly negatedQ = PolyNeg(q);
    Poly added = PolyAdd(p, &negatedQ);

    PolyDestroy(&negatedQ);
    return added;
}

poly_exp_t PolyDegBy(const Poly *p, size_t varIdx) {
    assert(hasProperForm(p));

    if (PolyIsZero(p))
        return -1;

    poly_exp_t cnt = 0;
    degBy(p, 0, varIdx, &cnt);

    return cnt;
}

poly_exp_t PolyDeg(const Poly *p) {
    assert(hasProperForm(p));

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

bool PolyIsEq(const Poly *p, const Poly *q) {
    assert(hasProperForm(p) && hasProperForm(q));

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
    assert(hasProperForm(p));

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

void PrintPolyLaTeX(const Poly *p, char *label) {
    printf("[%s]: ", label);
    printPolyLaTeX(p, 0);
    printf("\n");
}

void PrintPolyNormalized(const Poly *p) {
    printPolyNormalized(p, 0);
    printf("\n");
}
