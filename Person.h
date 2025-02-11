#ifndef PERSON_H
#define PERSON_H

#include "mainCode.h"

typedef enum { READ, NOT_READ, FAILED_CALLOC } ReadStatus;

struct Person {
	char name[NAME_LENGTH];
	char surname[SURNAME_LENGTH];
	char email[EMAIL_LENGTH];
	char phone_number[PHONE_NUMBER_LENGTH];
	struct Person *next;
};

ReadStatus load_addresses_from_a_file(struct Person **list);
int display_addresses(struct Person *list);
int add_new_address_at_postion(struct Person *person, struct Person **list, int position);
int add_new_address_at_the_end(struct Person *person, struct Person **list);
struct Person *create_new_person(char *name, char *surname, char *email, char *phone_number);
int delete_address_at_position(struct Person **list, int position);
int delete_all_addresses(struct Person **list);
char *get_file_path();
struct Person *find_the_last_address(struct Person *list);
struct Person *find_address_by_position(struct Person *list, int position);
struct Person *find_address_by_name(struct Person *list, char *name);
struct Person *find_address_by_surname(struct Person *list, char *surname);
struct Person *find_address_by_email(struct Person *list, char *email);
struct Person *find_address_by_phone_number(struct Person *list, char *phone_number);

#endif