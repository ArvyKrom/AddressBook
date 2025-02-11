
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mainCode.h"
#include "validation.h"
#include "Person.h"

ReadStatus load_addresses_from_a_file(struct Person **list)
{
	FILE *file = fopen(get_file_path(), "r");

	if (file == NULL) {
		perror("Was unable to open the addresses.csv file");
		return NOT_READ;
	}

	ReadStatus ret = READ;
	char line[ADDRESS_LINE_LENGTH];

	while (fgets(line, ADDRESS_LINE_LENGTH, file)) {
		if (strcmp(line, "\n") == 0) {
			continue;
		}

		char *name	   = NULL;
		char *surname	   = NULL;
		char *email	   = NULL;
		char *phone_number = NULL;

		name	     = strtok(line, DELIMETER_IN_CSV_FILE);
		surname	     = strtok(NULL, DELIMETER_IN_CSV_FILE);
		email	     = strtok(NULL, DELIMETER_IN_CSV_FILE);
		phone_number = strtok(NULL, "\n");

		if (is_valid_address(name, surname, email, phone_number) == VALID) {
			struct Person *person = NULL;
			person		      = create_new_person(name, surname, email, phone_number);
			if (person == NULL) {
				ret = FAILED_CALLOC;
				break;
			}
			add_new_address_at_the_end(person, list);
		}
	}
	fclose(file);
	return ret;
}

char *get_file_path()
{
	int path_length = strlen(getenv("HOME")) + 1 + strlen(ADDRESSES_FILE_NAME);
	char *path	= NULL;
	path		= (char *)malloc(sizeof(char) * (path_length + 1));

	if (path == NULL)
		return NULL;

	strcat(path, getenv("HOME"));
	strcat(path, "/");
	strcat(path, ADDRESSES_FILE_NAME);

	return path;
}

struct Person *create_new_person(char *name, char *surname, char *email, char *phone_number)
{
	struct Person *person = NULL;

	// Using Calloc to be able to find an empty struct
	person = (struct Person *)calloc(1, sizeof(struct Person));
	if (person == NULL) {
		perror("Failed to allocate memory for a person object");
		return NULL;
	}

	strncpy(person->name, name, NAME_LENGTH - 1);
	strncpy(person->surname, surname, SURNAME_LENGTH - 1);
	strncpy(person->email, email, EMAIL_LENGTH - 1);
	strncpy(person->phone_number, phone_number, PHONE_NUMBER_LENGTH - 1);

	return person;
}

int add_new_address_at_the_end(struct Person *person, struct Person **list)
{
	struct Person *temp;

	temp = *list;
	if (strlen(temp->name) == 0) {
		*list = person;
		return 0;
	}
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = person;
	return 0;
}

