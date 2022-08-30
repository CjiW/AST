#include <string.h>
#include <stdlib.h>

#ifndef AST_STACK_H
#define AST_STACK_H
#define stack_maxsize(stack) (stack->size)
#define stack_isempty(stack) (stack->top == stack->base)
#define stack_depth(stack) (stack->top - stack->base)
#define stack_gettop(stack) ((stack->top == stack->base)? NULL: *(stack->top - 1))
typedef struct{
    void **base; /* 栈底 */
    void **top;  /* 栈顶 */
    int len;
    int size;    /* 栈的大小 */
}Stack;
int stack_init(Stack *stack, int size);
int stack_push(Stack *stack, void *node);
void*stack_top(Stack *stack);
void stack_pop(Stack *stack);
void stack_free(Stack *stack);
#endif //AST_STACK_H
