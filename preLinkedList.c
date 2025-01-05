#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

/*
TODO:
     - Potentially make this into a header library file for future usage within C/C++ projects (may edit for std::string)
*/

#define BUFFER 50 // Buffer (a.k.a. max limit) of characters that can be used within an array.

union unsafeConversion{ // I am using this union for unsafe BINARY conversion between a float and integer, they both occupy the same amout of memory (4 bytes)
    int integerType;
    int floatType;
}typedef unsafeConversion; // IGNORE, this is now useless.

enum dataTypes{ // Using enums for deferencing void pointers.
    CHAR_TYPE_SIGNED,   // 0
    CHAR_TYPE_UNSIGNED, // 1 
    CHAR_TYPE_ARRAY,    // 2
    INT_TYPE_SIGNED,    // 3
    INT_TYPE_UNSIGNED,  // 4
    FLOAT_TYPE,         // 5
    LINKED_LIST,        // 6
    VOID_POINTER        // 7
} typedef dataTypes;

struct LinkedList{ // Struct for the linked list.
    void* data;
    dataTypes dataType; 
    LinkedList* Node; // A pointer to the data type.
}typedef LinkedList;

void nullCheck (void* dynamicAllocatedMemory){
    if (dynamicAllocatedMemory == NULL){
        printf("\n************ MEMORY ALLOCATION FAILED ************\n");
        exit(1);
    }
}

LinkedList* CreateNode(void* data, dataTypes dataType) {
    LinkedList* newNode = (LinkedList*)malloc(sizeof(LinkedList));
    nullCheck(newNode);
    if (dataType == CHAR_TYPE_ARRAY) {
        size_t strLen = strlen((char*)data) + 1; // +1 for the null terminator
        newNode->data = malloc(strLen);
        nullCheck(newNode->data);
        strcpy((char*)newNode->data, (char*)data);
    } 
    else {
        size_t dataSize;
        switch (dataType) {
            case CHAR_TYPE_SIGNED:
                dataSize = sizeof(char);
                break;
            case INT_TYPE_SIGNED:
                dataSize = sizeof(int);
                break;
            case INT_TYPE_UNSIGNED:
                dataSize = sizeof(unsigned int);
                break;
            case FLOAT_TYPE:
                dataSize = sizeof(int);
                break;
            default:
                dataSize = sizeof(void*);
                break;
        }

        newNode->data = malloc(dataSize);
        nullCheck(newNode->data);
        memcpy(newNode->data, data, dataSize);
    }

    newNode->dataType = dataType;
    newNode->Node = NULL;

    return newNode;
}

void printList(LinkedList* List){
    if (List == NULL) {
        printf("List is empty.\n");
        return;
    }
    LinkedList* temp = List;
    while(temp != NULL){
        dataTypes typing = temp->dataType;
        switch (typing){
            case CHAR_TYPE_SIGNED:
                printf("%c -> ", *(char*)temp->data);
                break;
        /*  case CHAR_TYPE_UNSIGNED:
                printf("%x -> ", *(char*)temp->data);
                break;
        */
            case CHAR_TYPE_ARRAY:
                printf("%s -> ", (char*)temp->data);
                break;
            case INT_TYPE_SIGNED:
                printf("%d -> ", *(int*)temp->data);
                break;
            case INT_TYPE_UNSIGNED:
                printf("%u -> ", *(unsigned int*)temp->data);
                break;
            case FLOAT_TYPE: {
                unsafeConversion intToFloat;
                intToFloat.integerType = *(int*)temp->data;
                printf("%f -> ", *(float*)&intToFloat.floatType);
                break;
            }
            default:
                printf("Unknown data type -> ");
                break;
        }

        temp = temp->Node;
    }
    printf("NULLPTR\n");
}

void freeNodes(LinkedList* List){
    while (List != NULL){
        LinkedList* temp = List;
        List = List->Node;
        free(temp->data); // Free allocated data
        free(temp);  
    }
}

int main(int argc, char **argv){
    unsigned int numLoop;
    unsigned int numOfElements;
    dataTypes dataType;
    void* data;
    printf("Input the number of elements you would like in your linked list?\n");
    scanf("%u", &numOfElements);
    numLoop = numOfElements;
    if (numLoop == 0){
        return 0;
    }
    LinkedList* List = NULL;
    LinkedList* tail = NULL;

    while (numLoop != 0){
        LinkedList* newNode = NULL;
        printf("\nPlease input a data type (if incorrectly typed, then undefined behaviour will occur.)\n");
        scanf("%d", &dataType);
        switch (dataType){
            case INT_TYPE_SIGNED:
                int signedInteger;
                printf("Input signed integer:");
                scanf("%d", &signedInteger);
                data = &signedInteger;
                newNode = CreateNode(&signedInteger, INT_TYPE_SIGNED);
                break;
            case INT_TYPE_UNSIGNED:
                unsigned int usignedInteger;
                printf("Input unsigned integer:");
                scanf("%u", &usignedInteger);
                data = &usignedInteger;
                newNode = CreateNode(&usignedInteger, INT_TYPE_UNSIGNED);
                break;
            case CHAR_TYPE_SIGNED:
                char signedChar;
                printf("Input signed character:");
                scanf(" %c", &signedChar);
                data = &signedChar;
                newNode = CreateNode(&signedChar, CHAR_TYPE_SIGNED);
                break;
            case FLOAT_TYPE:
                float floatingPoint;
                printf("Input floating point number:");
                scanf ("%f", &floatingPoint);
                //unsafeConversion floatConvertor;
                //floatConvertor.floatType = floatingPoint;
                //data = &floatConvertor.integerType;
                //newNode = CreateNode(&floatConvertor.integerType, FLOAT_TYPE);
                data = &floatingPoint;
                newNode = CreateNode(&floatingPoint, FLOAT_TYPE);
                break;
            case CHAR_TYPE_ARRAY:
                char* string;
                string = (char*)calloc(BUFFER, sizeof(char));
                nullCheck(string);
                getchar();
                printf("Input string:");
                fgets(string, BUFFER, stdin);
                size_t len = strlen(string);
                if (len > 0 && string[len - 1] == '\n') {
                    string[len - 1] = '\0';
                }
                data = string;
                newNode = CreateNode(string, CHAR_TYPE_ARRAY);
                free(string);
                break;
            default:
                printf("\nINVALID DATA TYPE. Please try again.");
                break;
        }
        if (List == NULL) {
            List = newNode; 
        } 
        else {
            tail->Node = newNode;
        }
        tail = newNode; 
        numLoop--;
    }
    printList(List);
    freeNodes(List);
    return 0;
}