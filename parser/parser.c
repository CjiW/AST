#include "header.h"

int w, type;
int haveMistake;
int isVoid, hasReturn, isInRecycle = 0;
int note_num=0, include_num=0, define_num=0;
Note notes[128];
char include[128][128];
char define[128][128];

ASTTree *newNode(int node_type) {
    ASTTree *node = (ASTTree *) malloc(sizeof(ASTTree));
    memset(node, 0, sizeof(ASTTree));
    node->type = node_type;
    return node;
}
void init(){
    for (int i = 0; i < 128; i++) {
        notes[i].data[0] = '\0';
        notes[i].row = 0;
        include[i][0] = '\0';
        define[i][0] = '\0';
    }
    include_num = 0;
    define_num = 0;
    
}
void syntaxAnalyse() {
    init();
    ASTTree *root = program();
    if (haveMistake == 1) {
        Warning("Syntax Error!");
    } else {
        preorderTranverse(root, 0);
        printf("Included files:\n");
        for (int i = 0; i < include_num; i++) {
            printf("    %s\n", include[i]);
        }
        printf("Defined macros:\n");
        for (int i = 0; i < define_num; i++) {
            printf("    %s\n", define[i]);
        }
        printf("Notes:\n");
        for (int i = 0; i < note_num; i++) {
            printf("    [%d] %s\n", notes[i].row, notes[i].data);
        }
    }
}

void freeTree(ASTTree *root) {
    if (root) {
        freeTree(root->l);
        freeTree(root->r);
        free(root);
    }
}

ASTTree *program() {
    row_ = 1;
    haveMistake = 0;
    tableInit();
    clearToken();
    w = getToken(fr);
    while (w == ANNO || w == INCLUDE || w == DEFINE) {
        switch (w) {
            case ANNO:
                strcpy(notes[note_num++].data, token_text_);
                notes->row = row_;
                break;
            case INCLUDE:
                strcpy(include[include_num++], token_text_);
                break;
            case DEFINE:
                strcpy(define[define_num++], token_text_);
                break;
        }
        w = getToken(fr);
    }
    ASTTree *p = ExtDefList();
    if (haveMistake == 1) {
        return NULL;
    }
    if (p != NULL) {
        if (isVoid == 0 && hasReturn == 0) {
            Warning("Missing return value");
            haveMistake = 1;
            return NULL;
        }
        ASTTree *root = p;
        root->type = EXTDEFLIST;
        return root;
    } else {
        haveMistake = 1;
        return NULL;
    }
}

ASTTree *ExtDefList() {
    if (w == -1) {
        return NULL;
    }
    ASTTree *root = newNode(EXTDEFLIST);
    root->l = ExtDef();
    if (haveMistake == 1)return NULL;
    w = getToken(fr);
    while (w == ANNO || w == INCLUDE || w == DEFINE) {
        switch (w) {
            case ANNO:
                strcpy(notes[note_num++].data, token_text_);
                notes->row = row_;
                break;
            case INCLUDE:
                strcpy(include[include_num++], token_text_);
                break;
            case DEFINE:
                strcpy(define[define_num++], token_text_);
                break;
        }
        w = getToken(fr);
    }
    root->r = ExtDefList();
    if (haveMistake == 1)return NULL;
    return root;
}

ASTTree *ExtDef() {
    if (w != INT && w != DOUBLE && w != CHAR && w != LONG && w != SHORT && w != FLOAT && w != VOID) {
        Warning("Expected: type");
        haveMistake = 1;
        return NULL;
    }
    type = w;
    w = getToken(fr);
    while (w == ANNO || w == INCLUDE || w == DEFINE) {
        switch (w) {
            case ANNO:
                strcpy(notes[note_num++].data, token_text_);
                notes->row = row_;
                break;
            case INCLUDE:
                strcpy(include[include_num++], token_text_);
                break;
            case DEFINE:
                strcpy(define[define_num++], token_text_);
                break;
        }
        w = getToken(fr);
    }
    if (w != IDENT && w != ARRAY) {
        Warning("Expected: ident(var Or func)");
        haveMistake = 1;
        return NULL;
    }
    char token_text0[20];
    strcpy(token_text0, token_text_);
    ASTTree *p;
    w = getToken(fr);
    while (w == ANNO || w == INCLUDE || w == DEFINE) {
        switch (w) {
            case ANNO:
                strcpy(notes[note_num++].data, token_text_);
                notes->row = row_;
                break;
            case INCLUDE:
                strcpy(include[include_num++], token_text_);
                break;
            case DEFINE:
                strcpy(define[define_num++], token_text_);
                break;
        }
        w = getToken(fr);
    }
    strcpy(token_text_, token_text0);
    if (w == LP) {
        p = FuncDef();
        if (haveMistake == 1) {
            return NULL;
        }
    } else {
        p = ExtVarDef();
        if (haveMistake == 1) {
            return NULL;
        }
    }
    return p;
}

