/*
 * @author Mitchell Van Braeckel (mvanbrae@uoguelph.ca) 1002297
 * @version 13/10/2018
 * CIS2520 Assignment 2
 * q1.c
    - Runs a management system for a car rental place_holder
    - Contains 3 lists of cars: available-for-rent, rented, repair
    - uses a menu for the user to select which option to complete
    NOTE: assume that each car has a unique plate number
        --> user will not try to add a new car with the same plate number as another car

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
    --> 7: will only end program if '7' is entered
 */

// =========================== INCLUDED LIBRARIES ===========================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// =========================== STRUCT DECLARATIONS ==========================

typedef struct car_struct {
    char plate[8];
    int mileage;
    int return_date;    // format "yymmdd" assumed year of 2000s (21st century)
} Car;

typedef struct linked_list_stack_struct {
    Car car;
    struct linked_list_stack_struct *next;
} Node;

// =========================== FUNCTION PROTOTYPES ==========================
void push_available(Node* to_add);
void push_rented(Node* to_add);
void push_repair(Node *to_add);
Node* pop_top_available();
int pop_rented(char *plate_number, int *mileage);
int pop_repair(char *plate_number, int *mileage);

void free_lists();
void print_lists();
void print_available();
void print_rented();
void print_repair();

void write_text(char *filename);
void read_text(char *filename);

void flush_input(char *input);
int isNumber(char *input);
int isWhitespace(char *input);

// ============================ GLOBAL VARIABLES ============================
// declare the 3 linked lists
Node *available_list = NULL;
Node *rented_list = NULL;
Node *repair_list = NULL;

