CFLAGS = -Wall

.PHONY: all clean

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
LIB_NAME = $(shell make --no-print-directory -C lib echo_libname)
LIB_VERSION = $(shell make --no-print-directory -C lib echo_version)

all: AddressBook

AddressBook: $(OBJ) lib/lib$(LIB_NAME).so.$(LIB_VERSION)
	 $(CC) $(OBJ) $(CFLAGS) -Wl,-rpath=./lib -L./lib -l$(LIB_NAME) -o AddressBook

$(OBJ): $(SRC)
	make --no-print-directory -C src

lib/lib$(LIB_NAME).so.$(LIB_VERSION): lib/$(LIB_NAME).c
	make --no-print-directory -C lib

clean:
	$(RM) AddressBook
	make --no-print-directory -C src clean
	make --no-print-directory -C lib clean
	


	



