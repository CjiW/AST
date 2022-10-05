#include "header.h"

int level;
IdentNode *symbol_table_;

void tableInit()
{
    symbol_table_ = (IdentNode *)malloc(sizeof(IdentNode));
    symbol_table_->level = symbol_table_->type = -1;
    symbol_table_->next = NULL;
    level = 0;
}

int insertIdent(char *token_text, int ident_type)
{
    if (checkIdent(token_text, VAR) || checkIdent(token_text, FUNC))
    {
        return 1;
    }
    IdentNode *p = (IdentNode *)malloc(sizeof(IdentNode));
    p->type = ident_type;
    p->level = level;
    strcpy(p->name, token_text);
    p->next = symbol_table_->next;
    symbol_table_->next = p;
    return 0;
}

int checkIdent(char *token_text, int ident_type)
{
    IdentNode *p = symbol_table_->next;
    for (; p != NULL; p = p->next)
    {
        if (p->level <= level && p->type == ident_type && strcmp(p->name, token_text) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void removeIdent()
{
    IdentNode *p, *q;
    for (p = symbol_table_; p->next != NULL; p = p->next)
    {
        while (p->next->level == level)
        {
            q = p->next;
            p->next = q->next;
            free(q);
        }
    }
}