int main(int argc, char **argv) {
    // declare variables
    char menu_input[3];
    char plate_input[9];
    char mileage_input[12];     //10-digit number for mileage
    char return_date_input[8];  //6-digit number of "yymmdd"
    double return_charge = 0;
    double total_income = 0;

    // booleans
    int goodInput = 0;

    read_text("car_data.txt");
    printf("\nWelcome to Mitch's Car Rental Place Management System!\n\n");
    // infinite menu loop, only ending program with '7' as input
    while(1) {
        printf("What would you like to do?\n(Please enter the number of the management option you would like to use)\n\n" \
                "\t1. Add a new car to the available-for-rent list\n" \
                "\t2. Add a returned car to the available-for-rent list\n" \
                "\t3. Add a returned car to the repair list\n" \
                "\t4. Transfer a car from the repair list to the available-for-rent list\n" \
                "\t5. Rent the first available car\n" \
                "\t6. Print all the lists\n" \
                "\t7. Quit.\n\n" \
                "Select option: ");
        fgets(menu_input, 3, stdin);    //only need the first character of input for menu option
        flush_input(menu_input);

        // checks user input
        if(strlen(menu_input) > 1) {
            // display error msg inputting more than just the number option
            printf("\nError: invald input - You must choose one of the menu options by number: [1,7]\nPlease try again\n\n");

        }
        else if(menu_input[0] == '1') {         // ================================= 1 =================================
            // adds a new car to the available_list
            Node *new_car = malloc(sizeof(Node));
            // make sure memory allocation succeeded
            if(!new_car) {
                fprintf(stderr, "\n1: Add new car, malloc messed up\n\n");
                exit(-1);
            }

            // prompt user for plate number
            goodInput = 0;
            while(!goodInput) {
                printf("Enter plate number: ");
                fgets(plate_input, 9, stdin);
                flush_input(plate_input);

                // checks input
                if(strlen(plate_input) > 7) {
                    printf("\nError: Invalid plate number - must be 7 characters or less\n\n");   //display err msg
                } else if(isWhitespace(plate_input)) {
                    printf("\nError: Invalid plate number - please try again\n\n");   //display err msg -only whitespace chars
                } else {
                    goodInput = 1;
                }
            }
            // store plate number in the new car
            strncpy(new_car->car.plate, plate_input, 7);
            new_car->car.plate[strlen(new_car->car.plate)] = '\0';

            // prompt user for mileage
            goodInput = 0;
            while(!goodInput) {
                printf("Enter mileage: ");
                fgets(mileage_input, 12, stdin);
                flush_input(mileage_input);

                // checks input
                if(strlen(mileage_input) > 10) {
                    printf("\nError: Invalid mileage - max of 10 digits\n\n");   //display err msg -string too long
                } else {
                    strncpy(mileage_input, mileage_input, 10);  //gets it to correct size string
                    mileage_input[strlen(mileage_input)] = '\0';
                    if(isNumber(mileage_input)) {
                            goodInput = 1;
                    } else {
                        printf("\nError: Invalid mileage - must enter a non-negative number\n\n");   //display err msg -contained non-digits
                    }
                }
            }
            // store mileage in the new car
            new_car->car.mileage = atoi(mileage_input);

            new_car->car.return_date = 0;
            // add the new car to be available-for-rent
            push_available(new_car);

            // print transaction statement
            printf("\nTransaction Option 1: You have just added a car to the Available-for-Rent list\n\n");

        }
        else if(menu_input[0] == '2') {         // ================================= 2 =================================
            // removes returned car from rented_list identifying by plate, then adds it to available_list
            // skip if rented list is empty
            if(rented_list == NULL) {
                printf("\nSorry, there are no cars rented out right now.\n\n");
                continue;
            }

            // declare variables
            Node *returned_car = NULL;

            // prompt user for plate number
            goodInput = 0;
            while(!goodInput) {
                printf("Enter plate number: ");
                fgets(plate_input, 9, stdin);
                flush_input(plate_input);

                // checks input
                if(strlen(plate_input) > 7) {
                    printf("\nError: Invalid plate number - must be 7 characters or less\n\n");   //display err msg
                } else if(isWhitespace(plate_input)) {
                    printf("\nError: Invalid plate number - please try again\n\n");   //display err msg -only whitespace chars
                } else {
                    goodInput = 1;
                }
            }
            // get it correct size
            strncpy(plate_input, plate_input, 7);

            // find matching car's plate number, get it's mileage, and remove it from rented list
            int prev_mileage;
            if(!pop_rented(plate_input, &prev_mileage)) {
                printf("\nSorry, a car with that plate number is not being rented out right now.\n\n");
                continue;
            } else {
                // create and init the new instance of the returned car's attributes
                returned_car = malloc(sizeof(Node));
                strcpy(returned_car->car.plate, plate_input);
                returned_car->car.mileage = prev_mileage;
            }

            // prompt user for new mileage
            goodInput = 0;
            while(!goodInput) {
                printf("Enter the new mileage: ");
                fgets(mileage_input, 12, stdin);
                flush_input(mileage_input);

                // checks input
                if(strlen(mileage_input) > 10) {
                    printf("\nError: Invalid mileage - max of 10 digits\n\n");   //display err msg -string too long
                } else {
                    strncpy(mileage_input, mileage_input, 10);  //gets it to correct size string
                    mileage_input[strlen(mileage_input)] = '\0';

                    if(isNumber(mileage_input)) {
                        // must be more or same mileage
                        if(atoi(mileage_input) < returned_car->car.mileage) {
                            printf("\nError: Invalid mileage - the mileage can't be less thatn when the car was rented out.\n\n");
                        } else {
                            goodInput = 1;
                        }
                    } else {
                        printf("\nError: Invalid mileage - must enter a non-negative number\n\n");   //display err msg -contained non-digits
                    }
                }
            }
            // store mileage in the returned car
            returned_car->car.mileage = atoi(mileage_input);

            returned_car->car.return_date = 0;
            // add the returned car to be available-for-rent
            push_available(returned_car);

            // print transaction statement
            printf("\nTransaction Option 2: Since a car was just returned, " \
                    "you have removed it from the Rented list and put it in the Available-for-Rent list\n");

            // calculate return charge
            return_charge = 40.00;  //flat-rate for up to 100km
            if(returned_car->car.mileage - prev_mileage > 100) {
                return_charge += (returned_car->car.mileage - prev_mileage - 100) * 0.15;  //15 cents per additional kilometer over 100km
            }
            total_income += return_charge;  //accumulate total income
            // display cost charge for rental
            printf("\nYou have charged the renter $%.2lf for their use of the car.\n\n", return_charge);

        }
        else if(menu_input[0] == '3') {         // ================================= 3 =================================
            // removes returned car from rented_list identifying by plate, then adds it to repair_list
            // skip if rented list is empty
            if(rented_list == NULL) {
                printf("\nSorry, there are no cars rented out right now.\n\n");
                continue;
            }

            // declare variables
            Node *returned_car = NULL;

            // prompt user for plate number
            goodInput = 0;
            while(!goodInput) {
                printf("Enter plate number: ");
                fgets(plate_input, 9, stdin);
                flush_input(plate_input);

                // checks input
                if(strlen(plate_input) > 7) {
                    printf("\nError: Invalid plate number - must be 7 characters or less\n\n");   //display err msg
                } else if(isWhitespace(plate_input)) {
                    printf("\nError: Invalid plate number - please try again\n\n");   //display err msg -only whitespace chars
                } else {
                    goodInput = 1;
                }
            }
            // get it correct size
            strncpy(plate_input, plate_input, 7);

            // find matching car's plate number, get it's mileage, and remove it from rented list
            int prev_mileage;
            if(!pop_rented(plate_input, &prev_mileage)) {
                printf("\nSorry, a car with that plate number is not being rented out right now.\n\n");
                continue;
            } else {
                // create and init the new instance of the returned car's attributes
                returned_car = malloc(sizeof(Node));
                strcpy(returned_car->car.plate, plate_input);
                returned_car->car.mileage = prev_mileage;
            }

            // prompt user for new mileage
            goodInput = 0;
            while(!goodInput) {
                printf("Enter the new mileage: ");
                fgets(mileage_input, 12, stdin);
                flush_input(mileage_input);

                // checks input
                if(strlen(mileage_input) > 10) {
                    printf("\nError: Invalid mileage - max of 10 digits\n\n");   //display err msg -string too long
                } else {
                    strncpy(mileage_input, mileage_input, 10);  //gets it to correct size string
                    mileage_input[strlen(mileage_input)] = '\0';

                    if(isNumber(mileage_input)) {
                        // must be more or same mileage
                        if(atoi(mileage_input) < returned_car->car.mileage) {
                            printf("\nError: Invalid mileage - the mileage can't be less thatn when the car was rented out.\n\n");
                        } else {
                            goodInput = 1;
                        }
                    } else {
                        printf("\nError: Invalid mileage - must enter a non-negative number\n\n");   //display err msg -contained non-digits
                    }
                }
            }
            // store mileage in the returned car
            returned_car->car.mileage = atoi(mileage_input);

            returned_car->car.return_date = 0;
            // add the car to be repaired
            push_repair(returned_car);

            // print transaction statement
            printf("\nTransaction Option 3: Since a car was just returned and needs repairs, " \
                    "you have removed it from the Rented list and put it in the In Repair list\n");

            // calculate return charge
            return_charge = 40.00;  //flat-rate for up to 100km
            if(returned_car->car.mileage - prev_mileage > 100) {
                return_charge += (returned_car->car.mileage - prev_mileage - 100) * 0.15;  //15 cents per additional kilometer over 100km
            }
            total_income += return_charge;  //accumulate total income
            // display cost charge for rental
            printf("\nYou have charged the renter $%.2lf for their use of the car.\n\n", return_charge);
        }
        else if(menu_input[0] == '4') {         // ================================= 4 =================================
            // transfers a car from repair_list to be available-for-rent list
            // skip if repair list is empty
            if(repair_list == NULL) {
                printf("\nSorry, there are no cars being repaired right now.\n\n");
                continue;
            }

            // declare variables
            Node *repaired_car = NULL;

            // prompt user for plate number
            goodInput = 0;
            while(!goodInput) {
                printf("Enter plate number: ");
                fgets(plate_input, 9, stdin);
                flush_input(plate_input);

                // checks input
                if(strlen(plate_input) > 7) {
                    printf("\nError: Invalid plate number - must be 7 characters or less\n\n");   //display err msg
                } else if(isWhitespace(plate_input)) {
                    printf("\nError: Invalid plate number - please try again\n\n");   //display err msg -only whitespace chars
                } else {
                    goodInput = 1;
                }
            }
            // get it correct size
            strncpy(plate_input, plate_input, 7);

            // find matching car's plate number, get it's mileage, and remove it from rented list
            int prev_mileage;
            if(!pop_repair(plate_input, &prev_mileage)) {
                printf("\nSorry, a car with that plate number is not being repaired right now.\n\n");
                continue;
            } else {
                // create and init the new instance of the returned car's attributes
                repaired_car = malloc(sizeof(Node));
                strcpy(repaired_car->car.plate, plate_input);
                repaired_car->car.mileage = prev_mileage;
            }

            // store mileage in the returned car
            repaired_car->car.mileage = atoi(mileage_input);
            repaired_car->car.return_date = 0;  //just in case
            // add the repaired car to be available-for-rent
            push_available(repaired_car);

            // print transaction statement
            printf("\nTransaction Option 4: Since a car has just finished being repaired, " \
                    "you have removed it from the In Repair list and put it in the Available-for-Rent list\n\n");

        }
        else if(menu_input[0] == '5') {         // ================================= 5 =================================
            /* rents the first available car --> removes the top car from available_list,
                gets user input for expected return date, adds it to rented_list sorted by earliest return_date */
            // skip if available list is empty
            if(available_list == NULL) {
                printf("\nSorry, there are no cars availale for rent right now.\n\n");
                continue;
            }

            // get the first car from the available_list
            Node *top_car = pop_top_available();

            // prompt user for expected return date
            goodInput = 0;
            while(!goodInput) {
                printf("Enter the expected return date (Format = YYMMDD): ");
                fgets(return_date_input, 8, stdin);
                flush_input(return_date_input);

                // checks input
                if(strlen(return_date_input) > 6) {
                    printf("\nError: Invalid return date - expected return date must be entered in format 'YYMMDD'\n\n");   //display err msg -string too long
                } else {
                    strncpy(return_date_input, return_date_input, 6);  //gets it to correct size string
                    return_date_input[strlen(return_date_input)] = '\0';

                    if(isNumber(return_date_input) && strlen(return_date_input) == 6) {
                        goodInput = 1;
                    } else {
                        printf("\nError: Invalid return date - expected return date must be entered in format 'YYMMDD'\n\n");   //display err msg -contained non-digits
                    }
                }
            }
            // store the expected return date in the car being rented
            top_car->car.return_date = atoi(return_date_input);
            // add the car being rented to the rented_list
            push_rented(top_car);

            // print transaction statement
            printf("\nTransaction Option 5: Since the car with lowest mileage has just been rented, " \
                    "you have removed it from the Available-for-Rent list and put it in the Rented list\n\n");

        }
        else if(menu_input[0] == '6') {         // ================================= 6 =================================
            print_lists();

            // print transaction statement
            printf("\nTransaction Option 6: You have just printed all the cars in each list\n\n");

        }
        else if(menu_input[0] == '7') {         // ================================= 7 =================================
            // display total income from returned cars
            printf("\nYou have earned a total income of $%.2lf for charging people who have returned cars today.\n\n", total_income);
            // print transaction statement
            printf("\nTransaction Option 7: You have just ended the management program\n\n");
            printf("\nThank you and have a nice day!\n\n");

            // write all lists to a file while freeing each element of each list
            write_text("car_data.txt");
            return 0;   //end program successfully

        }
        else {                                  // =============================== ELSE ================================
            // display error msg for bad input
            printf("\nError: invald input - You must choose one of the menu options by number: [1,7]\nPlease try again\n\n");
        }
    }
}

