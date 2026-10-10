#ifndef ADDRESS_BOOK_H 
#define ADDRESS_BOOK_H 

#define NAME_BUFFER_SIZE 30
#define SURNAME_BUFFER_SIZE 30
#define EMAIL_BUFFER_SIZE 320
#define PHONE_BUFFER_SIZE 20


typedef struct Record {
    char name[NAME_BUFFER_SIZE];
    char surname[SURNAME_BUFFER_SIZE];
    char email[EMAIL_BUFFER_SIZE];
    char phone[PHONE_BUFFER_SIZE];
    int index;
    struct Record *next;
} record;

int print_book(record *addressBook); 
int add_to_book(record **addressBook, record *entry); 
int add_by_position(record **addressBook, record *entry, int index); 
int delete_by_position(record **addressBook, int index); 
int delete_book(record **addressBook); 
record *find_by_position(record *addressBook, int index); 
record *find_by_name(record *addressBook, char *name); 
record *find_by_surname(record *addressBook, char *surname); 
record *find_by_email(record *addressBook, char *email); 
record *find_by_phone(record *addressBook, char *phone); 
record *load_address_book(const char *filename, int *addressCount); 
int save_address_book(const char *filename, record *addressBook); 
record *create_node(char *name, char *surname, char *email, char *phone);
int print_node(record *node);
int free_node(record **node);
record *copy_node(record *node);

#endif // ADDRESS_BOOK