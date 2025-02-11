#ifndef VALIDATION_H
#define VALIDATION_H

typedef enum {
	VALID,
	WRONG_NAME_LENGTH,
	WRONG_SURNAME_LENGTH,
	WRONG_EMAIL_LENGTH,
	WRONG_PHONE_NUMBER_LENGTH,
	WRONG_LENGTH
} ValidationStatus;

ValidationStatus is_valid_address(char *name, char *surname, char *email, char *phone_number);
ValidationStatus is_of_right_length(char *str, int len);

#endif