// ========================== PUSH & POP FUNCTIONS ==========================

/**
 * Adds a car to the available-for-rent list (inserted in ascending order using mileage)
 * @param Node *to_add -the car to be added to the list
 */
void push_available(Node* to_add) {
    // adds car to the available-for-rent list sorted by mileage (ascending order)
    Node *curr = available_list;
    Node *prev;

    if(available_list == NULL) {        //is empty
        to_add->next = available_list;
        available_list = to_add; //reset head
        return;
    }

    // finds where it should be inserted into the list
    while(curr != NULL && to_add->car.mileage > curr->car.mileage) {
        prev = curr; //holds curr, before curr iterates
        curr = curr->next;
    }

    // inserts it into the list
    if(curr == NULL) {                  //last element
        to_add->next = curr;
        prev->next = to_add;
    } else if(curr == available_list) { //first element
        to_add->next = curr;
        available_list = to_add; //reset head
    } else {                            //middle element
        to_add->next = curr;
        prev->next = to_add;
    }
}

/**
 * Adds a car to the rented list (inserted in ascending order using expected return date)
 * @param Node *to_add -the car to be added to the list
 */
void push_rented(Node* to_add) {
    // adds car to the rented list sorted by earliest return date(ascending order)
    Node *curr = rented_list;
    Node *prev;

    if(rented_list == NULL) {   //is empty
        to_add->next = rented_list;
        rented_list = to_add;   //reset head
        return;
    }

    // finds where it should be inserted into the list
    while(curr != NULL && to_add->car.return_date > curr->car.return_date) {
        prev = curr; //holds curr, before curr iterates
        curr = curr->next;
    }

    // inserts it into the list
    if(curr == NULL) {                  //last element
        to_add->next = curr;
        prev->next = to_add;
    } else if(curr == rented_list) { //first element
        to_add->next = curr;
        rented_list = to_add; //reset head
    } else {                            //middle element
        to_add->next = curr;
        prev->next = to_add;
    }
}

