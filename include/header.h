#ifndef AST_HEADER_H
#define AST_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************
 *                Main                *
 **************************************/
char fileIn[128], fileOut[128];
FILE *fr, *fw;

/**************************************
 *               Stack                *
 **************************************/
typedef struct
{
    void **base; /* 栈底 */
    void **top;  /* 栈顶 */
    int len;
    int size; /* 栈的大小 */
} Stack;

int stack_init(Stack *stack, int size);

int stack_push(Stack *stack, void *node);

void *stack_top(Stack *stack);

void stack_pop(Stack *stack);

void stack_free(Stack *stack);
/**************************************
 *               Token                *
 **************************************/
#define MAX_LEN 128
extern int row_;
extern char token_text_[MAX_LEN];
typedef enum TokenType
{
    // 错误码
    ERROR_TOKEN = 1,
    // 标识符
    IDENT,
    // 整形常量
    INT_CONST,
    // 长整型常量
    LONG_CONST,
    // 浮点型常量
    FLOAT_CONST,
    // 双精度浮点型常量
    DOUBLE_CONST,
    // 字符型常量
    CHAR_CONST,
    // 字符串常量
    STRING_CONST,
    // 关键字
    KEYWORD,
    // 1 关键字 int
    INT,
    // 2 关键字 float
    FLOAT,
    // 3 关键字 char
    CHAR,
    // 4 关键字 long
    LONG,
    // 5 关键字 short
    SHORT,
    // 6 关键字 double
    DOUBLE,
    // 7 关键字 if
    IF,
    // 8 关键字 else
    ELSE,
    // 9 关键字 do
    DO,
    // 10关键字 while
    WHILE,
    // 11关键字 for
    FOR,
    // 13关键字 break
    BREAK,
    // 14关键字 return
    RETURN,
    // 15关键字 continue
    CONTINUE,
    // 16关键字 void
    VOID,
    // 符号 {
    LB,
    // 符号 }
    RB,
    // 符号 [
    LM,
    // 符号 ]
    RM,
    // 符号 ;
    SEMI,
    // 符号 ,
    COMMA,
    /*运算符，必须连在一起*/
    // 1 运算符 ==
    EQ,
    // 2 运算符 !=
    NEQ,
    // 3 运算符 =
    ASSIGN,
    // 4 运算符 (
    LP,
    // 5 运算符 )
    RP,
    // 6 运算符 *
    TIMES,
    // 7 运算符 /
    DIVIDE,
    // 8 运算符 +
    PLUS,
    // 9运算符 #
    POUND,
    // 10运算符 >
    MORE,
    // 11运算符 <
    LESS,
    // 12运算符 >=
    MOREEQ,
    // 13运算符 <=
    LESSEQ,
    // 14运算符 -
    MINUS,
    // 15运算符&&
    AND,
    // 16运算符||
    OR,
    // 注释
    ANNO,
    // 头文件
    INCLUDE,
    // 宏
    DEFINE,
    // 数组
    ARRAY,
} TokenType;

int isNum(char c);

int isAlpha(char c);

int isLetterOrNumOr_(char c);

void Warning(char *s);

void clearToken();

int getToken(FILE *);

/**************************************
 *                Lex                 *
 **************************************/
int lexicalAnalyse();
/**************************************
 *           Symbol Table             *
 **************************************/
#define VAR 0
#define FUNC 1
typedef struct Note
{
    int row;
    char data[128];
} Note;

typedef struct IdentNode
{
    int level;
    int type;
    char name[MAX_LEN];
    struct IdentNode *next;
} IdentNode;

extern IdentNode *symbol_table_;

extern int level;

void tableInit();

int insertIdent(char *token_text, int ident_type);

int checkIdent(char *token_text, int ident_type);

void removeIdent();

/**************************************
 *               Parser               *
 **************************************/
extern int note_num, include_num, define_num;
extern Note notes[128];
extern char include[128][128];
extern char define[128][128];
extern int haveMistake;
typedef struct ASTTree
{
    struct ASTTree *l;
    struct ASTTree *r;
    int type;
    struct data
    {
        int type;
        char *data;
    } data;
} ASTTree;

typedef enum DataType
{
    //外部定义序列
    EXTDEFLIST = 1,
    //外部变量定义
    EXTVARDEF,
    //外部变量类型
    EXTVARTYPE,
    //外部变量名序列
    EXTVARLIST,
    //外部变量名
    EXTVAR,
    //函数定义
    FUNCDEF,
    //函数返回值类型
    FUNCRETURNTYPE,
    //函数名
    FUNCNAME,
    //函数形式参数序列
    FUNCFORMALPARALIST,
    //函数形式参数
    FUNCFORMALPARADEF,
    //函数形参类型
    FUNCFORMALPARATYPE,
    //函数形参名
    FUNCFORMALPARA,
    //函数调用
    FUNCCALL,
    //实参序列
    ACTUALPARALIST,
    //实参
    ACTUALPAR,
    //复合语句
    COMPSTATE,
    //局部变量定义序列
    LOCALVARDEFLIST,
    //局部变量定义
    LOCALVARDEF,
    //局部变量类型
    LOCALVARTYPE,
    //局部变量名序列
    LOCALVARNAMELIST,
    //局部变量名
    LOCALVARNAME,
    //语句序列
    STATELIST,
    //操作数
    OPERAND,
    //运算符
    OPERATOR,
    //表达式
    EXPRESSION,
    // if语句部分
    IFPART,
    // else部分
    ELSEPART,
    // if语句25
    IFSTATEMENT,
    // if-else语句
    IFELSESTATEMENT,
    // while语句结点
    WHILESTATEMENT,
    // while条件语句结点
    WHILEPART,
    // for语句结点30
    FORSTATEMENT,
    // for条件语句
    FORPART,
    // for语句一
    FORPART1,
    // for语句二
    FORPART2,
    // for语句三
    FORPART3,
    // for语句体
    FORBODY,
    // return语句
    RETURNSTATEMENT,
    // break语句
    BREAKSTATEMENT,
    // do-while循环语句
    DOWHILESTATEMENT,
    // do-while条件
    DOWHILECONDITION,
    // continue语句
    CONTINUESTATEMENT,
    //函数声明
    FUNCCLAIM
} DataType;

void clear_arr();

void syntaxAnalyse();

void freeTree(ASTTree *root);

ASTTree *program();

ASTTree *ExtDefList();

ASTTree *ExtDef();

ASTTree *ExtVarDef();

ASTTree *FuncDef();

ASTTree *FormParaList();

ASTTree *FormParaDef();

ASTTree *CompState();

ASTTree *LocalVarDefList();

ASTTree *StateList();

ASTTree *Statement();

ASTTree *Expression(int end);

ASTTree *FuncCall();

ASTTree *ActualParaList();

ASTTree *ActualPara();

char Precede(int c1, int c2);

void returnToken(FILE *fp);

void showType(int cur_type);

void preorderTranverse(ASTTree *root, int depth, int isend);

/**************************************
 *               Format               *
 **************************************/
void formatCode();

void TABs(int d);

void formatTree(ASTTree *T, int depth);

#endif // AST_HEADER_H
