#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <stdbool.h>
#include "stack.h"

bool isComment(const char *str);
bool isEmpty(const char *str);
void printError(size_t lineNumber, char *error);
void wrongCommand(size_t lineNumber);
void stackUnderflow(size_t lineNumber);
bool pretendsToBeCommand(const char *str);
void handleCommand(char *str, size_t lineNumber, Stack *stack);
void handlePoly(char *str, size_t lineNumber, Stack *stack);

#endif //INPUT_HANDLER_H
