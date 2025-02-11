#ifndef UI_H
#define UI_H

#include "linked_list.h"
#include "main.h"
#include "validation.h"

extern int is_main_loop_running;

typedef enum { OK, INVALID_COMMAND, INVALID_ARGUMENT } ReadCommandResult;

int help_command(void);
int display_command(struct Person *list);
ReadCommandResult read_command(char *line, char **command, char *arg[]);
int add_address_command(struct Person **list, char *arg[]);
int clear_book_command(struct Person **list);
int del_address_command(struct Person **list, char *arg[]);
int find_address_at_command(struct Person *list, char *arg[]);
int filter_by_attr_command(struct Person *list, char *arg[], FilteringOptions filter);
int NULL_every_arg(char *arg[]);
int main_loop(struct Person **list);

#endif