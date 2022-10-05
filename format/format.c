#include "header.h"
void formatCode() {
    init();
    ASTTree *root = program();
    if (haveMistake == 1) {
        printf("Syntax Error!\n");
        return;
    } else {
        for (int i = 0; i < include_num; i++) {
            fprintf(fw, "%s\n", include[i]);
        }
        for (int i = 0; i < define_num; i++) {
            fprintf(fw, "%s\n", define[i]);
        }
        formatTree(root, 0);
    }
}

void TABs(int d) {
    for (int i = 0; i < d; i++) {
        fprintf(fw, "    ");
    }
}
void formatTree(ASTTree *T, int depth){
    int tmp;
    switch (T->type) {
        // //外部定义序列
        // EXTDEFLIST
        case EXTDEFLIST:
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            break;
        // //外部变量定义
        // EXTVARDEF,
        case EXTVARDEF:
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            fprintf(fw,";\n");
            break;
        // //外部变量类型
        // EXTVARTYPE,
        case EXTVARTYPE:
            fprintf(fw, "%s ", T->data.data);
            break;
        // //外部变量名序列
        // EXTVARLIST,
        case EXTVARLIST:
            if (T->l)formatTree(T->l, depth);
            if (T->r){
                fprintf(fw, ", ");
                formatTree(T->r, depth);
            }
            break;
        // //外部变量名
        // EXTVAR,
        case EXTVAR:
            fprintf(fw, "%s", T->data.data);
            break;
        // //函数定义
        // FUNCDEF,
        case FUNCDEF:
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            break;
        // //函数返回值类型
        // FUNCRETURNTYPE,
        case FUNCRETURNTYPE:
            fprintf(fw, "%s ", T->data.data);
            break;
        // //函数名
        // FUNCNAME,
        case FUNCNAME:
            fprintf(fw, "%s", T->data.data);
            fprintf(fw, "(");
            if (T->l) formatTree(T->l, depth);
            fprintf(fw, ")\n");
            if (T->r){
                formatTree(T->r, depth);
            } else{
                fseek(fw, -1, SEEK_CUR);
            }
            break;
        // //函数形式参数序列
        // FUNCFORMALPARALIST,
        case FUNCFORMALPARALIST:
            if (T->l)formatTree(T->l, depth);
            if (T->r){
                fprintf(fw, ", ");
                formatTree(T->r, depth);
            }
            break;
        // //函数形式参数
        // FUNCFORMALPARADEF,
        case FUNCFORMALPARADEF:
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            break;
        // //函数形参类型
        // FUNCFORMALPARATYPE,
        case FUNCFORMALPARATYPE:
            fprintf(fw, "%s ", T->data.data);
            break;
        // //函数形参名
        // FUNCFORMALPARA,
        case FUNCFORMALPARA:
            fprintf(fw, "%s", T->data.data);
            break;
        // //函数调用
        // FUNCCALL,
        case FUNCCALL:
            if (T->l)formatTree(T->l, depth);
            fseek(fw, -1, SEEK_CUR);
            if (T->r)formatTree(T->r, depth);
            fprintf(fw, ") ");
            break;
        // //实参序列
        // ACTUALPARALIST,
        case ACTUALPARALIST:
            if (T->l)formatTree(T->l, depth);
            if (T->r) {
                fprintf(fw, ", ");
                formatTree(T->r, depth);
            }
            break;
        // //实参
        // ACTUALPAR,
        case ACTUALPAR:
            fprintf(fw, "%s", T->data.data);
            break;
        // //复合语句
        // COMPSTATE,
        case COMPSTATE:
            TABs(depth);
            fprintf(fw, "{\n");
            if(T->l)formatTree(T->l, depth+1);
            if(T->r)formatTree(T->r, depth+1);
            TABs(depth);
            fprintf(fw, "}\n");
            break;
        // //局部变量定义序列
        // LOCALVARDEFLIST,
        case LOCALVARDEFLIST:
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            break;
        // //局部变量定义
        // LOCALVARDEF,
        case LOCALVARDEF:
            TABs(depth);
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            fprintf(fw, ";\n");
            break;
        // //局部变量类型
        // LOCALVARTYPE,
        case LOCALVARTYPE:
            fprintf(fw, "%s ", T->data.data);
            break;
        // //局部变量名序列
        // LOCALVARNAMELIST,
        case LOCALVARNAMELIST:
            if (T->l)formatTree(T->l, depth);
            if (T->r){
                fprintf(fw, ", ");
                formatTree(T->r, depth);
            }
            break;
        // //局部变量名
        // LOCALVARNAME,
        case LOCALVARNAME:
            fprintf(fw, "%s", T->data.data);
            break;
        // //语句序列
        // STATELIST,
        case STATELIST:
            if(T->l)formatTree(T->l, depth);
            if(T->r)formatTree(T->r, depth);
            break;
        // //操作数
        // OPERAND,
        case OPERAND:
            fprintf(fw, "%s ", T->data.data);
            break;
        // //运算符
        // OPERATOR,
        case OPERATOR:
            if (T->l)formatTree(T->l, depth);
            fprintf(fw, "%s ", T->data.data);
            if (T->r)formatTree(T->r, depth);
            break;
        // //表达式
        // EXPRESSION,
        case EXPRESSION:
            TABs(depth);
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            fseek(fw, -1, SEEK_CUR);
            fprintf(fw, ";\n");
            break;
        // // if语句部分
        // IFPART,
        case IFPART:
            TABs(depth);
            fprintf(fw, "if (");
            if(T->l)formatTree(T->l, 0);
            fseek(fw, -2, SEEK_CUR);
            fprintf(fw, ")\n");
            tmp=depth;
            if (T->r->type != COMPSTATE)tmp++;
            formatTree(T->r, tmp);
            break;
        // // else部分
        // ELSEPART,
        case ELSEPART:
            TABs(depth);
            fprintf(fw, "else ");
            tmp = T->r->type;
            if (tmp != IFELSESTATEMENT && tmp != IFPART && tmp != IFSTATEMENT){
                fprintf(fw, "\n");
                formatTree(T->r, depth+1);
            } else{
                formatTree(T->r, 0);
            }
            break;
        // // if语句25
        // IFSTATEMENT,
        case IFSTATEMENT:
            if (T->l)formatTree(T->l, depth);
            break;
        // // if-else语句
        // IFELSESTATEMENT,
        case IFELSESTATEMENT:
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            break;
        // // while语句结点
        // WHILESTATEMENT,
        case WHILESTATEMENT:
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            break;
        // // while条件语句结点
        // WHILEPART,
        case WHILEPART:
            TABs(depth);
            fprintf(fw, "while (");
            if (T->l) {
                formatTree(T->l, 0);
                fseek(fw, -2, SEEK_CUR);
                fprintf(fw, ")\n");
            }
            break;
        // // while语句体
        // WHILEBODY, 未使用，由 复合语句 代替
        // // for语句结点30
        // FORSTATEMENT,
        case FORSTATEMENT:
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            break;
        // // for条件语句
        // FORPART,
        case FORPART:
            TABs(depth);
            fprintf(fw, "for (");
            if (T->l)formatTree(T->l, depth);
            break;
        // // for语句一
        // FORPART1,
        case FORPART1:
            if (T->l)formatTree(T->l, 0);
            fseek(fw, -1, SEEK_CUR);
            fprintf(fw, " ");
            if (T->r)formatTree(T->r, 0);
            break;
        // // for语句二
        // FORPART2,
        case FORPART2:
            if (T->l)formatTree(T->l, 0);
            fseek(fw, -1, SEEK_CUR);
            fprintf(fw, " ");
            if (T->r)formatTree(T->r, 0);
            fseek(fw, -2, SEEK_CUR);
            fprintf(fw, ")\n");
            break;
        // // for语句三
        // FORPART3,
        case FORPART3:
            if (T->l)formatTree(T->l, 0);
            break;
        // // for语句体
        // FORBODY,
        case FORBODY:
            if (T->r)formatTree(T->r, depth);
            break;
        // // return语句
        // RETURNSTATEMENT,
        case RETURNSTATEMENT:
            TABs(depth);
            fprintf(fw, "return ");
            if (T->r)formatTree(T->r, 0);
            break;
        // // break语句
        // BREAKSTATEMENT,
        case BREAKSTATEMENT:
            TABs(depth);
            fprintf(fw, "break;\n");
            break;
        // // do-while循环语句
        // DOWHILESTATEMENT,
        case DOWHILESTATEMENT:
            TABs(depth);
            fprintf(fw, "do\n");
            if (T->l)formatTree(T->l, depth);
            fseek(fw, -1, SEEK_CUR);
            fprintf(fw, " while (");
            if (T->r)formatTree(T->r, depth);
            break;
        // // do-while语句体
        // DOWHILEBODY,
        // // do-while条件
        // DOWHILECONDITION,
        case DOWHILECONDITION:
            if (T->l)formatTree(T->l, 0);
            fseek(fw, -2, SEEK_CUR);
            fprintf(fw, ");\n");
            break;
        // // continue语句
        // CONTINUESTATEMENT,
        case CONTINUESTATEMENT:
            TABs(depth);
            fprintf(fw,"continue;\n");
            break;
        // //函数声明
        // FUNCCLAIM
        case FUNCCLAIM:
            if (T->l)formatTree(T->l, depth);
            if (T->r)formatTree(T->r, depth);
            fprintf(fw, ";\n");
            break;
    }
}
