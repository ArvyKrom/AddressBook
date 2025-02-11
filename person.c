
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "main.h"
#include "validation.h"
#include "person.h"

ReadStatus load_addresses_from_a_file(struct Person **list, char *path)
{
	path	   = get_file_path();
	FILE *file = fopen(path, "r");

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
				ret = FAILED_MALLOC;
				break;
			}

			add_new_address(list, person, -1);
		}
	}
	fclose(file);
	if (path != NULL)
		free(path);

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
	person		      = (struct Person *)malloc(sizeof(struct Person));

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

int add_new_address(struct Person **list, struct Person *person, int position)
{
	if (position == 1) {
		person->next = *list;
		*list	     = person;

	} else if (position > 1) {
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

	} else { // Put the new person at the end of the list
		struct Person *temp;
		temp = *list;

		if (temp == NULL) {
			*list = person;
			return 0;
		}

		while (temp->next != NULL) {
			temp = temp->next;
		}

		temp->next = person;
	}
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

	// Print "-----..." separator
	printf("\n");
	for (int i = 0; i < separator_line_width; i++) {
		printf("-");
	}
	printf("\n");

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

/*
 * If a position of -1 is passed the delete_address function will delete everything
 */

int delete_address(struct Person **list, int position)
{
	if (*list == NULL)
		return 1;

	struct Person *tmp		     = *list;
	struct Person *next_after_tmp	     = NULL;
	struct Person *before_the_delete_one = NULL;
	int i				     = 1;
	int is_specified_position_deleted    = 0;

	// Iterate through the list
	do {
		next_after_tmp = tmp->next;
		if (i == position - 1)
			before_the_delete_one = tmp;
		if (i == position || position == -1)
			free(tmp);
		if (i == position)
			is_specified_position_deleted = 1;
		tmp = next_after_tmp;
		i++;
	} while (tmp != NULL && !is_specified_position_deleted);

	if (tmp == NULL && position == -1) {
		*list = NULL;
		return 0;
	}

	if (is_specified_position_deleted) {
		before_the_delete_one->next = next_after_tmp;
		return 0;
	}

	return 1;
}

/*
 * Both of the filtering functions, either by position or by an attribute do the
 * filtering by hiding/unhiding certain addresses from the whole list
 */

int filter_addresses_by_position(struct Person *list, int position)
{
	// No need to check if list is NULL as that's done in change_hide... function
	if (change_hide_var_of_addresses(list, HIDE) != 0)
		return 1;

	for (int i = 1; i < position; i++) {
		if (list->next != NULL) {
			list = list->next;
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

int change_hide_var_of_addresses(struct Person *list, Conversion action)
{
	if (list == NULL)
		return 1;

	do {
		if (action == HIDE)
			list->is_hidden = 1;
		else if (action == UNHIDE)
			list->is_hidden = 0;
		list = list->next;
	} while (list != NULL);

	return 0;
}