ASTTree *ExtVarDef() {
    if (haveMistake == 1) {
        return NULL;
    }
    int cnt;
    if (type == VOID) {
        Warning("Can't declare a void type var");
        haveMistake = 1;
        return NULL;
    }

    haveMistake = insertIdent(token_text_, VAR);
    if (haveMistake == 1) {
        return NULL;
    }

    ASTTree *root = newNode(EXTVARDEF);

    ASTTree *p = newNode(EXTVARTYPE);
    p->data.type = type;

    switch (type) {
        case INT:
            p->data.data = "int";
            break;
        case DOUBLE:
            p->data.data = "double";
            break;
        case CHAR:
            p->data.data = "char";
            break;
        case FLOAT:
            p->data.data = "float";
            break;
        case LONG:
            p->data.data = "long";
            break;
        case SHORT:
            p->data.data = "short";
            break;
    }

    root->l = p;
    p = newNode(EXTVARLIST);
    root->r = p;

    p->l = newNode(EXTVAR);
    char *token_text1 = (char *) malloc(MAX_LEN * sizeof(char));  //@
    strcpy(token_text1, token_text_);
    p->l->data.data = token_text1;

    while (1) {
        if (w != COMMA && w != SEMI) {
            if (row_ > cnt) {
                row_--;
            }
            Warning("Expected: ',' or ';'");
            haveMistake = 1;
            return NULL;
        }
        if (w == SEMI) {
            return root;
        }
        w = getToken(fr);
        if (w != IDENT && w != ARRAY) {
            Warning("Expected: ident");
            haveMistake = 1;
            return NULL;
        }

        haveMistake = insertIdent(token_text_, VAR);
        if (haveMistake == 1) {
            return NULL;
        }

        ASTTree *q = newNode(EXTVARLIST);
        p->r = q;
        p = q;

        p->l = newNode(EXTVAR);
        token_text1 = (char *) malloc(MAX_LEN * sizeof(char));
        strcpy(token_text1, token_text_);
        p->l->data.data = token_text1;
        cnt = row_;
        w = getToken(fr);

        while (w == ANNO || w == INCLUDE || w == DEFINE) {
            switch (w) {
                case ANNO:
                    strcpy(notes[note_num++].data, token_text_);
                    notes->row = row_;
                    break;
                case INCLUDE:
                    strcpy(include[include_num++], token_text_);
                    break;
                case DEFINE:
                    strcpy(define[define_num++], token_text_);
                    break;
            }
            w = getToken(fr);
        }
    }
}

ASTTree *FuncDef() {
    if (haveMistake == 1) {
        return NULL;
    }
    ASTTree *root = newNode(FUNCDEF);
    ASTTree *p = newNode(FUNCRETURNTYPE);
    p->data.type = type;

    haveMistake = insertIdent(token_text_, FUNC);
    if (haveMistake == 1) {
        return NULL;
    }

    switch (type) {
        case INT:
            p->data.data = "int";
            isVoid = 0;
            break;
        case DOUBLE:
            p->data.data = "double";
            isVoid = 0;
            break;
        case CHAR:
            p->data.data = "char";
            isVoid = 0;
            break;
        case FLOAT:
            p->data.data = "float";
            isVoid = 0;
            break;
        case LONG:
            p->data.data = "long";
            isVoid = 0;
            break;
        case SHORT:
            p->data.data = "short";
            isVoid = 0;
            break;
        case VOID:
            p->data.data = "void";
            isVoid = 1;
            break;
    }
    p->l = NULL;
    p->r = NULL;
    root->l = p;

    // param
    ASTTree *q = newNode(FUNCNAME);
    char *token_text1 = (char *) malloc(MAX_LEN * sizeof(char));
    strcpy(token_text1, token_text_);
    q->data.data = token_text1;

    root->r = q;
    level++;
    q->l = FormParaList();
    if (haveMistake == 1)return NULL;
    w = getToken(fr);
    while (w == ANNO || w == INCLUDE || w == DEFINE) {
        switch (w) {
            case ANNO:
                strcpy(notes[note_num++].data, token_text_);
                notes->row = row_;
                break;
            case INCLUDE:
                strcpy(include[include_num++], token_text_);
                break;
            case DEFINE:
                strcpy(define[define_num++], token_text_);
                break;
        }
        w = getToken(fr);
    }
    if (w == SEMI) {
        // prototype declare
        root->r->r = NULL;
        root->type = FUNCCLAIM;
        removeIdent();
        level--;
    } else if (w == LB) {
        q->r = CompState();
        if (haveMistake == 1)return NULL;
        removeIdent();
        level--;
    } else {
        Warning("Expected: ';' or '{'");
        haveMistake = 1;
        return NULL;
    }
    return root;
}

