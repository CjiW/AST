#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "token.h"

// #include "hash_table.h"


#ifndef LANUAGE_FORMAT_PROCESSING_GRAMMAR_ANALYSE_H
#define LANUAGE_FORMAT_PROCESSING_GRAMMAR_ANALYSE_H

typedef struct ASTTree {
    struct ASTTree* l;
    struct ASTTree* r;
    int type;
    struct data {
        int type;
        char* data;
    } data;
} ASTTree;

typedef struct VariableDefNode {
    int size;            // define num
    char* variable[20];  // names
    struct VariableDefNode* next;
} VDN;

typedef enum DataType {
    EXTDEFLIST = 1,      //外部定义序列
    EXTVARDEF,           //外部变量定义
    EXTVARTYPE,          //外部变量类型
    EXTVARLIST,          //外部变量名序列
    EXTVAR,              //外部变量名
    FUNCDEF,             //函数定义
    FUNCRETURNTYPE,      //函数返回值类型
    FUNCNAME,            //函数名
    FUNCFORMALPARALIST,  //函数形式参数序列
    FUNCFORMALPARADEF,   //函数形式参数
    FUNCFORMALPARATYPE,  //函数形参类型
    FUNCFORMALPARA,      //函数形参名
    FUNCBODY,            //函数体
    LOCALVARDEFLIST,     //局部变量定义序列
    LOCALVARDEF,         //局部变量定义
    LOCALVARTYPE,        //局部变量类型
    LOCALVARNAMELIST,    //局部变量名序列
    LOCALVARNAME,        //局部变量名
    STATELIST,           //语句序列
    OPERAND,             //操作数
    OPERATOR,            //运算符
    EXPRESSION,          //表达式
    IFPART,              // if语句部分
    ELSEPART,            // else部分
    IFSTATEMENT,         // if语句25
    IFELSESTATEMENT,     // if-else语句
    WHILESTATEMENT,      // while语句结点
    WHILEPART,           // while条件语句结点
    WHILEBODY,           // while语句体
    FORSTATEMENT,        // for语句结点30
    FORPART,             // for条件语句
    FORPART1,            // for语句一
    FORPART2,            // for语句二
    FORPART3,            // for语句三
    FORBODY,             // for语句体
    RETURNSTATEMENT,     // return语句
    BREAKSTATEMENT,      // break语句
    DOWHILESTATEMENT,    // do-while循环语句
    DOWHILEBODY,         // do-while语句体
    DOWHILECONDITION,    // do-while条件
    CONTINUESTATEMENT,   // continue语句
    FUNCCLAIM,           //函数声明
    ARRAYDEF,            //数组定义
    ARRAYTYPE,           //数组类型
    ARRAYNAME,           //数组名45
    ARRAYSIZE            //数组大小
}DataType;

void syntaxAnalyse();

void freeTree(ASTTree* root);

ASTTree* program();

ASTTree* ExtDefList();

ASTTree* ExtDef();

ASTTree* ExtVarDef();

ASTTree* FuncDef();

ASTTree* FormParaList();

ASTTree* FormParaDef();

ASTTree* CompState();

ASTTree* LocalVarDefList();

ASTTree* StateList();

ASTTree* Statement();

ASTTree* Expression(int endsym);

char Precede(int c1, int c2);

void returnToken(FILE* fp);

void showType(int cur_type);

void preorderTranverse(ASTTree* root, int depth);

int addName(char* token_text);

int checkname_exist(char* token_text);

ASTTree* ArrayDef();

#endif  // LANUAGE_FORMAT_PROCESSING_GRAMMAR_ANALYSE_H
