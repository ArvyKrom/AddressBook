

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "person.h"
#include "main.h"
#include "validation.h"

int help_command(void);
int display_command(struct Person *list);
ReadCommandResult read_command(char *line, char **command, char *arg[]);
int add_address_command(struct Person **list, char *arg[]);
int clear_book_command(struct Person **list);
int del_address_command(struct Person **list, char *arg[]);
int find_address_at_command(struct Person *list, char *arg[]);
int filter_by_attr_command(struct Person *list, char *arg[], FilteringOptions filter);
int NULL_every_arg(char *arg[]);
void clean_exit(int signum);

//These two need to be global to be able to access them in the signal handler
struct Person *list = NULL;
char *path	    = NULL;

int main()
{
	printf("\u2022 Welcome to the Address Book!\n");
	printf("\u2022 Thank You for using the coolest Address Book in the area!\n");
	printf("\u2022 All rights reserved by Arvydas Productions\u00A9\n");
	printf("\u2022 Type \"\033[1mhelp\033[0m\" for a list of commands\n");

	int signals_to_ignore[] = { SIGHUP, SIGQUIT, SIGILL, SIGTRAP, SIGABRT };

	for (int i = 0; i < sizeof(signals_to_ignore) / sizeof(signals_to_ignore[0]); i++)
		signal(signals_to_ignore[i], SIG_IGN);

	signal(SIGINT, clean_exit);

	printf("\nTrying to read saved Addresses...\n");

	ReadStatus ret = load_addresses_from_a_file(&list, path);

	if (ret == FAILED_MALLOC)
		clean_exit(0);
	else if (ret == NOT_READ)
		printf("Did not load in any addresses\n");
	else
		printf("Read was succesful\n\n");

	char user_input[200];
	char *command = NULL;
	char *arg[5]  = { NULL, NULL, NULL, NULL, NULL };
	char position_arg
		[LONGEST_ARGUMENT_LENGTH]; // Needed to have a pointer to a string where a postion could be stored

	while (1) {
		NULL_every_arg(arg);
		printf("\033[1m> \033[0m");
		fgets(user_input, sizeof(user_input), stdin) == NULL;

		ReadCommandResult ret = read_command(user_input, &command, arg);

		if (command == NULL) {
			printf("\033[1m(!)\033[0m Seems like You have not entered anything\n");
			continue;
		}

		if (strcmp(command, "Display") == 0) {
			display_command(list);

		} else if (strcmp(command, "AddAddress") == 0) {
			arg[4] = position_arg;
			strncpy(arg[4], "-1", sizeof(arg[4]) - sizeof(char));
			add_address_command(&list, arg);

		} else if (strcmp(command, "AddAddressAt") == 0) {
			add_address_command(&list, arg);

		} else if (strcmp(command, "ClearBook") == 0) {
			arg[0] = position_arg;
			strncpy(arg[0], "-1", sizeof(arg[0]) - sizeof(char));
			del_address_command(&list, arg);

		} else if (strcmp(command, "DeleteAddressAt") == 0) {
			del_address_command(&list, arg);

		} else if (strcmp(command, "FindAddressAt") == 0) {
			find_address_at_command(list, arg);

		} else if (strcmp(command, "FilterByName") == 0) {
			filter_by_attr_command(list, arg, FILTER_BY_NAME);

		} else if (strcmp(command, "FilterBySurname") == 0) {
			filter_by_attr_command(list, arg, FILTER_BY_SURNAME);

		} else if (strcmp(command, "FilterByEmail") == 0) {
			filter_by_attr_command(list, arg, FILTER_BY_EMAIL);

		} else if (strcmp(command, "FilterByPhoneNumber") == 0) {
			filter_by_attr_command(list, arg, FILTER_BY_PHONE_NUMBER);

		} else if (strcmp(command, "q") == 0) {
			clean_exit(0);

		} else if (strcmp(command, "help") == 0) {
			help_command();

		} else {
			printf("\033[1m(!)\033[0m This command does not exist\n");
		}
	}

	return 0;
}

int help_command(void)
{
	printf(" 1. \"\033[1mDisplay\033[0m\" shows a table of all currently loaded addresses\n");
	printf(" 2. \"\033[1mAddAddress name surname email phone_number\033[0m\" adds new person at the end of the book\n");
	printf(" 3. \"\033[1mAddAddressAt name surname email phone_number position\033[0m\" adds new person at the specified position in the book\n");
	printf(" 4. \"\033[1mClearBook\033[0m\" Deletes the entire book content\n");
	printf(" 5. \"\033[1mDeleteAddressAt position\033[0m\" Deletes the the address of a person at a specified position\n");
	printf(" 6. \"\033[1mFindAddressAt position\033[0m\" shows the address of person at a specified position\n");
	printf(" 7. \"\033[1mFilterByName name\033[0m\" Filters the book by addresses' name and shows the matches\n");
	printf(" 8. \"\033[1mFilterBySurname surname\033[0m\" Filters the book by addresses' surname and shows the matches\n");
	printf(" 9. \"\033[1mFilterByEmail email\033[0m\" Filters the book by addresses' email and shows the matches\n");
	printf("10. \"\033[1mFilterByPhoneNumber phone_number\033[0m\" Filters the book by addresses' phone number and shows the matches\n");
	printf("11. \"\033[1mq\033[0m\" Exits the program\n");
	printf("\n! For the commands that require arguments seprate the arguments with white spaces.\n");
	printf("! Don't leave any leading or trailing whitespaces\n");
	printf("Filters can be stacked and will reset if a Display command is used or if the filter results in no matches\n");
	printf("Command example:\n > \033[1mAddAddress Josh Smith Josh@gmail.com +37064812267\033[0m\n\n");

	return 0;
}

