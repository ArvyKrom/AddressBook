#ifndef VALIDATION_H
#define VALIDATION_H

typedef enum {
	VALID,
	WRONG_NAME_LENGTH,
	NAME_NOT_ALPHABETIC,
	WRONG_SURNAME_LENGTH,
	SURNAME_NOT_ALPHABETIC,
	WRONG_LENGTH,
	INVALID_EMAIL,
	INVALID_PHONE_NUMBER
} ValidationStatus;

ValidationStatus is_valid_address(char *name, char *surname, char *email, char *phone_number);
ValidationStatus is_valid_name(char *str);
ValidationStatus is_valid_surname(char *str);
ValidationStatus is_valid_email(char *str);
ValidationStatus is_valid_phone_number(char *str);
ValidationStatus is_containing_only_letters(char *str);
ValidationStatus is_matching_a_pattern(char *str, char *pattern);
ValidationStatus is_of_right_length(char *str, int len);

#endif