/**
 * Adds a car to the repair list
 * @param Node *to_add -the car to be added to the list
 */
void push_repair(Node *to_add) {
    // adds car to the repair list sorted by mileage (ascending order)
    Node *curr = repair_list;
    Node *prev;

    if(repair_list == NULL) {        //is empty
        to_add->next = repair_list;
        repair_list = to_add; //reset head
        return;
    }

    // finds where it should be inserted into the list
    while(curr != NULL && to_add->car.mileage > curr->car.mileage) {
        prev = curr; //holds curr, before curr iterates
        curr = curr->next;
    }

    // inserts it into the list
    if(curr == NULL) {                  //last element
        to_add->next = curr;
        prev->next = to_add;
    } else if(curr == repair_list) { //first element
        to_add->next = curr;
        repair_list = to_add; //reset head
    } else {                            //middle element
        to_add->next = curr;
        prev->next = to_add;
    }
}

/**
 * Retrieves the first Car that is available-for-rent
 * @return The first element of the list
 */
Node* pop_top_available() {
    // is empty
    if(available_list == NULL) {
        printf("\nSorry, there are no cars availale for rent right now.\n\n");
    }

    Node* top_car = available_list;         //holds first car
    available_list = available_list->next;  //resets head
    return top_car;
}

/**
 * Retrieves the mileage of the car with the given plate number and removes it from the rented list
 * NOTE: assumes exact character matching for plates (upper and lowercase distinction)
 * @param char *plate_number -the plate number of the desired car
 * @param int *mileage -pointer to the value the car's mileage will be stored in
 * @return 1 if a match was found
 */
