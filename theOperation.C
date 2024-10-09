#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define MAX_LENGTH 100
#define MAX_LINES 100


void infixToPostfix(char* infix, char* postfix);
void infixToPrefix(char* infix, char* prefix);
void postfixToInfix(char* postfix, char* infix);
void prefixToInfix(char* prefix, char* infix);
void postfixToPrefix(char* postfix, char* prefix);
void prefixToPostfix(char* prefix, char* postfix);
int evaluateInfix(char* infix);
int evaluatePostfix(char* postfix);
int evaluatePrefix(char* prefix);
int validateExpression(char* expression);





int main() {

    char expressions[MAX_LINES][MAX_LENGTH];
    char expression[MAX_LENGTH]; 
    char converted[MAX_LENGTH];
    int choice, line, result, res;
    FILE *file;

    file = fopen("example.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    int numLines = 0;
    while (fgets(expressions[numLines], MAX_LENGTH, file) != NULL && numLines < MAX_LINES) {
        expressions[numLines][strcspn(expressions[numLines], "\n")] = 0; 
        numLines++;
    }
    fclose(file);


    printf("--------- main menu ---------\n");
    printf("1.  convert Infix to Postfix\n");
    printf("2.  convert Infix to Prefix\n");
    printf("3.  convert Postfix to Infix\n");
    printf("4.  convert Prefix to Infix\n");
    printf("5.  convert Postfix to Prefix\n");
    printf("6.  Convert Prefix to Postfix\n");
    printf("7.  Expression evaluation Infix\n");
    printf("8.  Expression evaluation Postfix\n");
    printf("9.  Expression evaluation Prefix\n");
    printf("10. Expression validation\n");
    printf("11. Exit\n");
    printf("-----------------------------\n");

    while (1) {
        printf("Enter the line number (1-%d) you want to process or 0 to exit: ", numLines);
        scanf("%d", &line);
        getchar(); // to read newline after the number

        if (line == 0) break;
        if (line < 1 || line > numLines) {
            printf("Invalid line number!\n");
            continue;
        }

        strcpy(expression, expressions[line - 1]);

        printf("Selected expression: %s\n", expression);
        printf("Please select an option: ");
        scanf("%d", &choice);
        getchar(); // to read newline after the number

    switch (choice) {
        case 1:
            
            infixToPostfix(expression, converted);
            printf("Postfix: %s\n", converted);
            break;
        case 2:
            infixToPrefix(expression, converted);
            printf("Prefix: %s\n", converted);
            break;
        case 3:
            postfixToInfix(expression, converted);
            printf("Infix: %s\n", converted);
            break;
        case 4:
            prefixToInfix(expression, converted);
            printf("Infix: %s\n", converted);
            break;
        case 5:
            postfixToPrefix(expression, converted);
            printf("Prefix: %s\n", converted);
            break;
        case 6:
            prefixToPostfix(expression, converted);
            printf("Postfix: %s\n", converted);
            break;
        case 7:
            res = validateExpression(expression);
            if(res!=1){
                printf("This expression is not infix\n");
                break;}
            result = evaluateInfix(expression);
            printf("Result: %d\n", result);
            break;
        case 8:
            res = validateExpression(expression);
            if(res!=2){
                printf("This expression is not postfix\n");
                break;}
            result = evaluatePostfix(expression);
            printf("Result: %d\n", result);
            break;
        case 9:
            res = validateExpression(expression);
            if(res!=3){
                printf("This expression is not prefix\n");
                break;}
            result = evaluatePrefix(expression);
            printf("Result: %d\n", result);
            break;
        case 10:
            result = validateExpression(expression);
            if (result == 1)
                printf("Infix\n");
            else if (result == 2)
                printf("Postfix\n");
            else if (result==3)
                printf("Prefix\n");
            else
                printf("The expression type was not recognized\n");
            break;
        case 11:
            printf("Exit\n");
            return 0;
        default:
            printf("The option is invalid\n");
    }
}
    return 0;

}


//******************-------*******************------*******************------*****************

// Helper functions 

int isOpertor(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

typedef struct Node {
    char data;
    struct Node* next;
} Node;

typedef struct {
    Node* top;
} Stack;



char peek(Stack* stack) {
    if (stack->top == NULL) {
        printf("Stack is empty\n");
        return -1;
    }
    return stack->top->data;
}


void push(Node** top, char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

char pop(Node** top) {
    if (*top == NULL) {
        return -1;
    }
    Node* temp = *top;
    char data = temp->data;
    *top = temp->next;
    free(temp);
    return data;
}

int isStackEmpty(Node* top) {
    return top == NULL;
}

int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
    }
    return -1;
}


//******************-------*******************------*******************------*****************

void infixToPostfix(char* infix, char* postfix) {
    Node* stack = NULL;
    int j = 0;
    for (int i = 0; infix[i]; ++i) {
        char c = infix[i];

        // If operand, add to output
        if (isalnum(c)) {
            postfix[j++] = c;
        }
        // If '(', push to stack
        else if (c == '(') {
            push(&stack, c);
        }
        // If ')', pop until '('
        else if (c == ')') {
            while (!isStackEmpty(stack) && stack->data != '(') {
                postfix[j++] = pop(&stack);
            }
            pop(&stack); // Remove '('
        }
        // If operator
        else {
            while (!isStackEmpty(stack) && precedence(c) <= precedence(stack->data)) {
                postfix[j++] = pop(&stack);
            }
            push(&stack, c);
        }
    }

    // Pop all remaining operators
    while (!isStackEmpty(stack)) {
        postfix[j++] = pop(&stack);
    }
    postfix[j] = '\0';
}



//******************-------*******************------*******************------*****************

void reverseString(char* str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}


void reverseStringAndReplaceParentheses(char* str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        // Swap characters
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;

        // Replace parentheses when swapping
        if (str[i] == '(') {
            str[i] = ')';
        } else if (str[i] == ')') {
            str[i] = '(';
        }

        if (str[n - i - 1] == '(') {
            str[n - i - 1] = ')';
        } else if (str[n - i - 1] == ')') {
            str[n - i - 1] = '(';
        }
    }

    // If odd length, replace middle character's parentheses
    if (n % 2 != 0) {
        if (str[n / 2] == '(') {
            str[n / 2] = ')';
        } else if (str[n / 2] == ')') {
            str[n / 2] = '(';
        }
    }
}


