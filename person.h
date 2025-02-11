#ifndef PERSON_H
#define PERSON_H

#include "main.h"

typedef enum { READ, NOT_READ, FAILED_MALLOC } ReadStatus;
typedef enum { FILTER_BY_NAME, FILTER_BY_SURNAME, FILTER_BY_EMAIL, FILTER_BY_PHONE_NUMBER } FilteringOptions;
typedef enum { HIDE, UNHIDE } Conversion;

struct Person {
	char name[NAME_LENGTH];
	char surname[SURNAME_LENGTH];
	char email[EMAIL_LENGTH];
	char phone_number[PHONE_NUMBER_LENGTH];
	int is_hidden;
	struct Person *next;
};

ReadStatus load_addresses_from_a_file(struct Person **list, char *path);
int display_addresses(struct Person *list);
int add_new_address(struct Person **list, struct Person *person, int position);
struct Person *create_new_person(char *name, char *surname, char *email, char *phone_number);
int delete_address(struct Person **list, int position);
char *get_file_path();
int filter_addresses_by_attributes(struct Person *list, FilteringOptions filter, char *matching_arg);
int filter_addresses_by_position(struct Person *list, int position);
int change_hide_var_of_addresses(struct Person *list, Conversion action);

#endif