int pop_rented(char *plate_number, int *mileage) {
    Node *curr = rented_list;
    Node *prev;

    // skip if rented list is empty
    if(rented_list == NULL) {
        printf("\nSorry, there are no cars rented out right now.\n\n");
        return 0;
    }

    // find the match
    while(curr != NULL) {
        if(strcmp(plate_number, curr->car.plate) == 0) {
            *mileage = curr->car.mileage; //get mileage

            // remove the node from list
            if(curr == rented_list) {   //first element
                rented_list = rented_list->next; //reset head
                free(curr);
            } else { //middle or last element
                prev->next = curr->next;
                free(curr);
            }
            return 1; //match found
        }
        prev = curr;
        curr = curr->next;
    }
    return 0; //match not found
}

/**
 * Retrieves the mileage of the car with the given plate number and removes it from the repair list
 * NOTE: assumes exact character matching for plates (upper and lowercase distinction)
 * @param char *plate_number -the plate number of the desired car
 * @param int *mileage -pointer to the value the car's mileage will be stored in
 * @return 1 if a match was found
 */
int pop_repair(char *plate_number, int *mileage) {
    Node *curr = repair_list;
    Node *prev;

    // skip if rented list is empty
    if(repair_list == NULL) {
        printf("\nSorry, there are no cars being repaired right now.\n\n");
        return 0;
    }

    // find the match
    while(curr != NULL) {
        if(strcmp(plate_number, curr->car.plate) == 0) {
            *mileage = curr->car.mileage; //get mileage

            // remove the node from list
            if(curr == repair_list) {   //first element
                repair_list = repair_list->next; //reset head
                free(curr);
            } else { //middle or last element
                prev->next = curr->next;
                free(curr);
            }
            return 1; //match found
        }
        prev = curr;
        curr = curr->next;
    }
    return 0; //match not found
}

