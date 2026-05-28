#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 4096
#define LEXEME_SIZE 128

enum TokenType {
    TOKEN_NUM,
    TOKEN_PLUS,
    TOKEN_LEFTPAREN,
    TOKEN_RIGHTPAREN,
    TOKEN_EOF,
    TOKEN_ERROR
};

char input[INPUT_SIZE];
char lexeme[LEXEME_SIZE];
int pos = 0;
int token = TOKEN_EOF;
int error = 0;

int isDigit(char c) {
    return c >= '0' && c <= '9';
}

int isWhiteSpace(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

void printIndent(int level) {
    int i;
    for (i = 0; i < level; i++) {
        printf("    ");
    }
}

void nextToken() {
    int i = 0;

    while (isWhiteSpace(input[pos])) {
        pos++;
    }

    if (input[pos] == '\0') {
        token = TOKEN_EOF;
        strcpy(lexeme, "EOF");
        return;
    }

    if (isDigit(input[pos])) {
        while (isDigit(input[pos]) && i < LEXEME_SIZE - 1) {
            lexeme[i++] = input[pos++];
        }
        lexeme[i] = '\0';
        token = TOKEN_NUM;
        return;
    }

    lexeme[0] = input[pos];
    lexeme[1] = '\0';

    if (input[pos] == '+') {
        token = TOKEN_PLUS;
    } else if (input[pos] == '(') {
        token = TOKEN_LEFTPAREN;
    } else if (input[pos] == ')') {
        token = TOKEN_RIGHTPAREN;
    } else {
        token = TOKEN_ERROR;
    }
    pos++;
}

void parseS(int level);
void parseSPrime(int level);
void parseE(int level);

void parseS(int level) {
    if (error) {
        return;
    }

    printIndent(level);
    printf("S -> E S'\n");
    parseE(level + 1);
    parseSPrime(level + 1);
}

void parseSPrime(int level) {
    if (error) {
        return;
    }

    if (token == TOKEN_PLUS) {
        printIndent(level);
        printf("S' -> + S\n");
        nextToken();
        parseS(level + 1);
    } else if (token == TOKEN_RIGHTPAREN || token == TOKEN_EOF) {
        printIndent(level);
        printf("S' -> epsilon\n");
    } else {
        error = 1;
    }
}

void parseE(int level) {
    if (error) {
        return;
    }

    if (token == TOKEN_NUM) {
        printIndent(level);
        printf("E -> num\n");
        printIndent(level + 1);
        printf("%s\n", lexeme);
        nextToken();
    } else if (token == TOKEN_LEFTPAREN) {
        printIndent(level);
        printf("E -> ( S )\n");
        nextToken();
        parseS(level + 1);

        if (token != TOKEN_RIGHTPAREN) {
            error = 1;
            return;
        }
        nextToken();
    } else {
        error = 1;
    }
}

int readInput() {
    int ch;
    int len = 0;

    while ((ch = getchar()) != EOF && len < INPUT_SIZE - 1) {
        input[len++] = (char)ch;
    }
    input[len] = '\0';

    if (len == 0) {
        strcpy(input, "(1+2+(3+4))+5");
    }

    return len;
}

int main() {
    readInput();
    nextToken();
    parseS(0);

    if (!error && token == TOKEN_EOF) {
        printf("Accept\n");
    } else {
        printf("Parse error\n");
    }

    return 0;
}
