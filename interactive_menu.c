#include "interactive_menu.h"

// #include "address_book.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int exitLoop = 0;

void clear_buffer() {
    if (exitLoop) return;
    int c;
    while ((c = getchar()) != '\n' && c != EOF && exitLoop == 0) {}
}

const char options[] = 
"1    Display all records in the address book \n"
"2    Add new address to the book (to the end) \n"
"3    Add new address indicating the position where to put it \n"
"4    Delete address indicating its position in the book \n"
"5    Delete whole address book \n"
"6    Find address by position \n"
"7    Find addressess by name \n"
"8    Find addressess by surname \n"
"9    Find addressess by email \n"
"10   Find addressess by phone \n"
"11   Save to file \n"
"12   Load from file \n"
"13   Exit \n";

int take_input_string(const char *prompt, char (*inputString)[], int size) {
    char *user_input = *inputString;
    int bufferSize = size;
    char *returnval = NULL;

    fprintf(stdout, "Please enter %s\n", prompt);
    returnval = fgets( (char *) user_input, bufferSize, stdin);
    
    if (returnval == NULL || strstr(user_input, "\n") == NULL) clear_buffer();

    //for signal handling
    if(exitLoop) {
        return -1;
    }

    while(strcmp(user_input, "\n") == 0) {
        printf("Invalid input, try again.\n");

        returnval = fgets( (char *) user_input, bufferSize, stdin);

        if (returnval == NULL || strstr(user_input, "\n") == NULL) clear_buffer();

        //for signal handling
        if(exitLoop) {
            return -1;
        }
    }

    user_input[strcspn(user_input, "\n")] = 0;

    return 0;
}

int take_input_choice(const int min, const int max) {
    int choice;
    char user_input[4];
    char *strptr;

    //for signal handling
    if(exitLoop) {
        return -1;
    }

    fprintf(stdout, "Please enter number between %d and %d\n", min, max);

    fgets( (char *) user_input, 4, stdin);

    if(user_input[2] == '\n' || user_input[1] == '\n') { // 1\n or 11\n 
        choice = strtol(user_input, &strptr, 10);
    }
    else {
        choice = -1;
        clear_buffer();
    }

    if(choice == 0 && (user_input[0] != '0' && user_input[1] != '0') ) { // either letter or 0, can be 0\n or 00\n, one or both have to be zero
        choice = -1;
    }

    //for signal handling
    if(exitLoop) {
        return -1;
    }
    
    while(choice < min || choice > max) {
        printf("Invalid input, try again.\n");

        fgets( (char *) user_input, 4, stdin);

        if(user_input[2] == '\n' || user_input[1] == '\n') { // 1\n or 11\n 
            choice = strtol(user_input, &strptr, 10);
        }
        else {
            choice = -1;
            clear_buffer();
        }
    
        if(choice == 0 && (user_input[0] != '0' && user_input[1] != '0') ) { // either letter or 0, can be 0\n or 00\n, one or both have to be zero
            choice = -1;
        }
    
        //for signal handling
        if(exitLoop) {
            return -1;
        }
    }

    return choice;
}

void display_options() {
    fprintf(stdout, "Enter number to choose option: \n");
    fprintf(stdout, options);

}