int display_addresses(struct Person *list)
{
	if (list == NULL)
		return 1;

	int position = 1;

	// Calculate "-----..." width

	int separator_line_width =
		5 + NAME_LENGTH + 3 + SURNAME_LENGTH + 3 + EMAIL_LENGTH + 3 + PHONE_NUMBER_LENGTH + 5;

	// Print table header

	printf("| No. | Name%*s | Surname%*s | Email%*s | Phone Number  |\n", NAME_LENGTH - 4, "",
	       SURNAME_LENGTH - 7, "", EMAIL_LENGTH - 5, "");

	// Print "-----..." separator
	for (int i = 0; i < separator_line_width; i++) {
		printf("-");
	}
	printf("\n");

	// Print all unhidden addresses

	do {
		if (list->is_hidden == 0) {
			printf("| %-3d | %-*s | %-*s | %-*s | %-*s |\n", position, NAME_LENGTH, list->name,
			       SURNAME_LENGTH, list->surname, EMAIL_LENGTH, list->email, PHONE_NUMBER_LENGTH,
			       list->phone_number);
		}

		list = list->next;
		position++;
	} while (list != NULL);

	// Print table bottom marking "-----..." separator

	for (int i = 0; i < separator_line_width; i++)
		printf("-");

	printf("\n\n");

	return 0;
}
int add_new_address_at_position(struct Person *person, struct Person **list, int position)
{
	if (position == 1) {
		//Inserted Person points at the initial list start
		person->next = *list;
		//List starts at the new inserted person
		*list = person;
	} else {
		struct Person *tmp = *list;
		//Do position-2 jumps from the start
		for (int i = 0; i < position - 2; i++) {
			if (tmp->next != NULL)
				tmp = (tmp)->next;
			else
				return 1;
		}
		// Make the person point at the selected position
		person->next = tmp->next;
		// Make the position before it point at the inserted person
		tmp->next = person;
	}
	return 0;
}
int delete_address_at_position(struct Person **list, int position)
{
	if (*list == NULL)
		return 1;

	if (position == 1) {
		//Save the pointer to the second person, delete the first one and make the second to be the start of the list
		struct Person *tmp = (*list)->next;
		free(*list);
		*list = tmp;
	} else {
		struct Person *tmp = *list;
		//Do position-2 jumps from the start
		for (int i = 0; i < position - 2; i++) {
			if (tmp->next != NULL)
				tmp = (tmp)->next;
			else
				return 1;
		}
		// Check if the next person (to be deleted) exists
		if (tmp->next == NULL)
			return 1;

		// Save the pointer of the to be delete person
		struct Person *to_be_deleted = tmp->next;
		// Make the current person point to the one after to_bo_deleted
		tmp->next = (tmp->next)->next;
		// Delete to_be_delete Person
		free(to_be_deleted);
		return 0;
	}
	return 0;
}
int delete_all_addresses(struct Person **list)
{
	if (*list == NULL)
		return 0;

	struct Person *tmp = NULL;

	do {
		tmp = (*list)->next;
		free(*list);
		*list = tmp;
	} while (*list != NULL);
	return 0;
}

int filter_addresses_by_position(struct Person *list, int position)
{
	// No need to check if list is NULL as that's done in hide function
	if (hide_every_address(list) != 0)
		return 1;

	for (int i = 1; i < position; i++) {
		if (list->next != NULL) {
			list->is_hidden = 1;
			list		= list->next;
		} else {
			return 1;
		}
	}
	list->is_hidden = 0;
	return 0;
}

int filter_addresses_by_attributes(struct Person *list, FilteringOptions filter, char *matching_arg)
{
	if (list == NULL)
		return 1;

	int atleast_one_match_found = 0;

	do {
		if (list->is_hidden != 1) {
			switch (filter) {
			case FILTER_BY_NAME:
				if (strcmp(list->name, matching_arg) == 0) {
					list->is_hidden		= 0;
					atleast_one_match_found = 1;
				} else {
					list->is_hidden = 1;
				}
				break;
			case FILTER_BY_SURNAME:
				if (strcmp(list->surname, matching_arg) == 0) {
					list->is_hidden		= 0;
					atleast_one_match_found = 1;
				} else {
					list->is_hidden = 1;
				}
				break;
			case FILTER_BY_EMAIL:
				if (strcmp(list->email, matching_arg) == 0) {
					list->is_hidden		= 0;
					atleast_one_match_found = 1;
				} else {
					list->is_hidden = 1;
				}
				break;
			case FILTER_BY_PHONE_NUMBER:
				if (strcmp(list->phone_number, matching_arg) == 0) {
					list->is_hidden		= 0;
					atleast_one_match_found = 1;
				} else {
					list->is_hidden = 1;
				}
				break;
			}
		}
		list = list->next;
	} while (list != NULL);
	if (atleast_one_match_found)
		return 0;
	else
		return 1;
}

int hide_every_address(struct Person *list)
{
	if (list == NULL)
		return 1;
	do {
		list->is_hidden = 1;
		list		= list->next;
	} while (list != NULL);
	return 0;
}

int unhide_every_address(struct Person *list)
{
	if (list == NULL)
		return 1;
	do {
		list->is_hidden = 0;
		list		= list->next;
	} while (list != NULL);
	return 0;
}