// ========================= FREE & PRINT FUNCTIONS =========================

/**
 * Frees the 3 linked lists
 */
void free_lists() {
    Node *temp;
    // frees available-for-rent list
    while(available_list != NULL) {
        temp = available_list;        //holds top node
        available_list = available_list->next;//resets top node
        free(temp);
    }
    // frees rented_list
    while(rented_list != NULL) {
        temp = rented_list;        //holds top node
        rented_list = rented_list->next;//resets top node
        free(temp);
    }
    // frees repair_list
    while(repair_list != NULL) {
        temp = repair_list;        //holds top node
        repair_list = repair_list->next;//resets top node
        free(temp);
    }
}

/**
 * Prints all the cars in each list
 */
void print_lists() {
    print_available();
    print_rented();
    print_repair();
}

/**
 * Prints all available cars for rent
 */
void print_available() {
    Node *list = available_list;

    printf("\nAvailable-for-Rent:\n");
    if(available_list == NULL) {        //is empty
        printf("Sorry, there are no cars available for rent.\n\n");
        return;
    }

    while(list != NULL) {
        printf("Plate Number: %7s\tMileage: %10d\tReturn Date: %6d\n",
                list->car.plate, list->car.mileage, list->car.return_date);
        list = list->next;
    }
    printf("\n");
}

/**
 * Prints all cars currently being rented
 */
void print_rented() {
    Node *list = rented_list;

    printf("\nRented:\n");
    if(rented_list == NULL) {        //is empty
        printf("Sorry, there are no cars being rented right now.\n\n");
        return;
    }

    while(list != NULL) {
        printf("Plate Number: %7s\tMileage: %10d\tReturn Date: %6d\n",
                list->car.plate, list->car.mileage, list->car.return_date);
        list = list->next;
    }
    printf("\n");
}

/**
 * Prints all cars currently being repaired
 */
void print_repair() {
    Node *list = repair_list;

    printf("\nIn Repair:\n");
    if(repair_list == NULL) {        //is empty
        printf("Sorry, there are no cars being repaired right now.\n\n");
        return;
    }

    while(list != NULL) {
        printf("Plate Number: %7s\tMileage: %10d\tReturn Date: %6d\n",
                list->car.plate, list->car.mileage, list->car.return_date);
        list = list->next;
    }
    printf("\n");
}

// ============================= FILE FUNCTIONS =============================

/**
 * Writes all 3 linked lists to a file
 * NOTE: format is CSV: plate,mileage,return_date
 */
