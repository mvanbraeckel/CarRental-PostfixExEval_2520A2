/*
 * @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * @version 13/10/2018
 * CIS2520 Assignment 2
 * q2.c
    Postfix expression evaluator:
    --> An expression is in postfix form if the operators appear after their operands.
      For example, “1 2+” means “1 + 2”, and “1 2 + 5 3− ∗” means “(1 + 2) ∗ (5−3)”.
    --> Evaluates postfix expressions composed of single digits and binary operators of +, −, ∗, and /,
      for addition, subtraction, multiplication, and division respectively.
    --> Takes an expression as a single command-line argument, with no space in it.
      For example: ./q2 12+53-*   ... where q2 is the name of the executable.
    --> Prints a result that has two digits after the decimal point.
    NOTE: if a divide by zero occurs anywhere, the program gives a result of "Answer = ndefined --> cannot dive by zero" and terminates
    NOTE: if pop function is called when the stack is empty, it will return an assumed default of zero (shouldn't occur due to input checking though)
    NOTE: will display an error msg if invalid characters are found in the postfix expression string
    NOTE: will only work if there is 1 less operator compared to operands in the postfix expression
            --> it will display an error msg and terminate if this happens

    If it's a digit, add it to the top of the stack, where as if it's an operator,
        pop top two elements, placing the first popped element on the RS of operator and the second on the LS.
    Then, execute the operation and add the result back on top of the stack.
    At the end, there should be only one value left in the stack, and it is the result/answer of the postfix expression
 */

// =========================== INCLUDED LIBRARIES ===========================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =========================== STRUCT DECLARATIONS ==========================
typedef struct linked_list_stack_struct {
    double n;
    struct linked_list_stack_struct *next;
} Node;

// =========================== FUNCTION PROTOTYPES ==========================
void push(Node** top, double n);
double pop(Node** top);
void free_stack(Node **top);

int main(int argc, char **argv) {
    // checks that the correct #of command line arguments have been entered
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <input: String postfix_expression_with_single_digits_no_spaces>\n", argv[0]);
        exit(-1);
    }

    // declare variables
    char *string = argv[1]; //rename postfix expression string input
    Node *stack = NULL;
    double num1, num2, temp;
    char currChar;
    int numOperands = 0;
    int numOperators = 0;

    // loops through the inputted postfix expression string, retrieving each significant char
    for(int i = 0; i < strlen(string); i++) {

        currChar = string[i];
        /* Push digits onto the top of the stack
            For operators, pop top two elements and apply the operator (pop_2 <opeartor> pop_top),
            then push that result onto the top of the stack */
        if(currChar >= '0' && currChar <= '9') {
            numOperands++;
            push( &stack, (double)(currChar-'0') );

        } else if( currChar == '+' || currChar == '-' || currChar == '*' || currChar == '/' ) {
            numOperators++;
            // pops top 2 nodes
            num1 = pop(&stack);
            num2 = pop(&stack);

            // check which operator specifically and calculate the result
            if(currChar == '+') {
                temp = num2 + num1;

            } else if(currChar == '-') {
                temp = num2 - num1;

            } else if(currChar == '*') {
                temp = num2 * num1;

            } else { //(currChar == '/')
                // checks for divide by 0 error
                if(num1 == 0) {
                    printf("Answer = undefined --> cannot divide by zero\n");
                    free_stack(&stack);
                    exit(-1);

                } else {
                    temp = num2 / num1;
                }
            }
            push(&stack, temp); //pushes the result back onto the top of the stack

        } else {
            // display error msg because character is invalid
            fprintf(stderr, "Invalid input character <%c>: all characters must be a digit [0,9], +, -, *, or /\n", currChar);
            free_stack(&stack);
            exit(-1);
        }
    } //end for-loop

    // check that the postfix expression was valid
    if(numOperands - numOperators == 1) {
        // display result to user
        double result = pop(&stack);
        printf("Answer = %.2lf\n", result);

        free_stack(&stack); //for good measure (even tho it should be empty)
        return 0;           //program would end here
    }

    // otherwise, invalid because there must be 1 less operator than operands, so display error msg
    if(numOperands > numOperators) {
        fprintf(stderr, "Invalid postfix expression: there must be less operator than operands," \
                        "you have %d more operand(s) than operators\n", numOperands - numOperators - 1);

    } else if(numOperands < numOperators) {
        fprintf(stderr, "Invalid postfix expression: there must be less operator than operands," \
                        "you have %d more operator(s) than operands\n", (numOperands - numOperators) *(-1) );

    } else { // equal
        fprintf(stderr, "Invalid postfix expression: there must be less operator than operands," \
                        "you have an equal number of operators and operand\n");
    }
    free_stack(&stack);
    exit(-1);   //otherwise, program terminates here
}

// =============================== FUNCTIONS ================================

/**
 * Adds a node to the top of the stack
 * @param **top -ptr to the ptr of the linked list stack's head
 * @param n -value to be stored by the new node
 */
void push(Node** top, double n){
    Node* new = malloc(sizeof(Node));   //creates node

    // make sure memory allocation succeeded
    if(!new) {
        fprintf(stderr, "ERROR: memory allocation in push function failed\n");
        exit(-1);
    }
    // links new node to the top of the stack
    new->n = n;
    new->next = *top;
    *top = new; //resets top node
}

/**
 * Removes the top node from the stack and gets its value
 * NOTE: assumes a return 0 if the stack is empty
 * @param **top -ptr to the ptr of the linked list stack's head
 * @return Only the stored value of the removed node (not the node)
 */
double pop(Node** top){
    // assume 0 when nothing is in the stack
    if(*top == NULL) {
        return 0;
    }

    Node* temp = *top;  //holds top node
    *top = temp->next;  //resets top node
    double n = temp->n; //holds value of node to be removed

    free(temp);
    return n;
}

/**
 * Frees all nodes in the stack
 */
void free_stack(Node **top) {
    Node *temp;
    while(*top != NULL) {
        temp = *top;        //holds top node
        *top = (*top)->next;//resets top node
        free(temp);
    }
}
