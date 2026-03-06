#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define Linked List Node
typedef struct Node {
    char data;
    int count;
    struct Node *next;
} Node;

// Create a new node
Node* createNode(char c) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode) {
        newNode->data = c;
        newNode->count = 1;
        newNode->next = NULL;
    }
    return newNode;
}

int main() {
    // Open the source file itself
    FILE *fp = fopen("main.c", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    Node *head = NULL;
    int ch; // Use int to handle EOF correctly

    // Read character by character
    while ((ch = fgetc(fp)) != EOF) {
        Node *curr = head;
        Node *prev = NULL;
        int found = 0;

        // Search for existing character in the list
        while (curr != NULL) {
            if (curr->data == (char)ch) {
                curr->count++;
                found = 1;
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        // If not found, add a new node
        if (!found) {
            Node *newNode = createNode((char)ch);
            if (head == NULL) {
                head = newNode;
            } else {
                prev->next = newNode;
            }
        }
    }

    fclose(fp);

    // Display results
    printf("Character Statistics (Char: Count):\n");
    Node *temp = head;
    while (temp != NULL) {
        if (temp->data == '\n') {
            printf("'\\n': %d\n", temp->count);
        } else if (temp->data == ' ') {
            printf("' ': %d\n", temp->count);
        } else if (temp->data == '\t') {
            printf("'\\t': %d\n", temp->count);
        } else if (temp->data == '\r') {
            printf("'\\r': %d\n", temp->count);
        } else {
            printf("'%c': %d\n", temp->data, temp->count);
        }
        
        // Free memory
        Node *toDelete = temp;
        temp = temp->next;
        free(toDelete);
    }

    return 0;
}