ASTTree *FormParaList() {
    if (haveMistake == 1) {
        return NULL;
    }
    w = getToken(fr);
    while (w == ANNO || w == INCLUDE || w == DEFINE) {
        switch (w) {
            case ANNO:
                strcpy(notes[note_num++].data, token_text_);
                notes->row = row_;
                break;
            case INCLUDE:
                strcpy(include[include_num++], token_text_);
                break;
            case DEFINE:
                strcpy(define[define_num++], token_text_);
                break;
        }
        w = getToken(fr);
    }
    if (w == RP) {
        return NULL;
    }
    if (w == COMMA) {
        w = getToken(fr);
        while (w == ANNO || w == INCLUDE || w == DEFINE) {
            switch (w) {
                case ANNO:
                    strcpy(notes[note_num++].data, token_text_);
                    notes->row = row_;
                    break;
                case INCLUDE:
                    strcpy(include[include_num++], token_text_);
                    break;
                case DEFINE:
                    strcpy(define[define_num++], token_text_);
                    break;
            }
            w = getToken(fr);
        }
    }
    ASTTree *root = newNode(FUNCFORMALPARALIST);
    root->l = FormParaDef();
    if (haveMistake == 1)return NULL;
    root->r = FormParaList();
    if (haveMistake == 1)return NULL;
    return root;
}

ASTTree *FormParaDef() {
    if (haveMistake == 1) {
        return NULL;
    }
    if (w != INT && w != DOUBLE && w != CHAR && w != LONG && w != SHORT &&
        w != FLOAT) {
        Warning("Expected: parameter's type");
        haveMistake = 1;
        return NULL;
    }
    type = w;
    w = getToken(fr);
    while (w == ANNO || w == INCLUDE || w == DEFINE) {
        switch (w) {
            case ANNO:
                strcpy(notes[note_num++].data, token_text_);
                notes->row = row_;
                break;
            case INCLUDE:
                strcpy(include[include_num++], token_text_);
                break;
            case DEFINE:
                strcpy(define[define_num++], token_text_);
                break;
        }
        w = getToken(fr);
    }
    if (w != IDENT) {
        Warning("Expected: parameter's name");
        haveMistake = 1;
        return NULL;
    }
    ASTTree *root = newNode(FUNCFORMALPARADEF);
    ASTTree *p = newNode(FUNCFORMALPARATYPE);
    p->data.type = type;
    switch (type) {
        case INT:
            p->data.data = "int";
            break;
        case DOUBLE:
            p->data.data = "double";
            break;
        case CHAR:
            p->data.data = "char";
            break;
        case FLOAT:
            p->data.data = "float";
            break;
        case LONG:
            p->data.data = "long";
            break;
        case SHORT:
            p->data.data = "short";
            break;
        case VOID:
            p->data.data = "void";
            break;
    }
    p->l = p->r = NULL;
    root->l = p;
    p = newNode(FUNCFORMALPARA);

    haveMistake = insertIdent(token_text_, VAR);
    if (haveMistake == 1) {
        return NULL;
    }

    char *token_text1 = (char *) malloc(MAX_LEN * sizeof(char));
    strcpy(token_text1, token_text_);
    p->data.data = token_text1;
    root->r = p;
    return root;
}

ASTTree *CompState() {
    ASTTree *root = newNode(COMPSTATE);
    w = getToken(fr);
    while (w == ANNO || w == INCLUDE || w == DEFINE) {
        switch (w) {
            case ANNO:
                strcpy(notes[note_num++].data, token_text_);
                notes->row = row_;
                break;
            case INCLUDE:
                strcpy(include[include_num++], token_text_);
                break;
            case DEFINE:
                strcpy(define[define_num++], token_text_);
                break;
        }
        w = getToken(fr);
    }
    if (w == INT || w == DOUBLE || w == CHAR || w == LONG || w == SHORT ||
        w == FLOAT) {
        root->l = LocalVarDefList();
        if (haveMistake == 1)return NULL;
    } else {
        root->l = NULL;
    }
    root->r = StateList();
    if (haveMistake == 1)return NULL;
    if (w == RB) {
        return root;
    } else {
        Warning("Excepted: '}'");
        haveMistake = 1;
        freeTree(root);
        return NULL;
    }
}

ASTTree *LocalVarDefList() {
    if (haveMistake == 1) {
        return NULL;
    }

    ASTTree *root = newNode(LOCALVARDEFLIST);
    ASTTree *p = newNode(LOCALVARDEF);
    root->l = p;
    p->l = newNode(LOCALVARTYPE);
    char *tmp_text = (char *) malloc(MAX_LEN * sizeof(char));
    strcpy(tmp_text, token_text_);
    p->l->data.data = tmp_text;

    w = getToken(fr);
    while (w == ANNO || w == INCLUDE || w == DEFINE) {
        switch (w) {
            case ANNO:
                strcpy(notes[note_num++].data, token_text_);
                notes->row = row_;
                break;
            case INCLUDE:
                strcpy(include[include_num++], token_text_);
                break;
            case DEFINE:
                strcpy(define[define_num++], token_text_);
                break;
        }
        w = getToken(fr);
    }
    ASTTree *q = newNode(LOCALVARNAMELIST);

    p->r = q;
    q->l = newNode(LOCALVARNAME);
    char *token_text2 = (char *) malloc(MAX_LEN * sizeof(char));
    strcpy(token_text2, token_text_);
    q->l->data.data = token_text2;

    haveMistake = insertIdent(token_text_, VAR);
    if (haveMistake == 1) {
        return NULL;
    }

    while (1) {
        w = getToken(fr);
        while (w == ANNO || w == INCLUDE || w == DEFINE) {
            switch (w) {
                case ANNO:
                    strcpy(notes[note_num++].data, token_text_);
                    notes->row = row_;
                    break;
                case INCLUDE:
                    strcpy(include[include_num++], token_text_);
                    break;
                case DEFINE:
                    strcpy(define[define_num++], token_text_);
                    break;
            }
            w = getToken(fr);
        }
        if (w == SEMI) {
            q->r = NULL;
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            break;
        } else if (w == COMMA) {
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            ASTTree *s = newNode(LOCALVARNAMELIST);
            q->r = s;
            q = q->r;
            q->l = newNode(LOCALVARNAME);
            tmp_text = (char *) malloc(MAX_LEN * sizeof(char));
            strcpy(tmp_text, token_text_);
            q->l->data.data = tmp_text;
            haveMistake = insertIdent(token_text_, VAR);
            if (haveMistake == 1) {
                freeTree(root);
                return NULL;
            }
        } else {
            Warning("Excepted: ',' or ';'");
            haveMistake = 1;
            return NULL;
        }
    }

    if (w == INT || w == DOUBLE || w == CHAR || w == LONG || w == SHORT ||
        w == FLOAT) {
        root->r = LocalVarDefList();
        if (haveMistake == 1) {
            return NULL;
        }
    } else {
        root->r = NULL;
    }
    root->r = NULL;
    return root;
}

