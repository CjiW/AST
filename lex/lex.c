#include "header.h"

int lexicalAnalyse() {
    int type;
    row_ = 1;
    clearToken();
    if (fr == NULL) {
        printf("fail to open file\n");
        exit(0);
    }
    printf("\nLineNum");
    printf("%10s", "Type");
    printf("\tValue\n");
    do {
        type = getToken(fr);
        if (type != ERROR_TOKEN) {
            printf("[%d]", row_);
            switch (type) {
                case IDENT:
                    printf("%14s", "IDENT");
                    break;
                case INT_CONST:
                    printf("%14s", "INT_CONST");
                    break;
                case LONG_CONST:
                    printf("%14s", "LONG_CONST");
                    break;
                case FLOAT_CONST:
                    printf("%14s", "FLOAT_CONST");
                    break;
                case DOUBLE_CONST:
                    printf("%14s", "DOUBLE_CONST");
                    break;
                case CHAR_CONST:
                    printf("%14s", "CHAR_CONST");
                    break;
                case STRING_CONST:
                    printf("%14s", "STRING_CONST");
                    break;
                case KEYWORD:
                    printf("%14s", "KEYWORD");
                    break;
                case INT:
                    printf("%14s", "INT");
                    break;
                case DOUBLE:
                    printf("%14s", "DOUBLE");
                    break;
                case FLOAT:
                    printf("%14s", "FLOAT");
                    break;
                case CHAR:
                    printf("%14s", "CHAR");
                    break;
                case SHORT:
                    printf("%14s", "SHORT");
                    break;
                case LONG:
                    printf("%14s", "LONG");
                    break;
                case IF:
                    printf("%14s", "IF");
                    break;
                case ELSE:
                    printf("%14s", "ELSE");
                    break;
                case DO:
                    printf("%14s", "DO");
                    break;
                case WHILE:
                    printf("%14s", "WHILE");
                    break;
                case FOR:
                    printf("%14s", "FOR");
                    break;
                case BREAK:
                    printf("%14s", "BREAK");
                    break;
                case RETURN:
                    printf("%14s", "RETURN");
                    break;
                case CONTINUE:
                    printf("%14s", "CONTINUE");
                    break;
                case VOID:
                    printf("%14s", "VOID");
                    break;
                case EQ:
                    printf("%14s", "EQ");
                    break;
                case NEQ:
                    printf("%14s", "NEQ");
                    break;
                case ASSIGN:
                    printf("%14s", "ASSIGN");
                    break;
                case LP:
                    printf("%14s", "LP");
                    break;
                case RP:
                    printf("%14s", "RP");
                    break;
                case LB:
                    printf("%14s", "LB");
                    break;
                case RB:
                    printf("%14s", "RB");
                    break;
                case LM:
                    printf("%14s", "LM");
                    break;
                case RM:
                    printf("%14s", "RM");
                    break;
                case SEMI:
                    printf("%14s", "SEMI");
                    break;
                case COMMA:
                    printf("%14s", "COMMA");
                    break;
                case TIMES:
                    printf("%14s", "TIMES");
                    break;
                case DIVIDE:
                    printf("%14s", "DIVIDE");
                    break;
                case ANNO:
                    printf("%14s", "ANNO");
                    break;
                case PLUS:
                    printf("%14s", "PLUS");
                    break;
                case MINUS:
                    printf("%14s", "MINUS");
                    break;
                case MORE:
                    printf("%14s", "MORE");
                    break;
                case MOREEQ:
                    printf("%14s", "MOREEQ");
                    break;
                case LESS:
                    printf("%14s", "LESS");
                    break;
                case LESSEQ:
                    printf("%14s", "LESSEQ");
                    break;
                case INCLUDE:
                    printf("%14s", "INCLUDE");
                    break;
                case DEFINE:
                    printf("%14s", "DEFINE");
                    break;
                case ARRAY:
                    printf("%14s", "ARRAY");
                    break;
                case AND:
                    printf("%14s", "AND");
                    break;
                case OR:
                    printf("%14s", "OR");
                    break;
                case EOF:
                    printf("%14s", "EOF");
            }
            printf("\t%s\n", token_text_);
        } else {
            printf("\tError in row_ %d\n", row_);
            break;
        }
    } while (type != EOF);
    row_ = 1;
    return 0;
}