void infixToPrefix(char* infix, char* prefix) {
    reverseStringAndReplaceParentheses(infix);
    char tempPostfix[100]; 
    infixToPostfix(infix, tempPostfix);
    reverseString(tempPostfix);
    strcpy(prefix, tempPostfix);
}

//******************-------*******************------*******************------*****************


char* createNewString(const char* str1, const char* op, const char* str2) {
    char* result = (char*)malloc(MAX_LENGTH);
    sprintf(result, "(%s %s %s)", str1, op, str2);
    return result;
}



void postfixToInfix(char* postfix, char* infix) {
    char* stack[MAX_LENGTH];
    int top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        if (isOpertor(c)) {
            char* op2 = stack[top--];
            char* op1 = stack[top--];
            char op[2] = {c, '\0'};

            stack[++top] = createNewString(op1, op, op2);

            free(op1);
            free(op2);
        } else {
            char* operand = (char*)malloc(2);
            operand[0] = c;
            operand[1] = '\0';
            stack[++top] = operand;
        }
    }

    strcpy(infix, stack[top--]);
    free(stack[top + 1]);
}





//******************-------*******************------*******************------*****************


void pushh(char* stack[], int* top, char* str) {
    stack[++(*top)] = strdup(str);
}

char* poop(char* stack[], int* top) {
    if (*top == -1) return "";
    return stack[(*top)--];
}