ASTTree *StateList() {
    if (haveMistake == 1) {
        return NULL;
    }
    ASTTree *root = NULL;
    ASTTree *r1 = Statement();
    if (haveMistake == 1)return NULL;
    if (r1 == NULL) {
        return NULL;
    } else {
        root = newNode(STATELIST);
        root->l = r1;
        w = getToken(fr);
        while (w == ANNO || w == INCLUDE || w == DEFINE) {
            switch (w) {
                case ANNO:
                    strcpy(notes[note_num++].data, token_text_);
                    notes->row = row_;
                    break;
                case INCLUDE:
                    strcpy(include[include_num++], token_text_);
                    break;
                case DEFINE:
                    strcpy(define[define_num++], token_text_);
                    break;
            }
            w = getToken(fr);
        }
        if (w != RB) {
            root->r = StateList();
            if (haveMistake == 1)return NULL;
            return root;
        } else {
            return root;
        }
    }
}

ASTTree *Statement() {
    if (haveMistake == 1) {
        return NULL;
    }
    ASTTree *root = newNode(0);
    switch (w) {
        case IF: {
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            if (w != LP) {
                Warning("Excepted: '('");
                haveMistake = 1;
                return NULL;
            }

            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            ASTTree *p1 = newNode(IFPART);
            p1->l = Expression(RP);
            if (haveMistake == 1)return NULL;
            if (p1->l == NULL) {
                Warning("in the condition of If-Statement");
                haveMistake = 1;
                return NULL;
            }
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            level++;
            if (w == LB) {
                p1->r = CompState();
                if (haveMistake == 1)return NULL;
            } else {
                p1->r = Statement();
                if (haveMistake == 1)return NULL;
                p1->r->r = NULL;
            }
            removeIdent();
            level--;
            root->l = p1;
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            if (w == ELSE) {
                root->type = IFELSESTATEMENT;
                ASTTree *p2 = newNode(ELSEPART);
                root->r = p2;
                w = getToken(fr);
                while (w == ANNO || w == INCLUDE || w == DEFINE) {
                    switch (w) {
                        case ANNO:
                            strcpy(notes[note_num++].data, token_text_);
                            notes->row = row_;
                            break;
                        case INCLUDE:
                            strcpy(include[include_num++], token_text_);
                            break;
                        case DEFINE:
                            strcpy(define[define_num++], token_text_);
                            break;
                    }
                    w = getToken(fr);
                }
                level++;
                if (w == LB) {
                    p2->r = CompState();
                    if (haveMistake == 1)return NULL;
                } else {
                    p2->r = Statement();
                    if (haveMistake == 1)return NULL;
                    p2->r->r = NULL;
                }
                removeIdent();
                level--;
            } else {
                root->type = IFSTATEMENT;
                returnToken(fr);
            }
            return root;
        }
        case WHILE: {
            isInRecycle++;
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            if (w != LP) {
                Warning("Error in WHILE");
                haveMistake = 1;
                return NULL;
            }

            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            ASTTree *p1 = newNode(WHILEPART);
            p1->l = Expression(RP);
            if (haveMistake == 1)return NULL;
            if (p1->l == NULL) {
                Warning("whileStatementCondition部分出错");
                haveMistake = 1;
                return NULL;
            }
            ASTTree *p2;
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            level++;
            if (w == LB) {
                p2 = CompState();
            } else {
                p2 = Statement();
            }
            removeIdent();
            level--;
            root->type = WHILESTATEMENT;
            root->l = p1;
            root->r = p2;
            isInRecycle--;
            return root;
        }
        case FOR: {
            isInRecycle++;
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            if (w != LP) {
                Warning("Error in FOR");
                haveMistake = 1;
                return NULL;
            }
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            ASTTree *p1 = newNode(FORPART);
            ASTTree *q = newNode(FORPART1);  // FOR part 1
            p1->l = q;
            q->l = Expression(SEMI);
            if (haveMistake == 1)return NULL;
            if (q->l == NULL) {
                q->data.data = "None";
            }
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            q->r = newNode(FORPART2);  // FOR part 2
            q = q->r;
            q->l = Expression(SEMI);
            if (haveMistake == 1)return NULL;
            if (q->l == NULL) {
                q->data.data = "None";
            }
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            q->r = newNode(FORPART3);  // FOR part 3
            q = q->r;
            q->l = Expression(RP);
            if (haveMistake == 1)return NULL;
            if (q->l == NULL) {
                q->data.data = "None";
            }

            ASTTree *p2 = newNode(FORBODY);  // FOR body
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            level++;
            if (w == LB) {
                p2->r = CompState();
                if (haveMistake == 1)return NULL;
            } else {
                p2->r = Statement();
                if (haveMistake == 1)return NULL;
                p2->r->r = NULL;
            }
            removeIdent();
            level--;
            root->type = FORSTATEMENT;
            root->l = p1;
            root->r = p2;
            isInRecycle--;
            return root;
        }
        case RETURN: {
            hasReturn = 1;
            if (isVoid == 1) {
                Warning("There should be no return statement\n");
                haveMistake = 1;
                return NULL;
            }
            root->type = RETURNSTATEMENT;
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            root->r = Expression(SEMI);
            if (haveMistake == 1)return NULL;
            return root;
        }
        case DO: {
            isInRecycle++;
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            if (w != LB) {
                Warning("missing bracket in do-while\n");
                haveMistake = 1;
                return NULL;
            }

            ASTTree *p1;

            ASTTree *p2 = newNode(DOWHILECONDITION);
            level++;
            p1 = CompState();
            removeIdent();
            level--;
            root->l = p1;
            root->r = p2;
            if (haveMistake == 1)return NULL;
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            if (w != WHILE) {
                Warning("missing WHILE in do-while");
                freeTree(root);
                haveMistake = 1;
                return NULL;
            }
            root->type = DOWHILESTATEMENT;
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            if (w != LP) {
                Warning("missing condition in do-while");
                freeTree(root);
                haveMistake = 1;
                return NULL;
            }
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            p2->l = Expression(RP);
            if (haveMistake == 1)return NULL;
            if (p2->l == NULL) {
                Warning("missing condition in do-while");
                haveMistake = 1;
                return NULL;
            }
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            if (w != SEMI) {
                Warning("missing semicolon in do-while");
                freeTree(root);
                haveMistake = 1;
                return NULL;
            }
            isInRecycle--;
            return root;
        }
        case BREAK: {
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            if (w != SEMI) {
                Warning("missing semicolon in BREAK");
                haveMistake = 1;
                return NULL;
            }
            if (isInRecycle == 0) {
                Warning("unexpected BREAK");
                haveMistake = 1;
                return NULL;
            }
            root->type = BREAKSTATEMENT;
            return root;
        }
        case CONTINUE: {
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
            if (w != SEMI) {
                Warning("missing semicolon in continue");
                haveMistake = 1;
                return NULL;
            }
            if (isInRecycle == 0) {
                Warning("unexpected continue");
                haveMistake = 1;
                return NULL;
            }
            root->type = CONTINUESTATEMENT;
            return root;
        }
        case INT_CONST:
        case LONG_CONST:
        case DOUBLE_CONST:
        case FLOAT_CONST:
        case CHAR_CONST:
        case IDENT:
        case ARRAY:
            return Expression(SEMI);
    }
    return root;
}

