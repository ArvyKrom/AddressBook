

#include <stdio.h>
#include <stdlib.h>

#include "Person.h"
#include "mainCode.h"

int main()
{
	struct Person *list = NULL;
	list		    = (struct Person *)calloc(1, sizeof(struct Person));

	if (list == NULL) {
		perror("Failed to allocate memory for a person object");
		exit(1);
	}

	ReadStatus ret = load_addresses_from_a_file(&list);
	if (ret == FAILED_CALLOC) {
		//Eiti i adresu istrynima ir visko uzdaryma
		exit(1);
	}
	display_addresses(list);
	struct Person *toBeInserted = create_new_person("Antanas", "Karbonaras", "Ontuons@gmail.com", "112");
	add_new_address_at_postion(toBeInserted, &list, 12);
	display_addresses(list);
	return 0;
}
