#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <stdbool.h>
#include "stack.h"

bool isComment(const char *str);
bool isEmpty(const char *str);
bool pretendsToBeCommand(const char *str);
void handleCommand(char *str, size_t lineNumber, Stack *stack);
void handlePoly(char *str, size_t lineNumber, Stack *stack);

#endif //INPUT_HANDLER_H
