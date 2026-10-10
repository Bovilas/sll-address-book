#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "address_book.h"

record *load_address_book(const char *filename, int *addressCount){
    FILE *fp = fopen(filename, "r");
    record *addressBook = NULL;
    int count = 0;
    int returnvalue;

    if(fp == NULL) {
        return NULL;
    }

    char line[512];
    char* token;
    char name[NAME_BUFFER_SIZE];
    char surname[SURNAME_BUFFER_SIZE];
    char email[EMAIL_BUFFER_SIZE];
    char phone[PHONE_BUFFER_SIZE];

    while (fgets(line, sizeof(line), fp)) {
        token = strtok(line, ",");
        if (token == NULL) continue;
        strncpy(name, token, NAME_BUFFER_SIZE);
        name[NAME_BUFFER_SIZE - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(surname, token, SURNAME_BUFFER_SIZE);
        surname[SURNAME_BUFFER_SIZE - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(email, token, EMAIL_BUFFER_SIZE);
        email[EMAIL_BUFFER_SIZE - 1] = '\0';

        token = strtok(NULL, "\n");
        if (token == NULL) continue;
        strncpy(phone, token, PHONE_BUFFER_SIZE);
        phone[PHONE_BUFFER_SIZE - 1] = '\0';

        returnvalue = add_to_book(&addressBook, create_node(name, surname, email, phone));
        if(returnvalue){
            return NULL; // handle 
        }

        ++count;
    }

    if (fp) {
        fclose(fp);
    }

    *addressCount = count;
    return addressBook;
}

int save_address_book(const char *filename, record *addressBook){
    record *temp = addressBook;

    if (temp == NULL) {
        return 2;
    }

    FILE * fp = fopen(filename, "w");
    if(fp == NULL) {
        return 1;
    }
    
    while(temp!= NULL) {
        fprintf(fp, "%s,%s,%s,%s\n", 
        temp -> name,
        temp -> surname,
        temp -> email,
        temp -> phone);

        temp =  temp->next;
    }

    if (fp != NULL) {
        fclose(fp);
    }

    return 0;
}

record *create_node(char *name, char *surname, char *email, char *phone) {
    record *node = malloc(sizeof(record));

    if (node == NULL) {
        fprintf(stderr, "Problem allocating memory.\n");
        return NULL;
    }

    strncpy(node->name, name, NAME_BUFFER_SIZE - 1);
    node->name[NAME_BUFFER_SIZE - 1] = '\0';

    strncpy(node->surname, surname, SURNAME_BUFFER_SIZE - 1);
    node->surname[SURNAME_BUFFER_SIZE - 1] = '\0';

    strncpy(node->email, email, EMAIL_BUFFER_SIZE - 1);
    node->email[EMAIL_BUFFER_SIZE - 1] = '\0';

    strncpy(node->phone, phone, PHONE_BUFFER_SIZE - 1);
    node->phone[PHONE_BUFFER_SIZE - 1] = '\0';

    node->next = NULL;

    return node;
}


int add_to_book(record **addressBook, record *entry) { // negali but fail state?
    if(entry == NULL || addressBook == NULL) {
        return 1;
    }

    if(*addressBook == NULL) {
        entry->index = 0;
        *addressBook = entry;
        return 0;
    }

    record *temp = *addressBook;
    int index = 1;
    while(temp->next != NULL) {
        temp =  temp->next;
        ++index;
    }
    entry->index = index;
    temp->next =  entry;

    return 0;

}

int print_book(record *addressBook) {
    record *temp = addressBook;

    if (temp == NULL) {
        return 1;
    }

    while(temp != NULL) {
        printf("%d, %s, %s, %s, %s \n", 
        temp -> index,
        temp -> name,
        temp -> surname,
        temp -> email,
        temp -> phone);
        temp =  temp->next;
    } 

    return 0;
}

int delete_by_position(record **addressBook, int index) { 
    if(*addressBook == NULL) {
        return 1;
    }

    record *temp = *addressBook;

    if(index == 0) {
        if(temp->next == NULL){
            *addressBook =  temp->next;

            free_node(&temp);

            return 0;
        };

        *addressBook =  temp->next;
        record *next =  temp->next;

        free_node(&temp);

        next->index--;

        while(next->next != NULL) {
            next =  next->next;
            next->index--;
        }
        return 0;
    }

    for (int k = 0; k < index-1; k++){
        if(temp->next == NULL) {
            return 2;
        }
        temp =  temp->next;
    }
    
    record *todelete =  temp->next;

    if(todelete == NULL) {
        return 2;
    }

    record *next =  todelete->next;

    free_node(&todelete);    
    
    temp->next =  next;

    if(temp->next == NULL) {
        return 0;
    }

    // update indexes
    next->index--;

    while(next->next != NULL) {
        next =  next->next;
        next->index--;
    }

    return 0;
}

int delete_book(record **addressBook)
{
    if (addressBook == NULL)
        return 1;

    while (*addressBook != NULL) {
        int returnValue = delete_by_position(addressBook, 0);

        if (returnValue != 0)
            return returnValue;
    }

    return 0;
}

int free_node(record **toDelete)
{
    if (toDelete == NULL || *toDelete == NULL)
        return 1;

    record *node = *toDelete;
    free(node);

    *toDelete = NULL;

    return 0;
}

int add_by_position(record **addressBook, record *entry, int index) { 
    if(index == 0) {
        entry->index = 0;

        entry->next = ( record *) *addressBook;
        *addressBook = entry;

        //update indexs
        record *next = *addressBook;
        while(next->next != NULL) {
        next =  next->next;
        next->index++;
        }
        return 0;
    }

    if(*addressBook == NULL) {
        return 1;
    }

    record *temp = *addressBook;

    for(int k = 0; k < index-1; ++k) {
        if(temp->next == NULL) {
            return 2;
        }
        temp =  temp->next;
    }

    entry->next = temp->next;
    temp->next =  entry;
    entry->index = index;

    record *next = entry;
    //update indexes
    while(next->next != NULL) {
        next =  next->next;
        next->index++;
    }

    return 0;
}

record *find_by_position(record *addressBook, int index) {
    if(addressBook == NULL) {
        return NULL;
    }

    if(index == 0) {
        return addressBook;
    }

    record *temp = addressBook;
    for (int k = 0; k < index - 1; ++k) {
        if(temp->next == NULL) {
            return NULL;
        }

        temp =  temp->next;
    }

    return  temp->next;
}

int print_node(record *node) {
    if(node == NULL) {
        return 1;
    }

    printf("%d, %s, %s, %s, %s \n", 
        node -> index,
        node -> name,
        node -> surname,
        node -> email,
        node -> phone);
    
    return 0;
}

record *find_by_name(record *addressBook, char *name) {
    record *temp = addressBook;
    record *new = NULL;

    if(addressBook == NULL) {
        return NULL;
    }

    while (temp != NULL) {
        if (strcmp(name, temp->name) == 0) {
            record *copy = copy_node(temp);
            if (copy != NULL) {
                copy->index = temp->index;

                if (new == NULL) {
                    new = copy;
                } else {
                    record *last = new;
                    while (last->next != NULL)
                        last = last->next;
                
                    last->next = copy;
                }
            }
        }

        temp =  temp->next;
    }

    return new;
}


record *copy_node(record *node) {
    return  create_node(node->name, node->surname, node->email, node->phone);
}

record *find_by_surname(record *addressBook, char *surname) {
    record *temp = addressBook;
    record *new = NULL;

    if(addressBook == NULL) {
        return NULL;
    }

    while (temp != NULL) {
        if (strcmp(surname, temp->surname) == 0) {
            record *copy = copy_node(temp);
            if (copy != NULL) {
                copy->index = temp->index;

                if (new == NULL) {
                    new = copy;
                } else {
                    record *last = new;
                    while (last->next != NULL)
                        last = last->next;
                
                    last->next = copy;
                }
            }
        }

        temp =  temp->next;
    }

    return new;
}

record *find_by_email(record *addressBook, char *email) {
    record *temp = addressBook;
    record *new = NULL;

    if(addressBook == NULL) {
        return NULL;
    }

    while (temp != NULL) {
        if (strcmp(email, temp->email) == 0) {
            record *copy = copy_node(temp);
            if (copy != NULL) {
                copy->index = temp->index;

                if (new == NULL) {
                    new = copy;
                } else {
                    record *last = new;
                    while (last->next != NULL)
                        last = last->next;
                
                    last->next = copy;
                }
            }
        }

        temp =  temp->next;
    }

    return new;
}



record *find_by_phone(record *addressBook, char *phone) {
    record *temp = addressBook;
    record *new = NULL;

    if(addressBook == NULL) {
        return NULL;
    }

    while (temp != NULL) {
        if (strcmp(phone, temp->phone) == 0) {
            record *copy = copy_node(temp);
            if (copy != NULL) {
                copy->index = temp->index;

                if (new == NULL) {
                    new = copy;
                } else {
                    record *last = new;
                    while (last->next != NULL)
                        last = last->next;
                
                    last->next = copy;
                }
            }
        }

        temp =  temp->next;
    }

    return new;
}