ASTTree *Expression(int end) {
    if (w == end) {
        return NULL;
    }
    int error = 0;
    Stack op;
    stack_init(&op, 1000);
    ASTTree *p = newNode(OPERATOR);
    p->data.type = POUND;
    stack_push(&op, p);
    Stack opn;
    stack_init(&opn, 1000);
    ASTTree *t, *t1, *t2, *root;
    while (((w != end) || (((ASTTree *) stack_top(&op))->data.type != POUND)) && !error) {
        if (((ASTTree *) stack_top(&op))->data.type == RP) {
            if (op.len < 3) {
                error++;
                break;
            }
            stack_pop(&op);
            stack_pop(&op);
        }
        if (w == IDENT) {
            if (checkIdent(token_text_, FUNC) == 1) {
                p = FuncCall();
                stack_push(&opn, p);
                w = getToken(fr);
                while (w == ANNO || w == INCLUDE || w == DEFINE) {
                    switch (w) {
                        case ANNO:
                            strcpy(notes[note_num++].data, token_text_);
                            notes->row = row_;
                            break;
                        case INCLUDE:
                            strcpy(include[include_num++], token_text_);
                            break;
                        case DEFINE:
                            strcpy(define[define_num++], token_text_);
                            break;
                    }
                    w = getToken(fr);
                }
            } else if (checkIdent(token_text_, VAR) == 0) {
                stack_free(&op);
                stack_free(&opn);
                Warning("Using undeclared ident");
                haveMistake = 1;
                return NULL;
            }
        }
        if (w == IDENT || w == INT_CONST || w == LONG_CONST || w == FLOAT_CONST || w == DOUBLE_CONST ||
            w == CHAR_CONST || w == ARRAY || w == STRING_CONST) {
            p = newNode(OPERAND);
            char *token_text1 = (char *) malloc(MAX_LEN * sizeof(char));
            strcpy(token_text1, token_text_);
            p->data.data = token_text1;
            stack_push(&opn, p);
            w = getToken(fr);
            while (w == ANNO || w == INCLUDE || w == DEFINE) {
                switch (w) {
                    case ANNO:
                        strcpy(notes[note_num++].data, token_text_);
                        notes->row = row_;
                        break;
                    case INCLUDE:
                        strcpy(include[include_num++], token_text_);
                        break;
                    case DEFINE:
                        strcpy(define[define_num++], token_text_);
                        break;
                }
                w = getToken(fr);
            }
        } else if (w == end) {
            p = newNode(OPERATOR);
            p->data.type = POUND;
            while (((ASTTree *) stack_top(&op))->data.type != POUND) {
                t2 = ((ASTTree *) stack_top(&opn));
                if (t2 != NULL)
                    stack_pop(&opn);
                if (opn.len == 0) {
                    t1 = NULL;
                } else {
                    t1 = ((ASTTree *) stack_top(&opn));
                }
                if (t1 != NULL)
                    stack_pop(&opn);
                t = ((ASTTree *) stack_top(&op));
                if (!t) {
                    error++;
                    break;
                }
                stack_pop(&op);
                t->l = t1;
                t->r = t2;
                stack_push(&opn, t);
            }
            if (opn.len != 1) {
                error++;
            }

        } else if (w >= EQ && w <= OR) {
            char *token_text1 = (char *) malloc(MAX_LEN * sizeof(char));  //@
            switch (Precede(((ASTTree *) stack_top(&op))->data.type, w)) {
                case '<':
                    p = newNode(OPERATOR);
                    p->data.type = w;
                    strcpy(token_text1, token_text_);
                    p->data.data = token_text1;
                    stack_push(&op, p);
                    w = getToken(fr);
                    while (w == ANNO || w == INCLUDE || w == DEFINE) {
                        switch (w) {
                            case ANNO:
                                strcpy(notes[note_num++].data, token_text_);
                                notes->row = row_;
                                break;
                            case INCLUDE:
                                strcpy(include[include_num++], token_text_);
                                break;
                            case DEFINE:
                                strcpy(define[define_num++], token_text_);
                                break;
                        }
                        w = getToken(fr);
                    }
                    break;
                case '=':
                    t = ((ASTTree *) stack_top(&op));
                    if (!t) {
                        error++;
                        stack_pop(&op);
                    }
                    w = getToken(fr);
                    while (w == ANNO || w == INCLUDE || w == DEFINE) {
                        switch (w) {
                            case ANNO:
                                strcpy(notes[note_num++].data, token_text_);
                                notes->row = row_;
                                break;
                            case INCLUDE:
                                strcpy(include[include_num++], token_text_);
                                break;
                            case DEFINE:
                                strcpy(define[define_num++], token_text_);
                                break;
                        }
                        w = getToken(fr);
                    }
                    break;
                case '>':
                    t2 = ((ASTTree *) stack_top(&opn));
                    if (t2 != NULL)
                        stack_pop(&opn);
                    if (opn.len == 0) {
                        t1 = NULL;
                    } else {
                        t1 = ((ASTTree *) stack_top(&opn));
                    }
                    if (t1 != NULL)
                        stack_pop(&opn);
                    t = ((ASTTree *) stack_top(&op));
                    if (!t) {
                        error++;
                        break;
                    }
                    stack_pop(&op);
                    t->l = t1;
                    t->r = t2;
                    stack_push(&opn, t);

                    p = newNode(OPERATOR);
                    p->data.type = w;
                    strcpy(token_text1, token_text_);
                    p->data.data = token_text1;
                    stack_push(&op, p);

                    w = getToken(fr);
                    while (w == ANNO || w == INCLUDE || w == DEFINE) {
                        switch (w) {
                            case ANNO:
                                strcpy(notes[note_num++].data, token_text_);
                                notes->row = row_;
                                break;
                            case INCLUDE:
                                strcpy(include[include_num++], token_text_);
                                break;
                            case DEFINE:
                                strcpy(define[define_num++], token_text_);
                                break;
                        }
                        w = getToken(fr);
                    }
                    break;
                case '\0':
                    stack_free(&op);
                    stack_free(&opn);
                    Warning("unknown operator");
                    haveMistake = 1;
                    return NULL;
            }
        } else {
            error = 1;
        }
    }
    if ((opn.len == 1) && (((ASTTree *) stack_top(&op))->data.type == POUND) && error == 0) {
        t = ((ASTTree *) stack_top(&opn));
        stack_pop(&opn);
        root = newNode(EXPRESSION);
        root->l = t;
        stack_free(&op);
        stack_free(&opn);
        return root;
    } else {
        stack_free(&op);
        stack_free(&opn);
        Warning("wrong expression");
        haveMistake = 1;
        return NULL;
    }
}

