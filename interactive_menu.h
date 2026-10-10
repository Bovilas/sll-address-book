#ifndef INTERACTIVE_MENU_H
#define INTERACTIVE_MENU_H

#include "address_book.h"

int take_input_choice(const int min, const int max);

void display_options();

int take_input_string(const char *prompt, char (*inputString)[], int size);

void clear_buffer();

void run_menu(record **passedBook, const char *filename, int addressCount);

void handle_end_insert(record **addressBook, int *addressCount);
void handle_pos_insert(record **addressBook, int *addressCount);
void handle_pos_delete(record **addressBook, int *addressCount);
void handle_book_print(record *addressBook);
void handle_delete_book(record **addressBook, int *addressCount);
void handle_find_by_pos(record *addressBook, int *addressCount);
void handle_find_by_name(record *addressBook, int *addressCount);
void handle_find_by_surname(record *addressBook, int *addressCount);
void handle_find_by_email(record *addressBook, int *addressCount);
void handle_find_by_phone(record *addressBook, int *addressCount);
void handle_save(const char *filename, record *addressBook);
void handle_load(const char *filename, record **addressBook, int *addressCount);
void handle_exit(record **addressBook);

#endif