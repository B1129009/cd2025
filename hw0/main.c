#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char character;
    int count;
    struct Node* next;
} Node;

void insert(Node** head, char ch) {
    if (ch == '\n') return;
    Node* current = *head;
    Node* prev = NULL;
    
    while (current != NULL) {
        if (current->character == ch) {
            current->count++;
            return;
        }
        prev = current;
        current = current->next;
    }
    
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "\n");
        exit(1);
    }
    newNode->character = ch;
    newNode->count = 1;
    newNode->next = NULL;
    
    if (prev == NULL) {
        *head = newNode;
    } else {
        prev->next = newNode;
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

void printList(Node* head) {
    while (head) {
        printf("%c: %d\n", head->character, head->count);
        head = head->next;
    }
}

int main() {
    FILE* file = fopen(__FILE__, "r");
    if (!file) {
        perror("err");
        return 1;
    }
    
    Node* head = NULL;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        insert(&head, ch);
    }
    
    fclose(file);
    printList(head);
    freeList(head);
    return 0;
}