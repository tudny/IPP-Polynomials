#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "stack.h"

void handleZero(char *str, size_t lineNumber, Stack *stack);

void handleIsCoeff(char *str, size_t lineNumber, Stack *stack);

void handleIsZero(char *str, size_t lineNumber, Stack *stack);

void handleClone(char *str, size_t lineNumber, Stack *stack);

void handleAdd(char *str, size_t lineNumber, Stack *stack);

void handleMul(char *str, size_t lineNumber, Stack *stack);

void handleNeg(char *str, size_t lineNumber, Stack *stack);

void handleSub(char *str, size_t lineNumber, Stack *stack);

void handleIsEq(char *str, size_t lineNumber, Stack *stack);

void handleDeg(char *str, size_t lineNumber, Stack *stack);

void handleDegBy(char *str, size_t lineNumber, Stack *stack);

void handleAt(char *str, size_t lineNumber, Stack *stack);

void handlePrint(char *str, size_t lineNumber, Stack *stack);

void handlePop(char *str, size_t lineNumber, Stack *stack);

#endif //COMMAND_HANDLER_H