void run_menu(record **passedBook, const char *filename, int addressCount) {
    int input;
    record *addressBook = *passedBook;

    while(exitLoop == 0) {
        display_options();

        input = take_input_choice(1, 13);
        if(input == -1) break;

        switch (input) {
            case 1: // 1    Display all records in the address book
                handle_book_print(addressBook);
            break;

            case 2: // 2    Add new address to the book (to the end)

                handle_end_insert(&addressBook, &addressCount);
            
            break;

            case 3: // 3    Add new address indicating the position where to put it

                handle_pos_insert(&addressBook, &addressCount);
            
            break;

            case 4: // 4    Delete address indicating its position in the book
            
                handle_pos_delete(&addressBook, &addressCount);

            break;

            case 5: // 5    Delete whole address book

                handle_delete_book(&addressBook, &addressCount);

            break;

            case 6: // 6    Find address by position

                handle_find_by_pos(addressBook, &addressCount);

            break;

            case 7: //    7    Find addressess by name

                handle_find_by_name(addressBook, &addressCount);

            break;

            case 8: // 8    Find addressess by surname

                handle_find_by_surname(addressBook, &addressCount);

            break;

            case 9: // 9    Find addressess by email

                handle_find_by_email(addressBook, &addressCount);

            break;

            case 10: // 10   Find addressess by phone

                handle_find_by_phone(addressBook, &addressCount);

            break;

            case 11: // 11   Save to file

                handle_save(filename, addressBook);

            break;

            case 12: // 12   Load from file
            
                handle_load(filename, &addressBook, &addressCount);

            break;

            case 13: // 13 Exit

                handle_exit(&addressBook);
            

            break;
        }
    }
}

void handle_end_insert(record **addressBook, int *addressCount) {
    char name[NAME_BUFFER_SIZE];
    char surname[SURNAME_BUFFER_SIZE];
    char email[EMAIL_BUFFER_SIZE];
    char phone[PHONE_BUFFER_SIZE];
    int returnval;

    take_input_string("name", &name, NAME_BUFFER_SIZE);
    //for signal handling
    if(exitLoop) {return; }

    take_input_string("surname", &surname, SURNAME_BUFFER_SIZE);
    if(exitLoop) {return; }

    take_input_string("email", &email, EMAIL_BUFFER_SIZE);
    if(exitLoop) {return; }
    
    take_input_string("phone", &phone, PHONE_BUFFER_SIZE);
    if(exitLoop) {return; }

    record *newAddress = create_node(name, surname, email, phone);
    returnval = add_to_book(addressBook, newAddress);
    if (returnval) {
        fprintf(stderr, "Problem inserting to book. \n");
        free_node(&newAddress);
    }
    else {
        print_node(newAddress);
        ++(*addressCount);
    }
}

void handle_pos_insert(record **addressBook, int *addressCount) {
    char name[NAME_BUFFER_SIZE];
    char surname[SURNAME_BUFFER_SIZE];
    char email[EMAIL_BUFFER_SIZE];
    char phone[PHONE_BUFFER_SIZE];
    int returnval;
    record *newAddress;
    int input;

    take_input_string("name", &name, NAME_BUFFER_SIZE);
    //for signal handling
    if(exitLoop) {return; }

    take_input_string("surname", &surname, SURNAME_BUFFER_SIZE);
    if(exitLoop) {return; }

    take_input_string("email", &email, EMAIL_BUFFER_SIZE);
    if(exitLoop) {return; }
    
    take_input_string("phone", &phone, PHONE_BUFFER_SIZE);
    if(exitLoop) {return; }

    newAddress = create_node(name, surname, email, phone);
    input = take_input_choice(0, *addressCount);
    if (input == -1){ 
        free_node(&newAddress);
        return;
    }

    returnval = add_by_position(addressBook, newAddress, input);

    if(returnval == 1) {
        fprintf(stderr, "Invalid addressbook. \n");
        free_node(&newAddress);
    }
    else if(returnval == 2) {
        fprintf(stderr, "Invalid index. \n");
        free_node(&newAddress);
    }
    else {
        print_node(newAddress);
        ++(*addressCount);
    }
}

void handle_pos_delete(record **addressBook, int *addressCount) {
    int returnValue;
    int input;
    if (*addressCount == 0) {
        fprintf(stdout, "Address book empty.\n");
        return;
    }
    fprintf(stdout, "Enter the index of the address you want to delete.\n");
    input = take_input_choice(0, *addressCount - 1);
    if (input == -1) return;
    
    returnValue = delete_by_position(addressBook, input);
    if(returnValue == 1){
        fprintf(stderr, "Invalid addressBook. \n");
    }
    else if (returnValue == 2) {
        fprintf(stderr, "Invalid index. \n");
    }
    else {
        --(*addressCount);
    }
}

