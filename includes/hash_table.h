#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef AST_HASH_TABLE_H
#define AST_HASH_TABLE_H
#define HASH_SIZE 128
#define PUSH_OK 0
#define PUSH_ERR_FULL 1
#define PUSH_ERR_EXSIT 2

typedef struct Ident {
    unsigned int key;
    char name[128];
}Ident;
Ident *idents_[HASH_SIZE];
unsigned int BKDRHash(char str[128]);
int Push(char name[128]);
int Check(char name[128]);
int Remove(char name[128]);
#endif //AST_HASH_TABLE_H
