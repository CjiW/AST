#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LANGUAGE_FORMAT_PROCESSING_TOOL_GETTOKEN_H
#define LANGUAGE_FORMAT_PROCESSING_TOOL_GETTOKEN_H

typedef enum TokenType {
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
    // 12关键字 struct
    STRUCT,
    // 13关键字 break
    BREAK,
    // 14关键字 switch
    SWITCH,
    // 15关键字 case
    CASE,
    // 16关键字 typedef
    TYPEDEF,
    // 17关键字 return
    RETURN,
    // 18关键字 continue
    CONTINUE,
    // 19关键字 void
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
int isLetterOrNum(char c);
int getToken(FILE*);

#endif  // LANGUAGE_FORMAT_PROCESSING_TOOL_GETTOKEN_H
