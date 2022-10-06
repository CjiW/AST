#include "header.h"

int row_ = 1;
int isEOF = 0;
char token_text_[MAX_LEN];
char Keywords[][20] =
    {
        "void", "char", "short", "int", "long", "float", "double",
        "if", "else", "do", "while", "for", "break", "return", "continue"};

int iKeyword[] =
    {
        VOID, CHAR, SHORT, INT, LONG, FLOAT, DOUBLE,
        IF, ELSE, DO, WHILE, FOR, BREAK, RETURN, CONTINUE};

int numKeyword = sizeof iKeyword / sizeof(int);

int isSpace(char c)
{
    return (c == ' ' || c == '\n' || c == '\t' || c == '\v' || c == '\f');
}

int isNum(char c) { return (c >= '0' && c <= '9'); }

int isXNum(char c)
{
    return ((isNum(c)) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}

int isAlpha(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

void Warning(char *s)
{
    printf("[line:%d]ERROR: %s\n", row_, s);
}

int isLetterOrNumOr_(char c) { return isAlpha(c) || isNum(c) || c == '_'; }

int text_idx_ = 0;

void clearToken()
{
    memset(token_text_, 0, sizeof token_text_);
    text_idx_ = 0;
}
char get_char(FILE *fp)
{
    char c = fgetc(fp);
    if (c == EOF)
    {
        if (isEOF == 1)
        {
            Warning("Unexpected EOF!");
            exit(0);
        }
        else
            isEOF = 1;
    }
    return c;
}
int add2token(char c)
{
    if (text_idx_ >= MAX_LEN - 1)
    {
        Warning("Word Too Long!");
        return -1; //添加失败
    }
    *(token_text_ + text_idx_) = c;
    *(token_text_ + text_idx_ + 1) = '\0';
    text_idx_++;
    return 1;
}

int getToken(FILE *fp)
{
    char c;
    clearToken();
    // eat Space
    do
    {
        c = get_char(fp);
        if (c == '\n')
        {
            row_++;
        }
    } while (isSpace(c));

    // IDENT、ARRAY or KEYWORD
    if (isAlpha(c) || c == '_')
    {
        do
        {
            add2token(c);
            c = get_char(fp);
        } while (isLetterOrNumOr_(c));
        ungetc(c, fp);
        for (int i = 0; i < numKeyword; i++)
        {
            if (strcmp(token_text_, Keywords[i]) == 0)
            {
                return iKeyword[i];
            }
        }
        c = get_char(fp);

        if (c == '[')
        {
            // ARRAY
            add2token(c);
            c = get_char(fp);
            while (isNum(c))
            {
                add2token(c);
                c = get_char(fp);
            }
            if (c != ']')
            {
                return ERROR_TOKEN;
            }
            add2token(c);
            return ARRAY;
        }
        else
        {
            ungetc(c, fp);
        }
        return IDENT;
    }

    // number
    if (isNum(c))
    {
        if (c == '0')
        {
            add2token(c);
            c = get_char(fp);
            // 十六进制
            if (c == 'x' || c == 'X')
            {
                do
                {
                    add2token(c);
                } while (c = get_char(fp), isXNum(c));
                if (c == 'L' || c == 'l')
                {
                    add2token(c);
                    return LONG_CONST;
                }
                if ((!isSpace(c)) && c != ';' && c != ')' && c != '+' && c != '-' && c != '*' && c != '/' && c != '|' &&
                    c != '&')
                {
                    return ERROR_TOKEN;
                }
                ungetc(c, fp);
                return INT_CONST;
            }
            // 八进制
            if (isNum(c) && c <= '7')
            {
                do
                {
                    add2token(c);
                } while (c = get_char(fp), isNum(c) && c <= '7');
                if (c == 'L' || c == 'l')
                {
                    add2token(c);
                    return LONG_CONST;
                }
                if ((!isSpace(c)) && c != ';' && c != ')' && c != '+' && c != '-' && c != '*' && c != '/')
                {
                    return ERROR_TOKEN;
                }
                ungetc(c, fp);
                return INT_CONST;
            }
            ungetc(c, fp);
            c = '0';
            clearToken();
        }
        do
        {
            add2token(c);
        } while (c = get_char(fp), isNum(c));
        if (isSpace(c) || c == ';' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '|' ||
            c == '&')
        {
            ungetc(c, fp);
            return INT_CONST;
        }
        if (c == '.')
        {
            do
            {
                add2token(c);
            } while (c = get_char(fp), isNum(c));
            if (c == 'F' || c == 'f')
            {
                add2token(c);
                return FLOAT_CONST;
            }
            if (c == 'L' || c == 'l')
            {
                add2token(c);
                return DOUBLE_CONST;
            }
            if (isSpace(c) || c == ';' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '|' ||
                c == '&')
            {
                ungetc(c, fp);
                return DOUBLE_CONST;
            }
            return ERROR_TOKEN;
        }
        if (c == 'L' || c == 'l')
        { // l
            add2token(c);
            return LONG_CONST;
        }
    }

    switch (c)
    {
    // float that starts with `.`
    case '.':
        do
        {
            add2token(c);
        } while (c = get_char(fp), isNum(c));
        ungetc(c, fp);
        return FLOAT_CONST;

        // char
    case '\'':
        add2token('\'');
        if ((c = get_char(fp)) != '\\')
        {
            add2token(c);
            if ((c = get_char(fp)) == '\'')
            {
                add2token(c);
                return CHAR_CONST;
            }
            else
            {
                return ERROR_TOKEN;
            }
        }
        else
        { //  '\*'
            c = get_char(fp);
            if (c == 'n' || c == 't' || c == '\\' || c == '\'' || c == '\"')
            {
                add2token(c);
                if ((c = get_char(fp)) == '\'')
                {
                    add2token(c);
                    return CHAR_CONST;
                }
                else
                {
                    return ERROR_TOKEN;
                }
            }
            else if (c == 'x')
            { // hex
                add2token(c);
                if (isXNum((c = get_char(fp))))
                {
                    add2token(c);
                    if (isXNum((c = get_char(fp))))
                    {
                        add2token(c);
                    }
                    else
                    {
                        ungetc(c, fp);
                    }
                    if ((c = get_char(fp)) == '\'')
                    {
                        add2token(c);
                        return CHAR_CONST;
                    }
                    else
                    {
                        return ERROR_TOKEN;
                    }
                }
                else
                {
                    return ERROR_TOKEN;
                }
            }
            else if (c >= '0' && c <= '7')
            {
                // oct
                add2token(c);
                if ((c = get_char(fp)) >= '0' && c <= '7')
                {
                    add2token(c);
                    if ((c = get_char(fp)) >= '0' && c <= '7')
                    {
                        add2token(c);
                        if ((c = get_char(fp)) == '\'')
                        {
                            add2token(c);
                            return CHAR_CONST;
                        }
                        else
                        {
                            return ERROR_TOKEN;
                        }
                    }
                    else if (c == '\'')
                    {
                        add2token('\'');
                        return CHAR_CONST;
                    }
                    else
                    {
                        return ERROR_TOKEN;
                    }
                }
                else
                {
                    if (c == '\'')
                    {
                        add2token(c);
                        return CHAR_CONST;
                    }
                    else
                    {
                        ungetc(c, fp);
                        return ERROR_TOKEN;
                    }
                }
            }
            else
            {
                return ERROR_TOKEN;
            }
        }

        // string
    case '"':
        do
        {
            if (c != '\\')
                add2token(c);
            if (c == '\\')
            {
                c = get_char(fp);
                add2token(c);
            }
        } while ((c = get_char(fp)) != '"' && c != '\n');
        if (c == '"')
        {
            add2token('"');
            return STRING_CONST;
        }
        else
        {
            return ERROR_TOKEN;
        }

        // div / comment
    case '/':
        add2token(c);
        if ((c = get_char(fp)) == '/')
        {
            do
            {
                add2token(c);
            } while ((c = get_char(fp)) != '\n');
            ungetc(c, fp);
            return ANNO;
        }
        else if (c == '*')
        {
            while (1)
            {
                add2token(c);
                c = get_char(fp);
                if (c == '*')
                {
                    add2token(c);
                    if ((c = get_char(fp)) == '/')
                    {
                        add2token(c);
                        return ANNO;
                    }
                }
                if (c == '\n')
                {
                    add2token(c);
                    // Align the output
                    c = '\t';
                    add2token(c);
                    add2token(c);
                }
            }
        }
        else
        {
            ungetc(c, fp);
            return DIVIDE;
        }
    case '#':
        add2token(c);
        if (c = get_char(fp), isAlpha(c))
        {
            do
            {
                add2token(c);
            } while (c = get_char(fp), isAlpha(c));

            if (strcmp(token_text_, "#include") == 0)
            {
                // include
                do
                {
                    add2token(c);
                } while (c = get_char(fp), c != '\n');
                row_++;
                return INCLUDE;
            }
            else if (strcmp(token_text_, "#define") == 0)
            {
                // define
                do
                {
                    add2token(c);
                } while (c = get_char(fp), c != '\n');
                row_++;
                return DEFINE;
            }
            else
            {
                return ERROR_TOKEN;
            }
        }
        else
        {
            return ERROR_TOKEN;
        }
    case ',':
        add2token(c);
        return COMMA;
    case ';':
        add2token(c);
        return SEMI;
    case '=':
        c = get_char(fp);
        if (c == '=')
        {
            add2token(c);
            add2token(c);
            return EQ;
        }
        ungetc(c, fp);
        add2token('=');
        return ASSIGN;
    case '!':
        c = get_char(fp);
        if (c == '=')
        {
            add2token('!');
            add2token('=');
            return NEQ;
        }
        else
        {
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
        if ((c = get_char(fp)) == '=')
        {
            add2token(c);
            return MOREEQ;
        }
        else
        {
            ungetc(c, fp);
            return MORE;
        }
    case '<':
        add2token(c);
        if ((c = get_char(fp)) == '=')
        {
            add2token(c);
            return LESSEQ;
        }
        else
        {
            ungetc(c, fp);
            return LESS;
        }
    case '&':
        add2token(c);
        if ((c = get_char(fp)) == '&')
        {
            add2token(c);
            return AND;
        }
        return ERROR_TOKEN;
    case '|':
        add2token(c);
        if ((c = get_char(fp)) == '|')
        {
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