void handle_book_print(record *addressBook) {
    int returnValue = print_book(addressBook);
    if (returnValue) {
        fprintf(stdout, "Book enpty. \n");
    }
}

void handle_delete_book(record **addressBook, int *addressCount) {
    int returnValue = delete_book(addressBook);
    if(returnValue) {
        fprintf(stderr, "Problem deleting book. \n");
    }
    *addressCount = 0;
}

void handle_find_by_pos(record *addressBook, int *addressCount) {
    if (*addressCount == 0) {
        fprintf(stdout, "Address book empty.\n");
        return;
    }
    int input = take_input_choice(0, (*addressCount) - 1);
    if (input == -1) return;
    int returnValue = print_node(find_by_position(addressBook, input));
    if (returnValue) {
        fprintf(stdout, "Node NULL \n");
    }
}

void handle_find_by_name(record *addressBook, int *addressCount) {
    char name[NAME_BUFFER_SIZE];
    if (*addressCount == 0) {
            fprintf(stdout, "Address book empty.\n");
            return;
        }
        take_input_string("name", &name, NAME_BUFFER_SIZE);   
        record *tempAddressBook = find_by_name(addressBook, name);
        int returnValue = print_book(tempAddressBook);
        if (returnValue) {
            fprintf(stdout, "Address not found. \n");
        }
        delete_book(&tempAddressBook);
}

void handle_find_by_surname(record *addressBook, int *addressCount) {
    char surname[SURNAME_BUFFER_SIZE];
    if (*addressCount == 0) {
            fprintf(stdout, "Address book empty.\n");
            return;
        }
        take_input_string("surname", &surname, SURNAME_BUFFER_SIZE);   
        record* tempAddressBook = find_by_surname(addressBook, surname);
        int returnValue = print_book(tempAddressBook);
        if (returnValue) {
            fprintf(stdout, "Address not found. \n");
        }
        delete_book(&tempAddressBook);
}

void handle_find_by_email(record *addressBook, int *addressCount) {
    char email[EMAIL_BUFFER_SIZE];
    if (*addressCount == 0) {
            fprintf(stdout, "Address book empty.\n");
            return;
        }
        take_input_string("email", &email, EMAIL_BUFFER_SIZE);   
        record *tempAddressBook = find_by_email(addressBook, email);
        int returnValue = print_book(tempAddressBook);
        if (returnValue) {
            fprintf(stdout, "Address not found. \n");
        }
        delete_book(&tempAddressBook);
}

void handle_find_by_phone(record *addressBook, int *addressCount) {
    char phone[PHONE_BUFFER_SIZE];
    if (*addressCount == 0) {
            fprintf(stdout, "Address book empty.\n");
            return;
        }
        take_input_string("phone", &phone, PHONE_BUFFER_SIZE);   
        record * tempAddressBook = find_by_phone(addressBook, phone);
        int returnValue = print_book(tempAddressBook);
        if (returnValue) {
            fprintf(stdout, "Address not found. \n");
        }
        delete_book(&tempAddressBook);
}

void handle_save(const char *filename, record *addressBook) {
    int returnValue = save_address_book(filename, addressBook);
    if(returnValue == 1) {
        fprintf(stderr, "Error opening file.");
    } 
    else if (returnValue == 2) {
        fprintf(stdout, "Book empty.\n");
    }
}

void handle_load(const char *filename, record **addressBook, int *addressCount) {
    record* newAddressBook =  load_address_book(filename, addressCount);
    
    if (newAddressBook == NULL) {
        fprintf(stderr, "Error loading book from file. \n");
    }

    int returnValue = delete_book(addressBook);
    
    if (returnValue) {
        fprintf(stderr, "Error deleting old book. \n");
    }

    *addressBook = newAddressBook;
}

void handle_exit(record **addressBook) {
    exitLoop = 1;
    int returnValue = delete_book(addressBook);
    if(returnValue) {
        fprintf(stderr, "Problem deleting book. \n");
    }
}