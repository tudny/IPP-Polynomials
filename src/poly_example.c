#ifdef NDEBUG
#undef NDEBUG
#endif

#include "poly.h"
#include <assert.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define CHECK_PTR(p)  \
  do {                \
    if (p == NULL) {  \
      exit(1);        \
    }                 \
  } while (0)

//#define C PolyFromCoeff
//
//static Mono M(Poly p, poly_exp_t n) {
//  return MonoFromPoly(&p, n);
//}
//
//static Poly MakePolyHelper(poly_exp_t dummy, ...) {
//  va_list list;
//  va_start(list, dummy);
//  size_t count = 0;
//  while (true) {
//    va_arg(list, Poly);
//    if (va_arg(list, poly_exp_t) < 0)
//      break;
//    count++;
//  }
//  va_start(list, dummy);
//  Mono *arr = calloc(count, sizeof (Mono));
//  CHECK_PTR(arr);
//  for (size_t i = 0; i < count; ++i) {
//    Poly p = va_arg(list, Poly);
//    arr[i] = MonoFromPoly(&p, va_arg(list, poly_exp_t));
//    assert(i == 0 || MonoGetExp(&arr[i]) > MonoGetExp(&arr[i - 1]));
//  }
//  va_end(list);
//  Poly res = PolyAddMonos(count, arr);
//  free(arr);
//  return res;
//}
//
//#define P(...) MakePolyHelper(0, __VA_ARGS__, PolyZero(), -1)

