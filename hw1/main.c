#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 根據 HW1 要求，僅 include 必要函式庫
// 這裡將原本儲存字元的 Linked List 修改為輔助判斷或處理 Token 的邏輯（若需要的話）
// 但 Scanner 的核心通常是透過狀態機直接輸出

void get_token(FILE *fp) {
    int ch;
    char buffer[256];
    int idx;

    while ((ch = fgetc(fp)) != EOF) {
        // 1. 忽略空白字元、換行、Tab
        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r') {
            continue;
        }

        // 2. 辨識關鍵字與識別碼 (ID): 以字母開頭 [a-zA-Z]
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            idx = 0;
            buffer[idx++] = (char)ch;
            while ((ch = fgetc(fp)) != EOF && 
                   ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))) {
                buffer[idx++] = (char)ch;
            }
            ungetc(ch, fp); // 將多讀取的字元退回
            buffer[idx] = '\0';

            // 判斷是否為 Keywords
            if (strcmp(buffer, "int") == 0) printf("%s: TYPE_TOKEN\n", buffer);
            else if (strcmp(buffer, "main") == 0) printf("%s: MAIN_TOKEN\n", buffer);
            else if (strcmp(buffer, "if") == 0) printf("%s: IF_TOKEN\n", buffer);
            else if (strcmp(buffer, "else") == 0) printf("%s: ELSE_TOKEN\n", buffer);
            else if (strcmp(buffer, "while") == 0) printf("%s: WHILE_TOKEN\n", buffer);
            else printf("%s: ID_TOKEN\n", buffer); // 一般識別碼
        }

        // 3. 辨識數字 (Number): [0-9]
        else if (ch >= '0' && ch <= '9') {
            idx = 0;
            buffer[idx++] = (char)ch;
            while ((ch = fgetc(fp)) != EOF && (ch >= '0' && ch <= '9')) {
                buffer[idx++] = (char)ch;
            }
            ungetc(ch, fp);
            buffer[idx] = '\0';
            printf("%s: LITERAL_TOKEN\n", buffer);
        }

        // 4. 辨識關係運算子 (Relop) 與 指定 (=)
        else if (ch == '=') {
            int next = fgetc(fp);
            if (next == '=') printf("==: RELOP_TOKEN\n");
            else {
                ungetc(next, fp);
                printf("=: ASSIGN_TOKEN\n");
            }
        }
        else if (ch == '>') {
            int next = fgetc(fp);
            if (next == '=') printf(">=: RELOP_TOKEN\n");
            else {
                ungetc(next, fp);
                printf(">: RELOP_TOKEN\n");
            }
        }
        else if (ch == '<') {
            int next = fgetc(fp);
            if (next == '=') printf("<=: RELOP_TOKEN\n");
            else {
                ungetc(next, fp);
                printf("<: RELOP_TOKEN\n");
            }
        }

        // 5. 辨識符號 (,),{,},;,+,-
        else if (ch == '(') printf("(: LEFTPAREN_TOKEN\n");
        else if (ch == ')') printf("): RIGHTPAREN_TOKEN\n");
        else if (ch == '{') printf("{: LEFTBRACE_TOKEN\n");
        else if (ch == '}') printf("}: RIGHTBRACE_TOKEN\n");
        else if (ch == ';') printf(";: SEMICOLON_TOKEN\n");
        else if (ch == '+') printf("+: PLUS_TOKEN\n");
        else if (ch == '-') printf("-: MINUS_TOKEN\n");
    }
}

int main() {
    // 開啟原始碼檔案進行分析
    FILE *fp = fopen("main.c", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    get_token(fp);

    fclose(fp);
    return 0;
}