ASTTree *FuncCall() {
    if (haveMistake == 1) {
        return NULL;
    }
    ASTTree *root = newNode(FUNCCALL);
    root->l = newNode(FUNCNAME);
    {
        char *func_name = (char *) malloc(MAX_LEN * sizeof(char));
        strcpy(func_name, token_text_);
        root->l->data.data = func_name;
    }
    int a = getToken(fr);
    if (a != LP) {
        haveMistake = 1;
        Warning("Expected '('");
        return NULL;
    }
    root->r = ActualParaList();
    if (haveMistake == 1) {
        return NULL;
    }
    return root;
}

ASTTree *ActualParaList() {
    if (haveMistake == 1) {
        return NULL;
    }
    w = getToken(fr);
    while (w == ANNO || w == INCLUDE || w == DEFINE) {
        switch (w) {
            case ANNO:
                strcpy(notes[note_num++].data, token_text_);
                notes->row = row_;
                break;
            case INCLUDE:
                strcpy(include[include_num++], token_text_);
                break;
            case DEFINE:
                strcpy(define[define_num++], token_text_);
                break;
        }
        w = getToken(fr);
    }
    if (w == RP) {
        return NULL;
    }
    if (w == COMMA) {
        w = getToken(fr);
        while (w == ANNO || w == INCLUDE || w == DEFINE) {
            switch (w) {
                case ANNO:
                    strcpy(notes[note_num++].data, token_text_);
                    notes->row = row_;
                    break;
                case INCLUDE:
                    strcpy(include[include_num++], token_text_);
                    break;
                case DEFINE:
                    strcpy(define[define_num++], token_text_);
                    break;
            }
            w = getToken(fr);
        }
    }

    ASTTree *root = newNode(ACTUALPARALIST);
    root->l = ActualPara();
    if (haveMistake == 1) {
        return NULL;
    }
    root->r = ActualParaList();
    if (haveMistake == 1) {
        return NULL;
    }
    return root;

}

