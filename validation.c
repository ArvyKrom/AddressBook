
#include <string.h>
#include "validation.h"
#include "mainCode.h"

ValidationStatus is_valid_address(char *name, char *surname, char *email, char *phone_number)
{
	if (is_of_right_length(name, NAME_LENGTH) != VALID)
		return WRONG_NAME_LENGTH;

	if (is_of_right_length(surname, SURNAME_LENGTH) != VALID)
		return WRONG_SURNAME_LENGTH;

	if (is_of_right_length(email, EMAIL_LENGTH) != VALID)
		return WRONG_EMAIL_LENGTH;

	if (is_of_right_length(phone_number, PHONE_NUMBER_LENGTH) != VALID)
		return WRONG_PHONE_NUMBER_LENGTH;

	return VALID;
}

ValidationStatus is_of_right_length(char *str, int len)
{
	if (str == NULL) {
		return WRONG_LENGTH;
	}
	// -1 to leave atleast one '\0'
	if ((strlen(str) <= len - 1) && str != NULL)
		return VALID;
	return WRONG_LENGTH;
}