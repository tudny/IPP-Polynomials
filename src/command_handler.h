#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "stack.h"

/**
 * Obsługa komendy ZERO.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleZero(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy IS_COEFF.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleIsCoeff(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy IS_ZERO.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleIsZero(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy CLONE.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleClone(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy ADD.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleAdd(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy MUL.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleMul(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy NEG.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleNeg(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy SUB.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleSub(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy IS_EQ.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleIsEq(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy DEG.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleDeg(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy DEG_BY.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleDegBy(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy AT.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handleAt(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy PRINT.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handlePrint(char *str, size_t lineNumber, Stack *stack);

/**
 * Obsługa komendy POP.
 * @param str : komenda wprowadzona przez użytkownika
 * @param lineNumber : numer linii
 * @param stack : stos kalkulatora
 * */
void handlePop(char *str, size_t lineNumber, Stack *stack);

#endif //COMMAND_HANDLER_H