ASTTree *ActualPara() {
    if (haveMistake == 1) {
        return NULL;
    }
    if (w != IDENT && w != INT_CONST && w != LONG_CONST && w != FLOAT_CONST && w != CHAR_CONST && w != DOUBLE_CONST) {
        haveMistake = 1;
        return NULL;
    }
    ASTTree *root = newNode(ACTUALPAR);
    char *tmp_text = (char *) malloc(MAX_LEN * sizeof(char));
    strcpy(tmp_text, token_text_);
    root->data.data = tmp_text;
    root->data.type = w;
    return root;
}

char Precede(int c1, int c2) {
    if (haveMistake == 1) {
        return -1;
    }
    if (c1 == PLUS || c1 == MINUS) {
        switch (c2) {
            case OR:
            case AND:
            case PLUS:
            case MINUS:
            case RP:
            case POUND:
            case MORE:
            case LESS:
            case MOREEQ:
            case LESSEQ:
            case EQ:
            case NEQ:
            case ASSIGN:
                return '>';
            case TIMES:
            case DIVIDE:
            case LP:
                return '<';
            default:
                return '\0';
        }
    } else if (c1 == TIMES || c1 == DIVIDE) {
        switch (c2) {
            case OR:
            case AND:
            case PLUS:
            case MINUS:
            case RP:
            case POUND:
            case TIMES:
            case DIVIDE:
            case MORE:
            case LESS:
            case MOREEQ:
            case LESSEQ:
            case EQ:
            case NEQ:
                return '>';
            case LP:
                return '<';
            default:
                return '\0';
        }
    } else if (c1 == LP) {
        switch (c2) {
            case OR:
            case AND:
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
                return '<';
            case RP:
                return '=';
            case MORE:
            case LESS:
            case MOREEQ:
            case LESSEQ:
            case EQ:
            case NEQ:
            case POUND:
                return '>';
            default:
                return '\0';
        }
    } else if (c1 == RP) {
        switch (c2) {
            case OR:
            case AND:
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQ:
            case LESSEQ:
            case EQ:
            case NEQ:
            case POUND:
                return '>';
            default:
                return '\0';
        }
    } else if (c1 == ASSIGN) {
        switch (c2) {
            case OR:
            case AND:
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQ:
            case LESSEQ:
            case EQ:
            case NEQ:
                return '<';
            case POUND:
                return '>';
            default:
                return '\0';
        }
    } else if (c1 == MORE || c1 == LESS || c1 == MOREEQ || c1 == LESSEQ) {
        switch (c2) {
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
                return '<';
            case OR:
            case AND:
            case RP:
            case MORE:
            case LESS:
            case MOREEQ:
            case LESSEQ:
            case EQ:
            case NEQ:
            case POUND:
                return '>';
            default:
                return '\0';
        }
    } else if (c1 == EQ || c1 == NEQ) {
        switch (c2) {
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQ:
            case LESSEQ:
                return '<';
            case OR:
            case AND:
            case RP:
            case EQ:
            case NEQ:
            case POUND:
                return '>';
            default:
                return '\0';
        }
    } else if (c1 == POUND) {
        switch (c2) {
            case OR:
            case AND:
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQ:
            case LESSEQ:
            case RP:
            case EQ:
            case NEQ:
            case ASSIGN:
                return '<';
            case POUND:
                return '=';
            default:
                return '\0';
        }
    } else if (c1 == OR) {
        switch (c2) {
            case AND:
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQ:
            case LESSEQ:
            case RP:
            case EQ:
            case NEQ:
                return '<';
            case OR:
            case ASSIGN:
            case POUND:
                return '>';
            default:
                return '\0';
        }
    } else if (c1 == AND) {
        switch (c2) {
            case PLUS:
            case MINUS:
            case TIMES:
            case DIVIDE:
            case LP:
            case MORE:
            case LESS:
            case MOREEQ:
            case LESSEQ:
            case RP:
            case EQ:
            case NEQ:
                return '<';
            case OR:
            case AND:
            case ASSIGN:
            case POUND:
                return '>';
            default:
                return '\0';
        }
    }
    return -1;
}