//static bool TestOp(Poly a, Poly b, Poly res,
//                   Poly (*op)(const Poly *, const Poly *)) {
//  Poly c = op(&a, &b);
//  bool is_eq = PolyIsEq(&c, &res);
//  PolyDestroy(&a);
//  PolyDestroy(&b);
//  PolyDestroy(&c);
//  PolyDestroy(&res);
//  return is_eq;
//}
//
//static bool TestAdd(Poly a, Poly b, Poly res) {
//  return TestOp(a, b, res, PolyAdd);
//}
//
//static bool TestAddMonos(size_t count, Mono monos[], Poly res) {
//  Poly b = PolyAddMonos(count, monos);
//  bool is_eq = PolyIsEq(&b, &res);
//  PolyDestroy(&b);
//  PolyDestroy(&res);
//  return is_eq;
//}
//
//static bool TestMul(Poly a, Poly b, Poly res) {
//  return TestOp(a, b, res, PolyMul);
//}
//
//static bool TestSub(Poly a, Poly b, Poly res) {
//  return TestOp(a, b, res, PolySub);
//}
//
//static bool TestDegBy(Poly a, size_t var_idx, poly_exp_t res) {
//  bool is_eq = PolyDegBy(&a, var_idx) == res;
//  PolyDestroy(&a);
//  return is_eq;
//}
//
//static bool TestDeg(Poly a, poly_exp_t res) {
//  bool is_eq = PolyDeg(&a) == res;
//  PolyDestroy(&a);
//  return is_eq;
//}
//
//static bool TestEq(Poly a, Poly b, bool res) {
//  bool is_eq = PolyIsEq(&a, &b) == res;
//  PolyDestroy(&a);
//  PolyDestroy(&b);
//  return is_eq;
//}
//
//static bool TestAt(Poly a, poly_coeff_t x, Poly res) {
//  Poly b = PolyAt(&a, x);
//  bool is_eq = PolyIsEq(&b, &res);
//  PolyDestroy(&a);
//  PolyDestroy(&b);
//  PolyDestroy(&res);
//  return is_eq;
//}
//
//static bool SimpleAddTest(void) {
//  bool res = true;
//  // Różne przypadki wielomian/współczynnik
//  res &= TestAdd(C(1),
//                 C(2),
//                 C(3));
//  res &= TestAdd(P(C(1), 1),
//                 C(2),
//                 P(C(2), 0, C(1), 1));
//  res &= TestAdd(C(1),
//                 P(C(2), 2),
//                 P(C(1), 0, C(2), 2));
//  res &= TestAdd(P(C(1), 1),
//                 P(C(2), 2),
//                 P(C(1), 1, C(2), 2));
//  res &= TestAdd(C(0),
//                 P(C(1), 1),
//                 P(C(1), 1));
//  // Upraszczanie wielomianu
//  res &= TestAdd(P(C(1), 1),
//                 P(C(-1), 1),
//                 C(0));
//  res &= TestAdd(P(C(1), 1, C(2), 2),
//                 P(C(-1), 1),
//                 P(C(2), 2));
//  res &= TestAdd(P(C(2), 0, C(1), 1),
//                 P(C(-1), 1),
//                 C(2));
//  // Dodawanie współczynnika i upraszczanie
//  res &= TestAdd(C(1),
//                 P(C(-1), 0, C(1), 1),
//                 P(C(1), 1));
//  res &= TestAdd(C(1),
//                 P(P(C(-1), 0, C(1), 1), 0),
//                 P(P(C(1), 1), 0));
//  res &= TestAdd(C(1),
//                 P(C(1), 0, C(2), 2),
//                 P(C(2), 0, C(2), 2));
//  res &= TestAdd(C(1),
//                 P(P(C(1), 0, C(1), 1), 0, C(2), 2),
//                 P(P(C(2), 0, C(1), 1), 0, C(2), 2));
//  res &= TestAdd(C(1),
//                 P(P(C(-1), 0, C(1), 1), 0, C(2), 2),
//                 P(P(C(1), 1), 0, C(2), 2));
//  // Dodawanie wielomianów wielu zmiennych
//  res &= TestAdd(P(P(C(1), 2), 0, P(C(2), 1), 1, C(1), 2),
//                 P(P(C(1), 2), 0, P(C(-2), 1), 1, C(1), 2),
//                 P(P(C(2), 2), 0, C(2), 2));
//  res &= TestAdd(P(P(C(1), 2), 0, P(C(2), 1), 1, C(1), 2),
//                 P(P(C(-1), 2), 0, P(C(1), 0, C(2), 1, C(1), 2), 1, C(-1), 2),
//                 P(P(C(1), 0, C(4), 1, C(1), 2), 1));
//  // Redukcja do współczynnika
//  res &= TestAdd(P(P(C(1), 0, C(1), 1), 0, C(1), 1),
//                 P(P(C(1), 0, C(-1), 1), 0, C(-1), 1),
//                 C(2));
//  res &= TestAdd(P(P(P(C(1), 0, C(1), 1), 0, C(1), 1), 1),
//                 P(P(P(C(1), 0, C(-1), 1), 0, C(-1), 1), 1),
//                 P(C(2), 1));
//  // Dodawanie wielomianu do siebie
//  Poly a = P(C(1), 1);
//  Poly b = PolyAdd(&a, &a);
//  Poly c = P(C(2), 1);
//  res &= PolyIsEq(&b, &c);
//  PolyDestroy(&a);
//  PolyDestroy(&b);
//  PolyDestroy(&c);
//  return res;
//}
//
//static bool SimpleAddMonosTest(void) {
//  bool res = true;
//  {
//    Mono m[] = {M(C(-1), 0), M(C(1), 0)};
//    res &= TestAddMonos(2, m, C(0));
//  }
//  {
//    Mono m[] = {M(C(-1), 1), M(C(1), 1)};
//    res &= TestAddMonos(2, m, C(0));
//  }
//  {
//    Mono m[] = {M(C(1), 0), M(C(1), 0)};
//    res &= TestAddMonos(2, m, C(2));
//  }
//  {
//    Mono m[] = {M(C(1), 1), M(C(1), 1)};
//    res &= TestAddMonos(2, m, P(C(2), 1));
//  }
//  {
//    Mono m[] = {M(P(C(-1), 1), 0), M(P(C(1), 1), 0)};
//    res &= TestAddMonos(2, m, C(0));
//  }
//  {
//    Mono m[] = {M(P(C(-1), 0), 1),
//                M(P(C(1), 0), 1),
//                M(C(2), 0),
//                M(C(1), 1),
//                M(P(C(2), 1), 2),
//                M(P(C(2), 2), 2)};
//    res &= TestAddMonos(6, m, P(C(2), 0, C(1), 1, P(C(2), 1, C(2), 2), 2));
//  }
//  return res;
//}
//
//static bool SimpleMulTest(void) {
//  bool res = true;
//  res &= TestMul(C(2),
//                 C(3),
//                 C(6));
//  res &= TestMul(P(C(1), 1),
//                 C(2),
//                 P(C(2), 1));
//  res &= TestMul(C(3),
//                 P(C(2), 2),
//                 P(C(6), 2));
//  res &= TestMul(P(C(1), 1),
//                 P(C(2), 2),
//                 P(C(2), 3));
//  res &= TestMul(P(C(-1), 0, C(1), 1),
//                 P(C(1), 0, C(1), 1),
//                 P(C(-1), 0, C(1), 2));
//  res &= TestMul(P(P(C(1), 2), 0, P(C(1), 1), 1, C(1), 2),
//                 P(P(C(1), 2), 0, P(C(-1), 1), 1, C(1), 2),
//                 P(P(C(1), 4), 0, P(C(1), 2), 2, C(1), 4));
//  return res;
//}
//
//static bool SimpleNegTest(void) {
//  Poly a = P(P(C(1), 0, C(2), 2), 0, P(C(1), 1), 1, C(1), 2);
//  Poly b = PolyNeg(&a);
//  Poly c = P(P(C(-1), 0, C(-2), 2), 0, P(C(-1), 1), 1, C(-1), 2);
//  bool is_eq = PolyIsEq(&b, &c);
//  PolyDestroy(&a);
//  PolyDestroy(&b);
//  PolyDestroy(&c);
//  return is_eq;
//}
//
//static bool SimpleSubTest(void) {
//  return TestSub(P(P(C(1), 2), 0, P(C(2), 1), 1, C(1), 2),
//                 P(P(C(1), 2), 0, P(C(-1), 0, C(-2), 1, C(-1), 2), 1, C(1), 2),
//                 P(P(C(1), 0, C(4), 1, C(1), 2), 1));
//}
//
//#define POLY_P P(P(C(1), 3), 0, P(C(1), 2), 2, C(1), 3)
//
//static bool SimpleDegByTest(void) {
//  bool res = true;
//  res &= TestDegBy(C(0), 1, -1);
//  res &= TestDegBy(C(1), 0, 0);
//  res &= TestDegBy(P(C(1), 1), 1, 0);
//  res &= TestDegBy(POLY_P, 0, 3);
//  res &= TestDegBy(POLY_P, 1, 3);
//  return res;
//}
//
//static bool SimpleDegTest(void) {
//  bool res = true;
//  res &= TestDeg(C(0), -1);
//  res &= TestDeg(C(1), 0);
//  res &= TestDeg(P(C(1), 1), 1);
//  res &= TestDeg(POLY_P, 4);
//  return res;
//}
//
//static bool SimpleIsEqTest(void) {
//  bool res = true;
//  res &= TestEq(C(0), C(0), true);
//  res &= TestEq(C(0), C(1), false);
//  res &= TestEq(C(1), C(1), true);
//  res &= TestEq(P(C(1), 1), C(1), false);
//  res &= TestEq(P(C(1), 1), P(C(1), 1), true);
//  res &= TestEq(P(C(1), 1), P(C(1), 2), false);
//  res &= TestEq(P(C(1), 1), P(C(2), 1), false);
//  res &= TestEq(POLY_P, POLY_P, true);
//  Poly a = C(1);
//  Poly b = C(2);
//  Poly p = POLY_P;
//  res &= TestEq(PolyAdd(&p, &a), PolyAdd(&p, &b), false);
//  PolyDestroy(&a);
//  PolyDestroy(&b);
//  PolyDestroy(&p);
//  return res;
//}
//
//static bool SimpleAtTest(void) {
//  bool res = true;
//  res &= TestAt(C(2), 1, C(2));
//  res &= TestAt(P(C(1), 0, C(1), 18), 10, C(1000000000000000001L));
//  res &= TestAt(P(C(3), 1, C(2), 3, C(1), 5), 10, C(102030));
//  res &= TestAt(P(P(C(1), 4), 0, P(C(1), 2), 2, C(1), 3), 2,
//                P(C(8), 0, C(4), 2, C(1), 4));
//  return res;
//}
//
//static bool OverflowTest(void) {
//  bool res = true;
//  res &= TestMul(P(C(1L << 32), 1), C(1L << 32), C(0));
//  res &= TestAt(P(C(1), 64), 2, C(0));
//  res &= TestAt(P(C(1), 0, C(1), 64), 2, C(1));
//  res &= TestAt(P(P(C(1), 1), 64), 2, C(0));
//  return res;
//}
//
//int main() {
//  assert(SimpleAddTest());
//  assert(SimpleAddMonosTest());
//  assert(SimpleMulTest());
//  assert(SimpleNegTest());
//  assert(SimpleSubTest());
//  assert(SimpleDegByTest());
//  assert(SimpleDegTest());
//  assert(SimpleIsEqTest());
//  assert(SimpleAtTest());
//  assert(OverflowTest());
//}

