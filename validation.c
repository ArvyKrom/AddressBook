
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "validation.h"
#include "mainCode.h"

ValidationStatus is_valid_address(char *name, char *surname, char *email, char *phone_number)
{
	if (is_valid_name(name) != VALID)
		return (is_valid_name(name));

	if (is_valid_surname(surname) != VALID)
		return (is_valid_surname(surname));

	if (is_valid_email(email) != VALID)
		return (is_valid_email(email));

	if (is_valid_phone_number(phone_number) != VALID)
		return (is_valid_phone_number(phone_number));

	return VALID;
}

ValidationStatus is_valid_name(char *str)
{
	if (is_containing_only_letters(str) != VALID)
		return NAME_NOT_ALPHABETIC;
	if (is_of_right_length(str, NAME_LENGTH) != VALID)
		return WRONG_NAME_LENGTH;
	return VALID;
}

ValidationStatus is_valid_surname(char *str)
{
	if (is_containing_only_letters(str) != VALID)
		return SURNAME_NOT_ALPHABETIC;
	if (is_of_right_length(str, SURNAME_LENGTH) != VALID)
		return WRONG_SURNAME_LENGTH;
	return VALID;
}

ValidationStatus is_containing_only_letters(char *str)
{
	if (str == NULL)
		return NAME_NOT_ALPHABETIC;

	for (int i = 0; i < strlen(str); i++) {
		if (!isalpha(*(str + i)))
			return NAME_NOT_ALPHABETIC;
	}

	return VALID;
}

ValidationStatus is_of_right_length(char *str, int len)
{
	// -1 to leave atleast one '\0'
	if (strlen(str) <= (len - 1) && (strlen(str) > 0))
		return VALID;
	return WRONG_LENGTH;
}

ValidationStatus is_valid_email(char *email)
{
	if (is_matching_a_pattern(email, "^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$") != VALID)
		return INVALID_EMAIL;
	if (is_of_right_length(email, EMAIL_LENGTH) != VALID)
		return INVALID_EMAIL;
	return VALID;
}

ValidationStatus is_valid_phone_number(char *phone_number)
{
	// No need to additionally check length as the pattern matches only 12 characters (13-1)
	if (is_matching_a_pattern(phone_number, "^\\+?[0-9]{2,11}$") == VALID)
		return VALID;
	return INVALID_PHONE_NUMBER;
}

ValidationStatus is_matching_a_pattern(char *str, char *pattern)
{
	regex_t regex;
	int ret;

	if (str == NULL)
		return INVALID_EMAIL;

	ret = regcomp(&regex, pattern, REG_EXTENDED | REG_NOSUB);

	if (ret != 0)
		return INVALID_EMAIL;

	ret = regexec(&regex, str, 0, NULL, 0);
	regfree(&regex);
	if (ret != 0)
		return INVALID_EMAIL;
	return VALID;
}