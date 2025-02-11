#ifndef PERSON_H
#define PERSON_H

#include "mainCode.h"

typedef enum { READ, NOT_READ, FAILED_CALLOC } ReadStatus;
typedef enum { FILTER_BY_NAME, FILTER_BY_SURNAME, FILTER_BY_EMAIL, FILTER_BY_PHONE_NUMBER } FilteringOptions;

struct Person {
	char name[NAME_LENGTH];
	char surname[SURNAME_LENGTH];
	char email[EMAIL_LENGTH];
	char phone_number[PHONE_NUMBER_LENGTH];
	int is_hidden;
	struct Person *next;
};

ReadStatus load_addresses_from_a_file(struct Person **list);
int display_addresses(struct Person *list);
int add_new_address_at_position(struct Person *person, struct Person **list, int position);
int add_new_address_at_the_end(struct Person *person, struct Person **list);
struct Person *create_new_person(char *name, char *surname, char *email, char *phone_number);
int delete_address_at_position(struct Person **list, int position);
int delete_all_addresses(struct Person **list);
char *get_file_path();
int filter_addresses_by_attributes(struct Person *list, FilteringOptions filter, char *matching_arg);
int filter_addresses_by_position(struct Person *list, int position);
int hide_every_address(struct Person *list);
int unhide_every_address(struct Person *list);

#endif