# CarRental-PostfixExEval_2520A2
Data Structures course A2 - 2 programs: (1) Car rental place management system (2) Simple postfix expression evaluator using a stack

*Do not use code from this or copy any aspects without explicit permission from creator*

(Data Structures course Assignment #2)

NOTE: assumed submission should be in a ".tar" file like last time for Assignment 1

To compile, just type "make" to compile everything.

car_data.txt file format:
NOTE: each list is specified by a line with a title (file should always contain them)
--> "AVAILABLE-FOR-RENT", "RENTED", & "IN-REPAIR"
NOTE: each line after a title will be loaded as a car into that respective list
--> format: plate_number,mileage,return_date

Q1: q1.c    - Usage: ./q1
NOTE: assumption made that you will not try to add a new car that has the same plate number as another car
NOTE: this will be a general description, see comments within program for more detailed flow
NOTE: sorry, short on time, so most code is in main()... could have been more modular, but oh well
/**
 * - Runs a management system for a car rental place_holder
 * - Contains 3 lists of cars: available-for-rent, rented, repair
 * - uses a menu for the user to select which option to complete
 * NOTE: assume that each car has a unique plate number
 *   --> user will not try to add a new car with the same plate number as another car
 *
 * NOTE: displays appropriate error msg upon bad input, if something is empty, and if a searched plate number is not in the list
 *   --> plate numbers are 7 characters or less
 *   --> mileage is max 10-digits
 *   --> return date integer format: YYMMDD
 * NOTE: displays a transaction msg after 1 of thee 7 options is chosen and finishes it's functionality
 * --> 1: prompts user for plate number, then mileage, then adds at to the available_list
 * --> 2: prompts user for plate number, then mileage, then searches for the plate number and removes that car from rented_list, adds it to available_list with updated mileage, also displays charge for rental car usage based on mileage added
 * --> 3: prompts user for plate number, then mileage, then searches for the plate number and removes that car from rented_list, adds it to repair_list with updated mileage, also displays charge for rental car usage based on mileage added
 * --> 4: prompts user for plate number, then searches for the plate number and removes that car from reapir_list, adds it to available_list
 * --> 5: prompts user for return date, removes the top car (lowest mileage) from available_list, adds it to rented_list with updated return_date
 * --> 6: prints the cars for all 3 lists
 * --> 7: will only end program if '7' is entered
 */

E.g.
- displays welcome msg and loads in data from text file into appropriate lists
- has a menu loop that prompts the user for what they want to do and their 7 options
NOTE: displays appropriate error msg upon bad input, if something is empty, and if a searched plate number is not in the list
    --> plate numbers are 7 characters or less
    --> mileage is max 10-digits
    --> return date integer format: YYMMDD
NOTE: displays a transaction msg after 1 of thee 7 options is chosen and finishes it's functionality
--> 1: prompts user for plate number, then mileage, then adds at to the available_list
--> 2: prompts user for plate number, then mileage, then searches for the plate number and removes that car from rented_list, adds it to available_list with updated mileage, also displays charge for rental car usage based on mileage added
--> 3: prompts user for plate number, then mileage, then searches for the plate number and removes that car from rented_list, adds it to repair_list with updated mileage, also displays charge for rental car usage based on mileage added
--> 4: prompts user for plate number, then searches for the plate number and removes that car from reapir_list, adds it to available_list
--> 5: prompts user for return date, removes the top car (lowest mileage) from available_list, adds it to rented_list with updated return_date
--> 6: prints the cars for all 3 lists
--> 7: writes to the text file to store lists, prints the total_income from all returned cars this run of the program, then ends program

Q2: q2.c    - Usage: ./q2 <input: String postfix_expression>
NOTE: Input: String postfix_expression - composed of only single digits and binary operators of +, −, ∗, and /
/**
 * Postfix expression evaluator:
 * --> An expression is in postfix form if the operators appear after their operands.
 *   For example, “1 2+” means “1 + 2”, and “1 2 + 5 3− ∗” means “(1 + 2) ∗ (5−3)”.
 * --> Evaluates postfix expressions composed of single digits and binary operators of +, −, ∗, and /,
 *   for addition, subtraction, multiplication, and division respectively.
 * --> Takes an expression as a single command-line argument, with no space in it.
 *   For example: ./q2 12+53-*   ... where q2 is the name of the executable.
 * --> Prints a result that has two digits after the decimal point.
 * NOTE: if a divide by zero occurs anywhere, the program gives a result of "Answer = ndefined --> cannot dive by zero" and terminates
 * NOTE: if pop function is called when the stack is empty, it will return an assumed default of zero (shouldn't occur due to input checking though)
 * NOTE: will display an error msg if invalid characters are found in the postfix expression string
 * NOTE: will only work if there is 1 less operator compared to operands in the postfix expression
 *       --> it will display an error msg and terminate if this happens
 *
 * If it's a digit, add it to the top of the stack, where as if it's an operator,
 *   pop top two elements, placing the first popped element on the RS of operator and the second on the LS.
 * Then, execute the operation and add the result back on top of the stack.
 * At the end, there should be only one value left in the stack, and it is the result/answer of the postfix expression
 */
E.g.
Input:  ./q2 12+53-*
Output: Answer = 6.00
NOTE: if a divide by zero occurs anywhere, the program gives a result of "Answer = undefined --> cannot dive by zero" and terminates
NOTE: if pop function is called when the stack is empty, it will return an assumed default of zero (shouldn't occur due to input checking though)
NOTE: will display an error msg if invalid characters are found in the postfix expression string
NOTE: will only work if there is 1 less operator compared to operands in the postfix expression
       --> it will display an error msg and terminate if this happens
