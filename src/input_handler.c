#include "input_handler.h"

bool isComment(const char *str) {
    return *str == '#';
}

bool isEmpty(const char *str) {
    return *str == '\0';
}