ReadCommandResult read_command(char *line, char **command, char *arg[])
{
	char *tmp = NULL;

	//Read the command
	tmp = strtok(line, " \n"); // Check for '\n' or ' ' as delimeter
	if (tmp == NULL || strlen(tmp) > COMMAND_LENGTH - 1)
		return INVALID_COMMAND;

	*command = tmp;

	//Read the arguments
	for (int i = 0; i < 5; i++) {
		tmp = strtok(NULL, " \n");
		if (tmp != NULL) {
			if (strlen(tmp) < LONGEST_ARGUMENT_LENGTH)
				arg[i] = tmp;
			else
				return INVALID_ARGUMENT;
		}
		tmp = NULL;
	}

	return OK;
}

int display_command(struct Person *list)
{
	change_hide_var_of_addresses(list, UNHIDE);
	if (display_addresses(list) == 1)
		printf("\033[1m(!)\033[0m There are no addresses in the book\n");
	return 0;
}

int add_address_command(struct Person **list, char *arg[])
{
	if (arg[0] == NULL || arg[1] == NULL || arg[2] == NULL || arg[3] == NULL || arg[4] == NULL) {
		printf("\033[1m(!)\033[0m Looks like You've provided too few arguments\n");
		return 1;
	}

	ValidationStatus is_this_address_valid = is_valid_address(arg[0], arg[1], arg[2], arg[3]);
	if (is_this_address_valid == VALID) {
		struct Person *person = create_new_person(arg[0], arg[1], arg[2], arg[3]);

		if (person == NULL) {
			perror("Failed to allocate memory to create a new person object");
			clean_exit(0);
		}

		char *endptr  = NULL;
		long position = strtol(arg[4], &endptr, 10);
		if (*endptr != '\0') {
			printf("\033[1m(!)\033[0m Looks like the position that You've entered is not a number\n");
			return 1;
		}

		if (add_new_address(list, person, (int)position) != 0) {
			printf("\033[1m(!)\033[0m Seems like the list is not that long\n");
			return 1;
		}

		if (position > 0) {
			printf("Successfully added a new address to the book in the position %ld\n",
			       position);
		} else {
			printf("Successfully added a new address to the end of the book\n");
			return 0;
		}

	} else if (is_this_address_valid == WRONG_NAME_LENGTH) {
		printf("\033[1m(!)\033[0m Looks like the name that you've entered was too long\n");

	} else if (is_this_address_valid == WRONG_SURNAME_LENGTH) {
		printf("\033[1m(!)\033[0m Looks like the surname that you've entered was too long\n");

	} else if (is_this_address_valid == WRONG_EMAIL_LENGTH) {
		printf("\033[1m(!)\033[0m Looks like the email that you've entered was too long\n");

	} else if (is_this_address_valid == WRONG_PHONE_NUMBER_LENGTH) {
		printf("\033[1m(!)\033[0m Looks like the phone number that you've entered was too long\n");
	}

	return 1;
}

int del_address_command(struct Person **list, char *arg[])
{
	char *endptr  = NULL;
	long position = strtol(arg[0], &endptr, 10);

	if (*endptr != '\0') {
		printf("\033[1m(!)\033[0m Looks like the position that You've entered is not a number\n");
		return 1;
	}

	if (delete_address(list, position) == 0) {
		if (position == -1)
			printf("Deleted every address in the book\n");
		else
			printf("Deleted the address at the %s position\n", arg[0]);

	} else {
		printf("\033[1m(!)\033[0m The address at the specified position does not exist\n");
	}

	return 0;
}

int find_address_at_command(struct Person *list, char *arg[])
{
	char *endptr  = NULL;
	long position = strtol(arg[0], &endptr, 10);

	if (*endptr != '\0') {
		printf("\033[1m(!)\033[0m Looks like the position that You've entered is not a number\n");
		return 1;
	}

	if (filter_addresses_by_position(list, position) != 0)
		printf("\033[1m(!)\033[0m The address at the specified position does not exist\n");

	else
		display_addresses(list);

	return 0;
}

int NULL_every_arg(char *arg[])
{
	if (arg == NULL)
		return 1;

	for (int i = 0; i < 5; i++) {
		arg[i] = NULL;
	}

	return 0;
}

int filter_by_attr_command(struct Person *list, char *arg[], FilteringOptions filter)
{
	if (filter_addresses_by_attributes(list, filter, arg[0]) != 0) {
		printf("\033[1m(!)\033[0m There are no addresses that would match this pattern. Filters are reset\n");
		change_hide_var_of_addresses(list, UNHIDE);

	} else {
		display_addresses(list);
	}

	return 0;
}

void clean_exit(int signum)
{
	delete_address(&list, -1);

	if (path != NULL)
		free(path);

	exit(0);
}
