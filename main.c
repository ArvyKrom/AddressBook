

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "linked_list.h"
#include "main.h"
#include "validation.h"
#include "ui.h"
#include "program_start.h"

void clean_exit(int signum);

int is_main_loop_running = 1;

int main()
{
	int ret		    = 0;
	struct Person *list = NULL;

	signal(SIGINT, clean_exit);

	if (program_start(&list) == 1) {
		ret		     = 1;
		is_main_loop_running = 0;
	}

	main_loop(&list);

	delete_address(&list, -1);

	return ret;
}

void clean_exit(int signum)
{
	is_main_loop_running = 0;
}
