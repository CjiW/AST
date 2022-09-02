#include "token.h"

char token_text_[128]; //存放单词自身值
int text_idx_ = 0;
int row_ = 1;

int isSpace(char c) {
    return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f');
}

int isNum(char c) { return (c >= '0' && c <= '9'); }

int isXNum(char c) {
    return ((isNum(c)) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

int isAlpha(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int isLetterOrNum(char c) { return isAlpha(c) || isNum(c); }

void clearToken(){
    memset(token_text_, 0, sizeof token_text_);
    text_idx_ = 0;
}

int add2token(char c) {
    /* max len : 128 */
    if (text_idx_ >= 127) {
        printf("Word Too Long!\n");
        return -1; //添加失败
    }
    *(token_text_ + text_idx_) = c;
    *(token_text_ + text_idx_ + 1) = '\0';
    text_idx_++;
    return 1;
}

int getToken(FILE *fp) {
    char c;
    clearToken();
    // eat Space
    do {
        c = fgetc(fp);
        if (c == '\n') {
            row_++;
        }
    } while (isSpace(c));

    // IDENT、ARRAY or KEYWORD
    if (isAlpha(c) || c == '_') {
        do {
            add2token(c);
        } while (c = fgetc(fp), isLetterOrNum(c)||c == '_');
        ungetc(c, fp);
        if (strcmp(token_text_, "int") == 0) { return INT; }
        if (strcmp(token_text_, "double") == 0) { return DOUBLE; }
        if (strcmp(token_text_, "char") == 0) { return CHAR; }
        if (strcmp(token_text_, "short") == 0) { return SHORT; }
        if (strcmp(token_text_, "long") == 0) { return LONG; }
        if (strcmp(token_text_, "float") == 0) { return FLOAT; }
        /* is keyword */
        if (strcmp(token_text_, "if") == 0) { return IF; }
        if (strcmp(token_text_, "else") == 0) { return ELSE; }
        if (strcmp(token_text_, "do") == 0) { return DO; }
        if (strcmp(token_text_, "while") == 0) { return WHILE; }
        if (strcmp(token_text_, "for") == 0) { return FOR; }
        if (strcmp(token_text_, "struct") == 0) { return STRUCT; }
        if (strcmp(token_text_, "break") == 0) { return BREAK; }
        if (strcmp(token_text_, "switch") == 0) { return SWITCH; }
        if (strcmp(token_text_, "case") == 0) { return CASE; }
        if (strcmp(token_text_, "typedef") == 0) { return TYPEDEF; }
        if (strcmp(token_text_, "return") == 0) { return RETURN; }
        if (strcmp(token_text_, "continue") == 0) { return CONTINUE; }
        if (strcmp(token_text_, "void") == 0) { return VOID; }
        c = fgetc(fp);

        if (c == '[') {
            // ARRAY
            add2token(c);
            c = fgetc(fp);
            while (isNum(c)) {
                add2token(c);
                c = fgetc(fp);
            }
            if (c != ']') {
                return ERROR_TOKEN;
            }
            add2token(c);
            return ARRAY;
        } else {
            ungetc(c, fp);
        }
        return IDENT;
    }

    // number
    if (isNum(c)) {
        if (c == '0') {
            add2token(c);
            c = fgetc(fp);
            // 十六进制
            if (c == 'x' || c == 'X') {
                do {
                    add2token(c);
                } while (c = fgetc(fp), isXNum(c));
                if (c == 'L' || c == 'l') {
                    add2token(c);
                    return LONG_CONST;
                }
                if ((!isSpace(c)) && c != ';' && c != ')' && c != '+' && c != '-' && c != '*' && c != '/' && c != '|' && c != '&') {
                    return ERROR_TOKEN;
                }
                ungetc(c, fp);
                return INT_CONST;
            }
            // 八进制
            if (isNum(c) && c <= '7') {
                do {
                    add2token(c);
                } while (c = fgetc(fp), isNum(c)&& c <= '7');
                if (c == 'L' || c == 'l') {
                    add2token(c);
                    return LONG_CONST;
                }
                if ((!isSpace(c)) && c != ';' && c != ')' && c != '+' && c != '-' && c != '*' && c != '/') {
                    return ERROR_TOKEN;
                }
                ungetc(c, fp);
                return INT_CONST;
            }
            ungetc(c, fp);
            c='0';
            clearToken();
        }
        do {
            add2token(c);
        } while (c = fgetc(fp), isNum(c));
        if (isSpace(c) || c == ';' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '|' || c == '&'){
            ungetc(c, fp);
            return INT_CONST;
        }
        if (c == '.') {
            do {
                add2token(c);
            } while (c = fgetc(fp), isNum(c));
            if (c == 'F' || c == 'f') {
                add2token(c);
                return FLOAT_CONST;
            }
            if (c == 'L' || c == 'l') {
                add2token(c);
                return DOUBLE_CONST;
            }
            if (isSpace(c) || c == ';' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '|' || c == '&'){
                ungetc(c, fp);
                return DOUBLE_CONST;
            }
            return ERROR_TOKEN;
        }
        if (c == 'L' || c == 'l'){ // l
            add2token(c);
            return LONG_CONST;
        }
    }

    switch (c) {
        // float that starts with `.`
        case '.':
            do {
                add2token(c);
            } while (c = fgetc(fp), isNum(c));
            ungetc(c, fp);
            return FLOAT_CONST;

            // char
        case '\'':
            add2token('\'');
            if ((c = fgetc(fp)) != '\\') {
                add2token(c);
                if ((c = fgetc(fp)) == '\'') {
                    add2token(c);
                    return CHAR_CONST;
                } else {
                    return ERROR_TOKEN;
                }
            } else { //  '\*'
                c = fgetc(fp);
                if (c == 'n' || c == 't' || c == '\\' || c == '\'' || c == '\"') {
                    add2token(c);
                    if ((c = fgetc(fp)) == '\'') {
                        add2token(c);
                        return CHAR_CONST;
                    } else {
                        return ERROR_TOKEN;
                    }
                } else if (c == 'x') { // hex
                    add2token(c);
                    if (isXNum((c = fgetc(fp)))) {
                        add2token(c);
                        if (isXNum((c = fgetc(fp)))) {
                            add2token(c);
                        } else {
                            ungetc(c, fp);
                        }
                        if ((c = fgetc(fp)) == '\'') {
                            add2token(c);
                            return CHAR_CONST;
                        } else {
                            return ERROR_TOKEN;
                        }
                    } else {
                        return ERROR_TOKEN;
                    }
                } else if (c >= '0' && c <= '7') {
                    // oct
                    add2token(c);
                    if ((c = fgetc(fp)) >= '0' && c <= '7') {
                        add2token(c);
                        if ((c = fgetc(fp)) >= '0' && c <= '7') {
                            add2token(c);
                            if ((c = fgetc(fp)) == '\'') {
                                add2token(c);
                                return CHAR_CONST;
                            } else {
                                return ERROR_TOKEN;
                            }
                        } else if (c == '\'') {
                            add2token('\'');
                            return CHAR_CONST;
                        } else {
                            return ERROR_TOKEN;
                        }
                    } else {
                        if (c == '\'') {
                            add2token(c);
                            return CHAR_CONST;
                        } else {
                            ungetc(c, fp);
                            return ERROR_TOKEN;
                        }
                    }
                } else {
                    return ERROR_TOKEN;
                }
            }

            // string
        case '"':
            do {
                if (c != '\\')
                    add2token(c);
                if (c == '\\') {
                    c = fgetc(fp);
                    add2token(c);
                }
            } while ((c = fgetc(fp)) != '"' && c != '\n');
            if (c == '"') {
                add2token('"');
                return STRING_CONST;
            } else {
                return ERROR_TOKEN;
            }

            // div / comment
        case '/':
            add2token(c);
            if ((c = fgetc(fp)) == '/') {
                do {
                    add2token(c);
                } while ((c = fgetc(fp)) != '\n');
                ungetc(c, fp);
                return ANNO;
            } else if (c == '*') {
                while (1) {
                    add2token(c);
                    c = fgetc(fp);
                    if (c == '*') {
                        add2token(c);
                        if ((c = fgetc(fp)) == '/') {
                            add2token(c);
                            return ANNO;
                        }
                    }
                    if (c == '\n') {
                        add2token(c);
                        // Align the output
                        c = '\t';
                        add2token(c);
                        add2token(c);
                    }
                }
            } else {
                ungetc(c, fp);
                return DIVIDE;
            }
        case '#':
            add2token(c);
            if (c = fgetc(fp), isAlpha(c)) {
                do {
                    add2token(c);
                } while (c = fgetc(fp), isAlpha(c));

                if (strcmp(token_text_, "#include") == 0) {
                    // include
                    do {
                        add2token(c);
                    } while (c = fgetc(fp),c != '\n');
                    row_++;
                    return INCLUDE;
                } else if (strcmp(token_text_, "#define") == 0) {
                    // define
                    do {
                        add2token(c);
                    } while (c= fgetc(fp), c!='\n');
                    row_++;
                    return DEFINE;
                } else {
                    return ERROR_TOKEN;
                }
            } else {
                return ERROR_TOKEN;
            }
        case ',':
            add2token(c);
            return COMMA;
        case ';':
            add2token(c);
            return SEMI;
        case '=':
            c = fgetc(fp);
            if (c == '=') {
                add2token(c);
                add2token(c);
                return EQ;
            }
            ungetc(c, fp);
            add2token('=');
            return ASSIGN;
        case '!':
            c = fgetc(fp);
            if (c == '=') {
                add2token('!');
                add2token('=');
                return NEQ;
            } else {
                return ERROR_TOKEN;
            }
        case '+':
            add2token(c);
            return PLUS;
        case '-':
            add2token(c);
            return MINUS;
        case '(':
            add2token(c);
            return LP;
        case ')':
            add2token(c);
            return RP;
        case '{':
            add2token(c);
            return LB;
        case '}':
            add2token(c);
            return RB;
        case '[':
            add2token(c);
            return LM;
        case ']':
            add2token(c);
            return RM;
        case '*':
            add2token(c);
            return TIMES;
        case '>':
            add2token(c);
            if ((c = fgetc(fp)) == '=') {
                add2token(c);
                return MOREEQ;
            } else {
                ungetc(c, fp);
                return MORE;
            }
        case '<':
            add2token(c);
            if ((c = fgetc(fp)) == '=') {
                add2token(c);
                return LESSEQ;
            } else {
                ungetc(c, fp);
                return LESS;
            }
        case '&':
            add2token(c);
            if ((c= fgetc(fp) == '&')){
                add2token(c);
                return AND;
            }
            return ERROR_TOKEN;
        case '|':
            add2token(c);
            if ((c= fgetc(fp) == '|')){
                add2token(c);
                return OR;
            }
            return ERROR_TOKEN;
        case EOF:
            return EOF;
        default:
            return ERROR_TOKEN;
    }
}
