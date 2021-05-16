/** @file
 * Interfejs modułu odpowiedzialnego za obsługę komend.
 *
 * Poprawne komendy to
 * - ZERO – wstawia na wierzchołek stosu wielomian tożsamościowo równy zeru;
 * - IS_COEFF – sprawdza, czy wielomian na wierzchołku stosu jest współczynnikiem – wypisuje na standardowe wyjście 0 lub 1;
 * - IS_ZERO – sprawdza, czy wielomian na wierzchołku stosu jest tożsamościowo równy zeru – wypisuje na standardowe wyjście 0 lub 1;
 * - CLONE – wstawia na stos kopię wielomianu z wierzchołka;
 * - ADD – dodaje dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich sumę;
 * - MUL – mnoży dwa wielomiany z wierzchu stosu, usuwa je i wstawia na wierzchołek stosu ich iloczyn;
 * - NEG – neguje wielomian na wierzchołku stosu;
 * - SUB – odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem, usuwa je i wstawia na wierzchołek stosu różnicę;
 * - IS_EQ – sprawdza, czy dwa wielomiany na wierzchu stosu są równe – wypisuje na standardowe wyjście 0 lub 1;
 * - DEG – wypisuje na standardowe wyjście stopień wielomianu (−1 dla wielomianu tożsamościowo równego zeru);
 * - DEG_BY idx – wypisuje na standardowe wyjście stopień wielomianu ze względu na zmienną o numerze idx (−1 dla wielomianu tożsamościowo równego zeru);
 * - AT x – wylicza wartość wielomianu w punkcie x, usuwa wielomian z wierzchołka i wstawia na stos wynik operacji;
 * - PRINT – wypisuje na standardowe wyjście wielomian z wierzchołka stosu;
 * - POP – usuwa wielomian z wierzchołka stosu.
 *
 * @author Aleksander Tudruj
 * @date 17.05.2021
 * */


#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "stack.h"

/**
 * Obsługa komendy ZERO.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleZero(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy IS_COEFF.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleIsCoeff(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy IS_ZERO.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleIsZero(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy CLONE.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleClone(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy ADD.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleAdd(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy MUL.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleMul(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy NEG.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleNeg(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy SUB.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleSub(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy IS_EQ.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleIsEq(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy DEG.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleDeg(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy DEG_BY.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleDegBy(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy AT.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handleAt(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy PRINT.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handlePrint(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy POP.
 * @param[in] str : komenda wprowadzona przez użytkownika
 * @param[in] lineNumber : numer linii
 * @param[in,out] stack : stos kalkulatora
 * */
void handlePop(char *str, size_t lineNumber, Stack *stack);

#endif //COMMAND_HANDLER_H