void returnToken(FILE *fp) {
    int len = strlen(token_text_);
    int i;
    for (i = 0; i < len; i++) {
        ungetc(token_text_[len - 1 - i], fp);
    }
}

void showType(int cur_type) {
    switch (cur_type) {
        case EXTDEFLIST:
            printf("ExtDefList\n");
            break;
        case EXTVARDEF:
            printf("ExtVarDef\n");
            break;
        case EXTVARTYPE:
            printf("ExtVarType\n");
            break;
        case EXTVARLIST:
            printf("ExtVarList\n");
            break;
        case EXTVAR:
            printf("ExtVar\n");
            break;
        case FUNCDEF:
            printf("FuncDef\n");
            break;
        case FUNCRETURNTYPE:
            printf("FuncReturnType\n");
            break;
        case FUNCNAME:
            printf("FuncName\n");
            break;
        case FUNCFORMALPARALIST:
            printf("FuncFormalParamList\n");
            break;
        case FUNCFORMALPARADEF:
            printf("FuncFormalParmaDef\n");
            break;
        case FUNCFORMALPARATYPE:
            printf("FuncFormalParmaType\n");
            break;
        case FUNCFORMALPARA:
            printf("FuncFormalPara\n");
            break;
        case FUNCCALL:
            printf("FuncCall\n");
            break;
        case ACTUALPARALIST:
            printf("ActualParaList\n");
            break;
        case ACTUALPAR:
            printf("ActualPara\n");
            break;
        case COMPSTATE:
            printf("CompState\n");
            break;
        case LOCALVARDEFLIST:
            printf("LocalVarDefList\n");
            break;
        case LOCALVARDEF:
            printf("LocalVarDef\n");
            break;
        case LOCALVARTYPE:
            printf("LocalVarType\n");
            break;
        case LOCALVARNAMELIST:
            printf("LocalVarNameList\n");
            break;
        case LOCALVARNAME:
            printf("LocalVarName\n");
            break;
        case STATELIST:
            printf("StateList\n");
            break;
        case OPERAND:
            printf("Operand\n");
            break;
        case OPERATOR:
            printf("Operator\n");
            break;
        case EXPRESSION:
            printf("Expression\n");
            break;
        case IFPART:
            printf("IF-Statement\n");
            break;
        case ELSEPART:
            printf("ELSE-Statement\n");
            break;
        case IFSTATEMENT:
            printf("IF-Statement\n");
            break;
        case IFELSESTATEMENT:
            printf("IF-ELSE-Statement\n");
            break;
        case WHILESTATEMENT:
            printf("WHILE-Statement\n");
            break;
        case WHILEPART:
            printf("WHILE-Condition\n");
            break;
        case FORSTATEMENT:
            printf("FOR-Statement\n");
            break;
        case FORPART:
            printf("FOR-Condition\n");
            break;
        case FORPART1:
            printf("FOR-Part1\n");
            break;
        case FORPART2:
            printf("FOR-Part2\n");
            break;
        case FORPART3:
            printf("FOR-Part3\n");
            break;
        case FORBODY:
            printf("FOR-Body\n");
            break;
        case RETURNSTATEMENT:
            printf("RETURN-Statement\n");
            break;
        case BREAKSTATEMENT:
            printf("BREAK-Statement\n");
            break;
        case DOWHILESTATEMENT:
            printf("DO-WHILE-Statement\n");
            break;
        case DOWHILECONDITION:
            printf("DO-WHILE-Condition\n");
            break;
        case CONTINUESTATEMENT:
            printf("continueStatement\n");
            break;
        case FUNCCLAIM:
            printf("FuncClaim\n");
            break;
        default:
            printf("ErrType %d\n", cur_type);
            break;
    }
}

void preorderTranverse(ASTTree *root, int depth) {
    if (root == NULL);
    else {
        int i;
        for (i = 0; i < depth; i++) {
            printf("   ");
        }
        showType(root->type);
        if (root->data.data != NULL) {
            for (i = 0; i < depth; i++) {
                printf("   ");
            }
            printf("%s\n", root->data.data);
        }
        preorderTranverse(root->l, depth + 1);
        preorderTranverse(root->r, depth + 1);
    }
}
