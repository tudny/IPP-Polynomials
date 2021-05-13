#include <stdio.h>
#include "stack.h"

static SElem *createSElem(Poly *p) {
    SElem *sElem = safeMalloc(sizeof(SElem));
    sElem->p = *p;

    return sElem;
}

static void destorySingle(SElem *sElem, bool deep) {
    if (deep) PolyDestroy(&sElem->p);
    safeFree((void **) &sElem);
}

static void printSElem(SElem *sElem) {
    if (sElem == NULL) {
        printf("TAIL{null}");
        return;
    }

    printf("Node{");
    PrintPolyNormalized(&sElem->p);
    printf("}-> ");

    printSElem(sElem->next);
}

Stack *createEmptyStack() {
    Stack *stack = safeMalloc(sizeof(Stack));
    stack->head = NULL;
    stack->size = 0;

    return stack;
}

void destoryStack(Stack *stack) {
    SElem *elem = stack->head;
    while (elem != NULL) {
        SElem *temp = elem;
        elem = elem->next;
        destorySingle(temp, true);
    }

    safeFree((void **) &stack);
}

void pushStack(Stack *stack, Poly p) {
    SElem *sElem = createSElem(&p);

    sElem->next = stack->head;
    stack->head = sElem;
    ++stack->size;
}

bool isEmptyStack(Stack *stack) {
    return sizeStack(stack) == 0;
}

size_t sizeStack(Stack *stack) {
    return stack->size;
}

static void popStackDeep(Stack *stack, int deep) {
    assert(!isEmptyStack(stack));

    SElem *sElem = stack->head;
    stack->head = stack->head->next;
    --stack->size;

    destorySingle(sElem, deep);
}

void popStack(Stack *stack) {
    popStackDeep(stack, true);
}

Poly topStack(Stack *stack) {
    assert(!isEmptyStack(stack));

    return stack->head->p;
}

Poly secondTopStack(Stack *stack) {
    assert(sizeStack(stack) >= 2);

    return stack->head->next->p;
}

Poly takeStack(Stack *stack) {
    Poly p = topStack(stack);
    popStackDeep(stack, false);
    return p;
}

void printStack(Stack *stack) {
    printSElem(stack->head);
    printf("\n");
}
