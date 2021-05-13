#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "stack.h"

void handleZero(const char *str, size_t lineNumber, Stack *stack);

void handleIsCoeff(const char *str, size_t lineNumber, Stack *stack);

void handleIsZero(const char *str, size_t lineNumber, Stack *stack);

void handleClone(const char *str, size_t lineNumber, Stack *stack);

void handleAdd(const char *str, size_t lineNumber, Stack *stack);

void handleMul(const char *str, size_t lineNumber, Stack *stack);

void handleNeg(const char *str, size_t lineNumber, Stack *stack);

void handleSub(const char *str, size_t lineNumber, Stack *stack);

void handleIsEq(const char *str, size_t lineNumber, Stack *stack);

void handleDeg(const char *str, size_t lineNumber, Stack *stack);

void handleDegBy(const char *str, size_t lineNumber, Stack *stack);

void handleAt(const char *str, size_t lineNumber, Stack *stack);

void handlePrint(const char *str, size_t lineNumber, Stack *stack);

void handlePop(const char *str, size_t lineNumber, Stack *stack);

#endif //COMMAND_HANDLER_H
