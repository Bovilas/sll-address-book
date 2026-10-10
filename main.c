#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <signal.h>

#include "address_book.h"
#include "interactive_menu.h"

extern int exitLoop; // because signal handling

// Signal handler 
void sigint_handler(){
    exitLoop = 1;    
}

void set_signal_action(void){
    struct sigaction act;

    act.sa_flags = 0;

    memset(&act, 0, sizeof(act));
    
    act.sa_handler = &sigint_handler;
    
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
}

/*
Your program should have this functionality:

    1    Display all records in the address book
    2    Add new address to the book (to the end)
    3    Add new address indicating the position where to put it
    4    Delete address indicating its position in the book
    5    Delete whole address book
    6    Find address by position
    7    Find addressess by name
    8    Find addressess by surname
    9    Find addressess by email
    10   Find addressess by phone
    11   Save to file
    12   Load from file
    13   Exit 
*/

int main() {
    set_signal_action();

    int addressCount = 0;

    record *addressBook = NULL;

    char filename[60];

    strcpy(filename, getenv("HOME"));
    strcat(filename, "/addresses.csv");

    addressBook =  load_address_book(filename, &addressCount);

    run_menu(&addressBook, filename, addressCount);

    if (addressBook != NULL) {
        delete_book(&addressBook);
    }
    

    return 0;
}

