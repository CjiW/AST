#include "format.h"
extern FILE *fr;
extern char token_text_[128];
extern FILE *fw;
extern int haveMistake;
void formatCode(){
    int w;
    w = getToken(fr);
    while (w == ANNO || w == INCLUDE||w==DEFINE) {
        fprintf(fw, "%s", token_text_);
        fprintf(fw,"\n");
        w = getToken(fr);
    }
    returnToken(fr);
    ASTTree* root = program();
    if (root == NULL || haveMistake == 1) {
        printf("Syntax Error!\n");
        return;
    } else {
        formatTree(root, 0);
    }
}

void TABs(int d){
    for (int i = 0; i < d; i++) {
        fprintf(fw,"    ");
    }
}
void formatTree(ASTTree*T, int depth){

    switch (T->type) {
        //     EXTDEFLIST = 1,      外部定义序列
        case EXTDEFLIST:
            if(T->l)formatTree(T->l,depth);
            if(T->r)formatTree(T->r,depth);
            break;
        //     EXTVARDEF,           外部变量定义
        case EXTVARDEF:
            formatTree(T->l,depth);
            formatTree(T->r,depth);
            break;
        //     EXTVARTYPE,          外部变量类型
        case EXTVARTYPE:
            fprintf(fw,"%s",T->data.data);
            break;
        //     EXTVARLIST,          外部变量名序列
        case EXTVARLIST:
            formatTree(T->l,depth);
            if(T->r)formatTree(T->r,depth);
            if(!T->r){
                fseek(fw,-1,SEEK_CUR);
                fprintf(fw,";\n");
            }
            break;
        //     EXTVAR,              外部变量名
        case EXTVAR:
            fprintf(fw," %s,",T->data.data);
            break;
        //     FUNCDEF,             函数定义
        case FUNCDEF:
            formatTree(T->l,depth);
            if(T->r)formatTree(T->r,depth);
            else fprintf(fw,";\n");
            break;

        //     FUNCRETURNTYPE,      函数返回值类型
        case FUNCRETURNTYPE:
            fprintf(fw,"%s ",T->data.data);
            break;
//     FUNCNAME,            函数名
        case FUNCNAME:
            fprintf(fw,"%s",T->data.data);
            fprintf(fw,"(");
            if(T->l)formatTree(T->l,depth);
            fprintf(fw,")");
            if(T->r)formatTree(T->r,depth);
            break;
//     FUNCFORMALPARALIST,  函数形式参数序列
        case FUNCFORMALPARALIST:
            formatTree(T->l,depth);
            if(T->r)formatTree(T->r,depth);
            if(!T->r)fseek(fw,-1,SEEK_CUR);
            break;
//     FUNCFORMALPARADEF,   函数形式参数
        case FUNCFORMALPARADEF:
            formatTree(T->l,depth);
            if(T->r)formatTree(T->r,depth);
            break;
//     FUNCFORMALPARATYPE,  函数形参类型
        case FUNCFORMALPARATYPE:
            fprintf(fw,"%s ",T->data.data);
            break;
//     FUNCFORMALPARA,      函数形参名
        case FUNCFORMALPARA:
            fprintf(fw,"%s,",T->data.data);
            break;
//     FUNCBODY,            函数体
        case FUNCBODY:
            fprintf(fw,"{\n");
            if(T->l)formatTree(T->l,depth+1);
            if(T->r)formatTree(T->r,depth+1);
            fprintf(fw,"}\n");
            break;
//     LOCALVARDEFLIST,     局部变量定义序列
        case LOCALVARDEFLIST:
            formatTree(T->l,depth);
            if(T->r)formatTree(T->r,depth);
            break;
//     LOCALVARDEF,         局部变量定义
        case LOCALVARDEF:
            TABs(depth);
            formatTree(T->l,depth);
            formatTree(T->r,depth);
            break;
//     LOCALVARTYPE,        局部变量类型
        case LOCALVARTYPE:
            fprintf(fw,"%s",T->data.data);
            break;
//     LOCALVARNAMELIST,    局部变量名序列
        case LOCALVARNAMELIST:
            formatTree(T->l,depth);
            if(T->r)formatTree(T->r,depth);
            else {
                fseek(fw,-1,SEEK_CUR);
                fprintf(fw,";\n");
            }
            break;
//     LOCALVARNAME,        局部变量名
        case LOCALVARNAME:
            fprintf(fw," %s,",T->data.data);
            break;
//     STATELIST,           语句序列
        case STATELIST:
            formatTree(T->l,depth);
            if(T->r){
                formatTree(T->r,depth);
            }
            break;
//     OPERAND,             操作数
        case OPERAND:
            fprintf(fw,"%s",T->data.data);
            break;
//     OPERATOR,            运算符
        case OPERATOR:
            if(T->l)formatTree(T->l,depth);
            if(T->data.data)fprintf(fw," %s ",T->data.data);
            if(T->r)formatTree(T->r,depth);
            break;
//     EXPRESSION,          表达式
        case EXPRESSION:
            TABs(depth);
            if(T->l)formatTree(T->l,depth);
            fprintf(fw,";\n");
            break;
//     IFPART,              if语句部分
        case IFPART:
            fprintf(fw,"if");
            fprintf(fw,"(");
            formatTree(T->l, 0);
            fseek(fw,-3,SEEK_CUR);
            fprintf(fw,"){\n");
            formatTree(T->r,depth+1);
            TABs(depth);
            fprintf(fw,"}\n");
            break;
//     ELSEPART,            else部分
        case ELSEPART:
            fseek(fw,-2,SEEK_CUR);
            fprintf(fw,"else{\n");
            formatTree(T->l,depth+1);
            TABs(depth);
            fprintf(fw,"}\n");
            break;
//     IFSTATEMENT,         if语句
        case IFSTATEMENT:
            TABs(depth);
            formatTree(T->l, depth);
            break;
//     IFELSESTATEMENT,     if-else语句
        case IFELSESTATEMENT:
            TABs(depth);
            formatTree(T->l,depth);
            formatTree(T->r,depth);
            break;
//     WHILESTATEMENT,      // while语句
//     WHILEPART,           // while条件语句
//     WHILEBODY,           // while语句体
//     FORSTATEMENT,        // for语句
//     FORPART,             // for条件语句
//     FORPART1,            // for语句一
//     FORPART2,            // for语句二
//     FORPART3,            // for语句三
//     FORBODY,             // for语句体
//     RETURNSTATEMENT,     // return语句
        case RETURNSTATEMENT:
            TABs(depth);
            fprintf(fw,"return ");
            if(T->r)formatTree(T->r,0);
            break;
//     BREAKSTATEMENT,      // break语句
        case BREAKSTATEMENT:
            TABs(depth);
            printf("break;\n");
            break;
//     DOWHILESTATEMENT,    // do-while循环语句
//     DOWHILEBODY,         // do-while语句体
//     DOWHILECONDITION,    // do-while条件
//     CONTINUESTATEMENT,   // continue语句
        case CONTINUESTATEMENT:
            TABs(depth);
            printf("continue;\n");
            break;
//     FUNCCLAIM,           //函数声明
//     ARRAYDEF,            //数组定义
//     ARRAYTYPE,           //数组类型
//     ARRAYNAME,           //数组名45
//     ARRAYSIZE            //数组大小
        default:
            break;
    }
}