void prefixToInfix(char* prefix, char* infix) {
    char* stack[MAX_LENGTH];
    int top = -1;

    int length = strlen(prefix);
    for (int i = length - 1; i >= 0; i--) {
        if (isOpertor(prefix[i])) {
            char* op1 = poop(stack, &top);
            char* op2 = poop(stack, &top);

            char temp[MAX_LENGTH];
            snprintf(temp, sizeof(temp), "(%s %c %s)", op1, prefix[i], op2);
            pushh(stack, &top, temp);

            free(op1);
            free(op2);
        } else if (isalnum(prefix[i])) {
            char operand[2] = {prefix[i], '\0'};
            pushh(stack, &top, operand);
        }
    }

    strcpy(infix, poop(stack, &top));
    free(stack[top + 1]);
}




//******************-------*******************------*******************------*****************

char* createNewString2(const char* op, const char* str1, const char* str2) {
    char* result = (char*)malloc(MAX_LENGTH);
    sprintf(result, "%s%s%s", op, str1, str2);
    return result;
}


void postfixToPrefix(char* postfix, char* prefix) {
    char* stack[MAX_LENGTH];
    int top = -1;

    int length = strlen(postfix);
    for (int i = 0; i < length; i++) {
        char c = postfix[i];

        if (isOpertor(c)) {
            char* op2 = stack[top--];
            char* op1 = stack[top--];
            char op[2] = {c, '\0'};

            stack[++top] = createNewString2(op, op1, op2);

            free(op1);
            free(op2);
        } else {
            char* operand = (char*)malloc(2);
            operand[0] = c;
            operand[1] = '\0';
            stack[++top] = operand;
        }
    }

    strcpy(prefix, stack[top--]);
    free(stack[top + 1]);
}







//******************-------*******************------*******************------*****************

void prefixToPostfix(char* prefix, char* postfix) {
    char* stack[MAX_LENGTH];
    int top = -1;

    int length = strlen(prefix);
    for (int i = length - 1; i >= 0; i--) {
        if (isOpertor(prefix[i])) {
            char* op1 = poop(stack, &top);
            char* op2 = poop(stack, &top);

            char temp[MAX_LENGTH];
            snprintf(temp, sizeof(temp), "%s%s%c", op1, op2, prefix[i]);
            pushh(stack, &top, temp);

            free(op1);
            free(op2);
        } else if (isalnum(prefix[i])) {
            char operand[2] = {prefix[i], '\0'};
            pushh(stack, &top, operand);
        }
    }

    strcpy(postfix, poop(stack, &top));
    free(stack[top + 1]);
}





//******************-------*******************------*******************------*****************

int isOperandd(char c) {
    return isdigit(c) || isalpha(c);
}



int validateExpression(char* expression) {
    int length = strlen(expression);

    
    if (strchr(expression, '(') || strchr(expression, ')') || (isOperandd(expression[0]) && isOperandd(expression[length - 1]))) {
        return 1; // Infix
    }
    
    else if (isOpertor(expression[length - 1])) {
        return 2; // Postfix
    }
    
    else if (isOpertor(expression[0])) {
        return 3; // Prefix
    }
    else {
        return 0; 
    }
}





//******************-------*******************------*******************------*****************

int applyOp2(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return (int)pow(a, b); // اضافه کردن عملگر توان
    }
    return 0;
}

int evaluatePostfix(char* postfix) {
    int stack[MAX_LENGTH];
    int top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {
        if (isdigit(postfix[i])) {
            stack[++top] = postfix[i] - '0';
        } else if (isOpertor(postfix[i])) {
            int val2 = stack[top--];
            int val1 = stack[top--];
            stack[++top] = applyOp2(val1, val2, postfix[i]);
        }
    }
    return stack[top];
}


//******************-------*******************------*******************------*****************

int evaluateInfix(char* infix) {
    char postfix[MAX_LENGTH];
    infixToPostfix(infix, postfix);
    return evaluatePostfix(postfix);
}



//******************-------*******************------*******************------*****************

int evaluatePrefix(char* prefix) {
    char postfix[MAX_LENGTH];
    prefixToPostfix(prefix, postfix);
    return evaluatePostfix(postfix);
}