void write_text(char *filename) {
    Node *temp;

    FILE *fp = fopen(filename, "w");
    if(fp == NULL) {
        fprintf(stderr, "ERROR: File could not be opened\n");

    } else {
        fprintf(fp, "AVAILABLE-FOR-RENT\n");
        while(available_list) {
            temp = available_list;
            fprintf(fp, "%s,%d,%d\n", temp->car.plate, temp->car.mileage, temp->car.return_date);
            available_list = available_list->next;
            free(temp);
        }
        fprintf(fp, "RENTED\n");
        while(rented_list) {
            temp = rented_list;
            fprintf(fp, "%s,%d,%d\n", temp->car.plate, temp->car.mileage, temp->car.return_date);
            rented_list = rented_list->next;
            free(temp);
        }
        fprintf(fp, "IN-REPAIR\n");
        while(repair_list) {
            temp = repair_list;
            fprintf(fp, "%s,%d,%d\n", temp->car.plate, temp->car.mileage, temp->car.return_date);
            repair_list = repair_list->next;
            free(temp);
        }
    }
    fclose(fp);
}

/**
 * Reads the data for all 3 linked lists and loads it into respective lists
 * NOTE: format is CSV: plate,mileage,return_date
 */
void read_text(char *filename) {
    FILE *fp = fopen(filename, "r");
    Node *new_car = NULL;
    // checks if fopen messed up
    if(fp == NULL) {
        fprintf(stderr, "ERROR: File could not be opened\n");

    } else {
        char buffer[30];
        int list_num = 0;
        // read until the end of the file -- if something is read, will return non-zero
        while(fgets(buffer, 30, fp)) {
            buffer[strlen(buffer)-1] = '\0';

            // set which list to load into, then skip bc line of file is not a car
            if(strcmp(buffer, "AVAILABLE-FOR-RENT") == 0) {
                list_num = 1;
                continue;

            } else if(strcmp(buffer, "RENTED") == 0) {
                list_num = 2;
                continue;

            } else if(strcmp(buffer, "IN-REPAIR") == 0) {
                list_num = 3;
                continue;

            } else {
                new_car = malloc(sizeof(Node));
                // create a new car using data from file
                strcpy(new_car->car.plate, strtok(buffer, ","));
                new_car->car.mileage = atoi(strtok(NULL, ","));
                new_car->car.return_date = atoi(strtok(NULL, ","));
                // add it to the correct list
                switch(list_num) {
                    case 1:
                        push_available(new_car);
                        break;

                    case 2:
                        push_rented(new_car);
                        break;

                    case 3:
                        push_repair(new_car);
                        break;

                    default:
                        // list_num will always be set, so shouldn't occur
                        fprintf(stderr, "\nERROR: something is wrong with read_text()\n\n");
                        break;
                }
            }
        } // end while-loop
        //note that file ptr will be at the very end of the file
    }
    fclose(fp);
}

// ============================== MY FUNCTIONS ==============================

/**
 * Flushes all leftover data in the stream
 * @param char *input -the string that was just read from stdin
 */
void flush_input(char *input) {
    /* if the '\n' is NOT found in the word itself, flush the stream */
    if(strchr(input, '\n') == NULL) {
        while ((getchar()) != '\n');
        input[strlen(input)] = '\0';
    } else {
        input[strlen(input)-1] = '\0';
    }
}

/**
 * Checks if the string is a postive number (or zero)
 * @param char *input -the string to be checked
 * @return 1 if the string only contains digits
 */
int isNumber(char *input) {
    for(int i = 0; i < strlen(input); i++) {
        if(!isdigit(input[i])) {
            return 0; //contained a non-digit character
        }
    }
    return 1;
}

/**
 * Checks if the string is pure whitespace
 * @param char *input -the string to be checked
 * @return 1 if the string only contains whitespace characters
 */
int isWhitespace(char *input) {
    for(int i = 0; i < strlen(input); i++) {
        if(!isspace(input[i])) {
            return 0; //contained a non-whitespace character
        }
    }
    return 1;
}
