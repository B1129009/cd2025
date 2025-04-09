#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char token_str[64];  // 存整個 token 字串
    char token_type[32]; // 存 token 類型名稱
    struct Node* next;
} Node;

void insert(Node** head, const char* token_str, const char* token_type) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    strcpy(newNode->token_str, token_str);
    strcpy(newNode->token_type, token_type);
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* current = *head;
        while (current->next != NULL)
            current = current->next;
        current->next = newNode;
    }
}

void printList(Node* head) {
    while (head) {
        printf("%s: %s\n", head->token_str, head->token_type);
        head = head->next;
    }
}

void freeList(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int isAlpha(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

int isDigit(char ch) {
    return (ch >= '0' && ch <= '9');
}

int isWhitespace(char ch) {
    return ch == ' ' || ch == '\n' || ch == '\t';
}

const char* checkKeyword(const char* str) {
    if (strcmp(str, "int") == 0) return "TYPE_TOKEN";
    if (strcmp(str, "main") == 0) return "MAIN_TOKEN";
    if (strcmp(str, "if") == 0) return "IF_TOKEN";
    if (strcmp(str, "else") == 0) return "ELSE_TOKEN";
    if (strcmp(str, "while") == 0) return "WHILE_TOKEN";
    return "ID_TOKEN";
}

const char* matchSymbol(const char* str) {
    if (strcmp(str, "==") == 0) return "EQUAL_TOKEN";
    if (strcmp(str, ">=") == 0) return "GREATEREQUAL_TOKEN";
    if (strcmp(str, "<=") == 0) return "LESSEQUAL_TOKEN";
    if (strcmp(str, ">") == 0) return "GREATER_TOKEN";
    if (strcmp(str, "<") == 0) return "LESS_TOKEN";
    if (strcmp(str, "=") == 0) return "ASSIGN_TOKEN";
    if (strcmp(str, "+") == 0) return "PLUS_TOKEN";
    if (strcmp(str, "-") == 0) return "MINUS_TOKEN";
    if (strcmp(str, "(") == 0) return "LEFTPAREN_TOKEN";
    if (strcmp(str, ")") == 0) return "REFTPAREN_TOKEN";
    if (strcmp(str, "{") == 0) return "LEFTBRACE_TOKEN";
    if (strcmp(str, "}") == 0) return "REFTBRACE_TOKEN";
    if (strcmp(str, ";") == 0) return "SEMICOLON_TOKEN";
    return NULL;
}

void scanFile(const char* filename, Node** tokenList) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open input file");
        exit(1);
    }

    char ch, buffer[64];
    int idx = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (isWhitespace(ch)) continue;

        if (isAlpha(ch)) {
            buffer[idx++] = ch;
            while ((ch = fgetc(file)) != EOF && (isAlpha(ch) || isDigit(ch)))
                buffer[idx++] = ch;
            buffer[idx] = '\0';
            const char* type = checkKeyword(buffer);
            insert(tokenList, buffer, type);
            idx = 0;
            if (ch != EOF) ungetc(ch, file);
        }
        else if (isDigit(ch)) {
            buffer[idx++] = ch;
            while ((ch = fgetc(file)) != EOF && isDigit(ch))
                buffer[idx++] = ch;
            buffer[idx] = '\0';
            insert(tokenList, buffer, "LITERAL_TOKEN");
            idx = 0;
            if (ch != EOF) ungetc(ch, file);
        }
        else {
            buffer[idx++] = ch;
            ch = fgetc(file);
            if (ch != EOF) {
                buffer[idx++] = ch;
                buffer[idx] = '\0';
                const char* type = matchSymbol(buffer);
                if (type) {
                    insert(tokenList, buffer, type);
                } else {
                    buffer[1] = '\0';
                    ungetc(ch, file);
                    type = matchSymbol(buffer);
                    if (type)
                        insert(tokenList, buffer, type);
                }
            } else {
                buffer[1] = '\0';
                const char* type = matchSymbol(buffer);
                if (type)
                    insert(tokenList, buffer, type);
            }
            idx = 0;
        }
    }

    fclose(file);
}

int main() {
    Node* tokenList = NULL;
    scanFile("input.c", &tokenList);
    printList(tokenList);
    freeList(tokenList);
    return 0;
}
