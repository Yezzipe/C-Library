#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

stack_elm_t* consStackElem();
void setStackElemValue(stack_elm_t* elem, void* value);
void* getStackElemValue(stack_elm_t* elem);
void setStackElemNext(stack_elm_t* elem, stack_elm_t* next_elem);

stack_elm_t* consStackElem() {
    stack_elm_t* elem = malloc(sizeof(stack_elm_t));
    elem->value = NULL;
    elem->next_elem = NULL;
    return elem;
}

void setStackElemValue(stack_elm_t* elem, void* value) {
    elem->value = value;
}

void* getStackElemValue(stack_elm_t* elem) {
    return elem->value;
}

void setStackElemNext(stack_elm_t* elem, stack_elm_t* next_elem) {
    elem->next_elem = next_elem;
}

stack_t* consStack() {
    stack_t* stack = malloc(sizeof(stack_t));
    stack->top = NULL;
    stack->n = 0;
    return stack;
}

bool isStackEmpty(stack_t* stack) {
    return stack->n == 0;
}

int getStackSize(stack_t* stack) {
    return stack->n;
}

void pushStack(stack_t* stack, void* value) {
    stack_elm_t* elem = consStackElem();
    setStackElemValue(elem, value);
    if (!isStackEmpty(stack))setStackElemNext(elem, stack->top);
    stack->top = elem;
    stack->n++;
}

void* peekStack(stack_t* stack) {
    if (isStackEmpty(stack)) return NULL;
    return getStackElemValue(stack->top);
}

void* popStack(stack_t* stack) {
    if (isStackEmpty(stack)) return NULL;
    stack_elm_t* elem = stack->top;
    stack->top = elem->next_elem;
    stack->n--;
    return getStackElemValue(elem);
}

void freeStack(stack_t** stack, void (*freeValue)()) {
    while ((*stack)->n > 0) {
        stack_elm_t* elem = (*stack)->top;
        (*stack)->top = elem->next_elem;
        (*stack)->n--;
        void* v = elem->value; 
        if (freeValue != NULL) freeValue(&v);
        free(elem);
    }
    free(*stack);
    *stack = NULL;
}