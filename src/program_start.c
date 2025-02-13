#include <signal.h>
#include <stdio.h>

#include "program_start.h"



int program_start(struct Person **list){
    printf("\u2022 Welcome to the Address Book!\n");
	printf("\u2022 Thank You for using the coolest Address Book in the area!\n");
	printf("\u2022 All rights reserved by Arvydas Productions\u00A9\n");
	printf("\u2022 Type \"\033[1mhelp\033[0m\" for a list of commands\n");

	int signals_to_ignore[] = { SIGHUP, SIGQUIT, SIGILL, SIGTRAP, SIGABRT };

	for (int i = 0; i < sizeof(signals_to_ignore) / sizeof(signals_to_ignore[0]); i++)
		signal(signals_to_ignore[i], SIG_IGN);

	printf("\nTrying to read saved Addresses...\n");

	ReadStatus ret = load_addresses_from_a_file(list);

	if (ret == FAILED_MALLOC)
		return 1;
	else if (ret == NOT_READ)
		printf("Did not load in any addresses\n");
	else
		printf("Read was succesful\n\n");

    return 0;
}