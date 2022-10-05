#include "header.h"

int stack_init(Stack *stack, int size)
{
    memset(stack, 0, sizeof(Stack));
    stack->base = (void **)calloc(size, sizeof(void *));
    if (NULL == stack->base)
    {
        return -1;
    }
    stack->top = stack->base;
    stack->size = size;
    stack->len = 0;
    return 0;
}

int stack_push(Stack *stack, void *node)
{
    if (stack->top >= stack->base + stack->size)
    {
        return -1;
    }
    *(stack->top) = node;
    stack->top++;
    stack->len++;
    return 0;
}

void *stack_top(Stack *stack)
{
    if (stack->top == stack->base)
    {
        return NULL;
    }
    void *p = *(stack->top - 1);
    return p;
}

void stack_pop(Stack *stack)
{
    if (stack->top == stack->base)
    {
        return;
    }
    stack->top--;
    stack->len--;
    *(stack->top) = NULL;
}

void stack_free(Stack *stack)
{
    free(stack->base);
    stack->base = NULL;
    stack->top = NULL;
    stack->size = 0;
}