int main() {
//    Poly a = PolyFromCoeff(9);
//    Mono a_m = MonoFromPoly(&a, 2137);
//
//    Poly p;
//    p.size = 1;
//    p.arr = malloc(sizeof(Mono));
//    p.arr[0] = a_m;
//
//    PolyPrint(&p);
//
//    Mono p_m = MonoFromPoly(&p, 4);
//    Poly q = PolyFromCoeff(21);
//    Mono q_m = MonoFromPoly(&q, 8);
//    Poly s;
//    s.size = 2;
//    s.arr = malloc(sizeof(Mono) * 2);
//    s.arr[0] = p_m;
//    s.arr[1] = q_m;
//    PolyPrint(&s);
//    printf("\n");
//
//    Poly u = PolyFromCoeff(17);
//    Poly r = PolyFromCoeff(21);
//    Poly t = PolyAdd(&r, &s);
//    Poly v = PolyAdd(&r, &u);
//
//    PolyPrint(&t);
//    PolyPrint(&v);
//
//    Mono a_1 = MonoFromPoly(&t, 0);
//    Poly A_1 = (Poly) {.arr = malloc(sizeof(Mono)), .size = 1};
//    A_1.arr[0] = a_1;
//    PolyPrint(&A_1);
//
//    Poly B_1 = PolyFromCoeff(420);
//
//    Poly A_B = PolyAdd(&A_1, &B_1);
//    //PolyPrint(&A_B);
//    PrintPolyWithLabel(&A_B, "A_B");
//
//    printf("p_size: %ld | q_size: %ld\n", t.size, A_B.size);
//    Poly C_1 = PolyAdd(&t, &A_B);
//    // PolyPrint(&C_1);
//    PrintPolyWithLabel(&C_1, "C_1");
//



//    Poly ABC = PolyFromCoeff(213);
//    Poly EFG = (Poly) {.arr = malloc(sizeof(Mono)), .size = 1};
//    EFG.arr[0] = MonoFromPoly(&ABC, 17);
//    Poly HIJ = (Poly) {.arr = malloc(sizeof(Mono)), .size = 1};
//    HIJ.arr[0] = MonoFromPoly(&EFG, 43);
//
//
//    Poly ABC_2 = PolyFromCoeff(-213);
//    Poly EFG_2 = (Poly) {.arr = malloc(sizeof(Mono)), .size = 1};
//    EFG_2.arr[0] = MonoFromPoly(&ABC_2, 17);
//    Poly HIJ_2 = (Poly) {.arr = malloc(sizeof(Mono)), .size = 1};
//    HIJ_2.arr[0] = MonoFromPoly(&EFG_2, 43);
//
//    PrintPolyWithLabel(&HIJ, "ABC");
//    PrintPolyWithLabel(&HIJ_2, "ABC_2");
//
//    Poly SUM = PolyAdd(&HIJ, &HIJ_2);
//    PrintPolyWithLabel(&SUM, "SUMA");
//
//
//
//
//    Poly A = PolyFromCoeff(7);
//    Poly B = PolyFromCoeff(5);
//    Poly C = PolyFromCoeff(-5);
//
//    Poly A_B = (Poly) {.arr = malloc(sizeof(Mono) * 2), .size = 2};
//    A_B.arr[0] = MonoFromPoly(&A, 0);
//    A_B.arr[1] = MonoFromPoly(&B, 4);
//
//    Poly A_C = (Poly) {.arr = malloc(sizeof(Mono) * 2), .size = 2};
//    A_C.arr[0] = MonoFromPoly(&A, 0);
//    A_C.arr[1] = MonoFromPoly(&C, 4);
//
//    PrintPolyWithLabel(&A_B, "skladowa 1");
//    PrintPolyWithLabel(&A_C, "skladowa 2");
//
//    Poly A_B_C = PolyAdd(&A_B, &A_C);
//    PrintPolyWithLabel(&A_B_C, "A_B_C");


//    Poly A = PolyFromCoeff(7);
//    Poly D = PolyFromCoeff(8);
//    Poly B = PolyFromCoeff(5);
//    Poly C = PolyFromCoeff(-5);
//
//    Mono *mono = malloc(sizeof(Mono) * 2);
//    mono[0] = MonoFromPoly(&A, 0);
//    mono[1] = MonoFromPoly(&B, 4);
//    Poly A_B = PolyAddMonos(2, mono);
//
//    Mono *mono2 = malloc(sizeof(Mono) * 2);
//    mono2[0] = MonoFromPoly(&D, 0);
//    mono2[1] = MonoFromPoly(&C, 4);
//    Poly A_C = PolyAddMonos(2, mono2);
//
//    PrintPolyWithLabel(&A_B, "skladowa 1");
//    PrintPolyWithLabel(&A_C, "skladowa 2");
//
//    Poly A_B_C = PolyAdd(&A_B, &A_C);
//    PrintPolyWithLabel(&A_B_C, "A_B_C");


    Poly A = PolyFromCoeff(7);
    Poly B = PolyFromCoeff(5);
    Poly C = PolyNeg(&B);

    PrintPolyWithLabel(&A, "A");
    PrintPolyWithLabel(&B, "B");
    PrintPolyWithLabel(&C, "C");

    Mono m_A = MonoFromPoly(&A, 4);
    Mono m_B = MonoFromPoly(&B, 5);
    Mono m_C = MonoFromPoly(&C, 5);

    printMono(&m_A, 123123); printf("\n");
    printMono(&m_B, 123123); printf("\n");
    printMono(&m_C, 123123); printf("\n");

    Mono *mono1 = malloc(sizeof(Mono) * 2);
    mono1[0] = m_A;
    mono1[1] = m_B;

    Mono *mono2 = malloc(sizeof(Mono) * 2);
    mono2[0] = m_A;
    mono2[1] = m_C;

    Poly A_B = PolyAddMonos(2, mono1);
    Poly A_C = PolyAddMonos(2, mono2);

    PrintPolyWithLabel(&A_B, "A_B");
    PrintPolyWithLabel(&A_C, "A_C");

    Poly A_B_A_C = PolyAdd(&A_B, &A_C);

    PrintPolyWithLabel(&A_B, "A_B");
    PrintPolyWithLabel(&A_C, "A_C");
    PrintPolyWithLabel(&A_B_A_C, "A_B_A_C");
}
