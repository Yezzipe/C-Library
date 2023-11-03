#pragma once

#include <stdbool.h>

typedef struct stack_elm {
    struct stack_elm* next_elem;
    void* value;
} stack_elm_t;

typedef struct stack {
    stack_elm_t* top;
    int n;
} stack_t;

stack_t* consStack();
bool isStackEmpty(stack_t* stack);
int getStackSize(stack_t* stack);

void pushStack(stack_t* stack, void* value);
void* peekStack(stack_t* stack);
void* popStack(stack_t* stack);

void freeStack(stack_t** stack, void (*freeValue)());
