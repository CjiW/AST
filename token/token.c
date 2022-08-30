#include "token.h"

char token_text_[128]; //存放单词自身值
char string_num_[128]; //存放数字的字符串
int row_ = 1;
int col_ = 1;

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

int add2token(char *token, char c) {
    int i = 0;
    while (*(token + i) != '\0') {
        i++;
    }
    /* max len : 128 */
    if (i >= 127) {
        return -1; //添加失败
    }
    *(token + i) = c;
    *(token + i + 1) = '\0';
    return 1;
}

int getToken(FILE *fp) {
    char c;
    *(token_text_) = '\0';
    // eat Space
    do {
        c = fgetc(fp);
        col_++;
        if (c == '\n') {
            row_++;
            col_ = 1;
        }
    } while (isSpace(c));

    // IDENT、ARRAY or KEYWORD
    if (isAlpha(c) || c == '_') {
        do {
            add2token(token_text_, c);
            col_++;
        } while (c = fgetc(fp), isLetterOrNum(c));
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
            add2token(token_text_, c);
            col_++;
            c = fgetc(fp);
            while (c >= '0' && c <= '9') {
                add2token(token_text_, c);
                col_++;
                add2token(string_num_, c);
                col_++;
                c = fgetc(fp);
            }
            if (c != ']') {
                return ERROR_TOKEN;
            }
            add2token(token_text_, c);
            col_++;
            return ARRAY;
        } else {
            ungetc(c, fp);
        }
        return IDENT;
    }

    // number
    if (isNum(c)) {
        if (c == '0') {
            add2token(token_text_, c);
            col_++;
            c = fgetc(fp);
            if (c == 'x' || c == 'X') {
                do {
                    add2token(token_text_, c);
                    col_++;
                } while (c = fgetc(fp), isXNum(c));
                if (c != 'u' && c != 'l') {
                    if ((!isSpace(c)) && c != ';' && c != ')' && c != '+' && c != '-' &&
                        c != '*' && c != '/') {
                        return ERROR_TOKEN;
                    }
                    ungetc(c, fp);
                    return INT_CONST;
                } else if (c == 'u') {
                    add2token(token_text_, c);
                    col_++;
                    c = fgetc(fp);
                    if (c == 'l') { // ul
                        add2token(token_text_, c);
                        col_++;
                        c = fgetc(fp);
                        if (c == 'l') { // ull
                            add2token(token_text_, c);
                            col_++;
                            return INT_CONST;
                        } else {
                            ungetc(c, fp);
                            return INT_CONST;
                        }
                    } else { // u
                        ungetc(c, fp);
                        return INT_CONST;
                    }
                } else { // l
                    add2token(token_text_, c);
                    col_++;
                    return INT_CONST;
                }
            }
            if (isNum(c) && c < '7') {
                do {
                    add2token(token_text_, c);
                    col_++;
                } while (c = fgetc(fp), isNum(c)&& c < '7');
                if (c != 'u' && c != 'l') {
                    if ((!isSpace(c)) && c != ';' && c != ')' && c != '+' && c != '-' &&
                        c != '*' && c != '/') {
                        return ERROR_TOKEN;
                    }
                    ungetc(c, fp);
                    return INT_CONST;
                }else if (c == 'u') {
                    add2token(token_text_, c);
                    col_++;
                    c = fgetc(fp);
                    if (c == 'l') { // ul
                        add2token(token_text_, c);
                        col_++;
                        c = fgetc(fp);
                        if (c == 'l') { // ull
                            add2token(token_text_, c);
                            col_++;
                            return INT_CONST;
                        } else {
                            ungetc(c, fp);
                            return INT_CONST;
                        }
                    } else { // u
                        ungetc(c, fp);
                        return INT_CONST;
                    }
                } else { // l
                    add2token(token_text_, c);
                    col_++;
                    return INT_CONST;
                }
            }
            if (c != '.' && c != 'u' && c != 'l'){
                if ((!isSpace(c)) && c != ';' && c != ')' && c != '+' && c != '-' &&
                    c != '*' && c != '/') {
                    return ERROR_TOKEN;
                }
                ungetc(c, fp);
                return INT_CONST;
            }else if (c == 'u') {
                add2token(token_text_, c);
                col_++;
                c = fgetc(fp);
                if (c == 'l') { // ul
                    add2token(token_text_, c);
                    col_++;
                    c = fgetc(fp);
                    if (c == 'l') { // ull
                        add2token(token_text_, c);
                        col_++;
                        return INT_CONST;
                    } else {
                        ungetc(c, fp);
                        return INT_CONST;
                    }
                } else { // u
                    ungetc(c, fp);
                    return INT_CONST;
                }
            } else if(c=='l'){ // l
                add2token(token_text_, c);
                col_++;
                return INT_CONST;
            }else{

            }
        }
        do {
            add2token(token_text_, c);
            col_++;
        } while (c = fgetc(fp), isNum(c));
        if (c != '.' && c != 'u' && c != 'l') {
            if ((!isSpace(c)) && c != ';' && c != ')' && c != '+' && c != '-' &&
                c != '*' && c != '/') {
                return ERROR_TOKEN;
            }
            ungetc(c, fp);
            return INT_CONST;
        } else if (c == '.') {
            c = fgetc(fp);
            if (!isNum(c)) {
                return ERROR_TOKEN;
            } else {
                ungetc(c, fp);
                c = '.';
                add2token(token_text_, c);
                col_++;
                c = fgetc(fp);
                do {
                    add2token(token_text_, c);
                    col_++;
                } while (c = fgetc(fp), isNum(c));
                if (c == 'f') {
                    add2token(token_text_, c);
                    col_++;
                    return FLOAT_CONST;
                } else {
                    ungetc(c, fp);
                }
                return FLOAT_CONST;
            }
        } else if (c == 'u') {
            add2token(token_text_, c);
            col_++;
            c = fgetc(fp);
            if (c == 'l') { // ul
                add2token(token_text_, c);
                col_++;
                c = fgetc(fp);
                if (c == 'l') { // ull
                    add2token(token_text_, c);
                    col_++;
                    return INT_CONST;
                } else {
                    ungetc(c, fp);
                    return INT_CONST;
                }
            } else { // u
                ungetc(c, fp);
                return INT_CONST;
            }
        } else { // l
            add2token(token_text_, c);
            col_++;
            return INT_CONST;
        }
    }

    switch (c) {
        // float that starts with `.`
        case '.':
            do {
                add2token(token_text_, c);
                col_++;
            } while (c = fgetc(fp), isNum(c));
            ungetc(c, fp);
            return FLOAT_CONST;

            // char
        case '\'':
            add2token(token_text_, '\'');
            col_++;
            if ((c = fgetc(fp)) != '\\') {
                add2token(token_text_, c);
                col_++;
                if ((c = fgetc(fp)) == '\'') {
                    add2token(token_text_, c);
                    col_++;
                    return CHAR_CONST;
                } else {
                    return ERROR_TOKEN;
                }
            } else { //  '\*'
                c = fgetc(fp);
                if (c == 'n' || c == 't' || c == '\\' || c == '\'' || c == '\"') {
                    add2token(token_text_, c);
                    col_++;
                    if ((c = fgetc(fp)) == '\'') {
                        add2token(token_text_, c);
                        col_++;
                        return CHAR_CONST;
                    } else {
                        return ERROR_TOKEN;
                    }
                } else if (c == 'x') { // hex
                    add2token(token_text_, c);
                    if (isXNum((c = fgetc(fp)))) {
                        add2token(token_text_, c);
                        if (isXNum((c = fgetc(fp)))) {
                            add2token(token_text_, c);
                        } else {
                            ungetc(c, fp);
                        }
                        if ((c = fgetc(fp)) == '\'') {
                            add2token(token_text_, c);
                            return CHAR_CONST;
                        } else {
                            return ERROR_TOKEN;
                        }
                    } else {
                        return ERROR_TOKEN;
                    }
                } else if (c >= '0' && c <= '7') {
                    // oct
                    add2token(token_text_, c);
                    if ((c = fgetc(fp)) >= '0' && c <= '7') {
                        add2token(token_text_, c);
                        if ((c = fgetc(fp)) >= '0' && c <= '7') {
                            add2token(token_text_, c);
                            if ((c = fgetc(fp)) == '\'') {
                                add2token(token_text_, c);
                                return CHAR_CONST;
                            } else {
                                return ERROR_TOKEN;
                            }
                        } else if (c == '\'') {
                            add2token(token_text_, '\'');
                            return CHAR_CONST;
                        } else {
                            return ERROR_TOKEN;
                        }
                    } else {
                        if (c == '\'') {
                            add2token(token_text_, c);
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
                    add2token(token_text_, c);
                if (c == '\\') {
                    c = fgetc(fp);
                    add2token(token_text_, c);
                }
            } while ((c = fgetc(fp)) != '"' && c != '\n');
            if (c == '"') {
                add2token(token_text_, '"');
                return STRING_CONST;
            } else {
                return ERROR_TOKEN;
            }

            // div / comment
        case '/':
            add2token(token_text_, c);
            if ((c = fgetc(fp)) == '/') {
                do {
                    add2token(token_text_, c);
                } while ((c = fgetc(fp)) != '\n');
                ungetc(c, fp);
                return ANNO;
            } else if (c == '*') {
                while (1) {
                    add2token(token_text_, c);
                    c = fgetc(fp);
                    if (c == '*') {
                        add2token(token_text_, c);
                        if ((c = fgetc(fp)) == '/') {
                            add2token(token_text_, c);
                            return ANNO;
                        }
                    }
                    if (c == '\n') {
                        add2token(token_text_, c);
                        // Align the output
                        c = '\t';
                        add2token(token_text_, c);
                        add2token(token_text_, c);
                    }
                }
            } else {
                ungetc(c, fp);
                return DIVIDE;
            }

            // macro
        case '#':
            add2token(token_text_, c);
            if (c = fgetc(fp), isAlpha(c)) {
                do {
                    add2token(token_text_, c);
                } while (c = fgetc(fp), isAlpha(c));
                if (strcmp(token_text_, "#include") == 0) {
                    do {
                        add2token(token_text_, c);
                    } while ((c = fgetc(fp)) != '\n');
                    ungetc(c, fp);
                    return INCLUDE;
                } else if (strcmp(token_text_, "#define") == 0) {
                    do {
                        add2token(token_text_, c);
                    } while ((c = fgetc(fp)) != '\n');
                    return MACRO;
                } else {
                    return ERROR_TOKEN;
                }
            } else {
                return ERROR_TOKEN;
            }
        case ',':
            add2token(token_text_, c);
            return COMMA;
        case ';':
            add2token(token_text_, c);
            return SEMI;
        case '=':
            c = fgetc(fp);
            if (c == '=') {
                add2token(token_text_, c);
                add2token(token_text_, c);
                return EQ;
            }
            ungetc(c, fp);
            add2token(token_text_, '=');
            return ASSIGN;
        case '!':
            c = fgetc(fp);
            if (c == '=') {
                add2token(token_text_, '!');
                add2token(token_text_, '=');
                return NEQ;
            } else {
                return ERROR_TOKEN;
            }
        case '+':
            add2token(token_text_, '+');
            return PLUS;
        case '-':
            add2token(token_text_, '-');
            return MINUS;
        case '(':
            add2token(token_text_, c);
            return LP;
        case ')':
            add2token(token_text_, c);
            return RP;
        case '{':
            add2token(token_text_, c);
            return LB;
        case '}':
            add2token(token_text_, c);
            return RB;
        case '[':
            add2token(token_text_, c);
            return LM;
        case ']':
            add2token(token_text_, c);
            return RM;
        case '*':
            add2token(token_text_, c);
            return TIMES;
        case '>':
            add2token(token_text_, c);
            if ((c = fgetc(fp)) == '=') {
                add2token(token_text_, c);
                return MOREEQUAL;
            } else {
                ungetc(c, fp);
                return MORE;
            }
        case '<':
            add2token(token_text_, c);
            if ((c = fgetc(fp)) == '=') {
                add2token(token_text_, c);
                return LESSEQUAL;
            } else {
                ungetc(c, fp);
                return LESS;
            }
        case '&':
            add2token(token_text_, c);
            if ((c= fgetc(fp) == '&')){
                add2token(token_text_, c);
                return AND;
            }
            return ERROR_TOKEN;
        case '|':
            add2token(token_text_, c);
            if ((c= fgetc(fp) == '|')){
                add2token(token_text_, c);
                return OR;
            }
            return ERROR_TOKEN;
        case EOF:
            return EOF;
        default:
            return ERROR_TOKEN;
    }
}
