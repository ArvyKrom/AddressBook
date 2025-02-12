CC = cc
RM = rm -f
#To make the output file "lighter" omit the CFLAGS
CFLAGS = -Wall -g

#Collect all .c files
SRC = $(wildcard *.c)
#Get the names of the object files (*.o)
OBJ = $(SRC:.c=.o)

all: AddressBook

AddressBook: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o AddressBook

# Compile file by file if their .o file does not exist or timestamp is too recent
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

# If a "clean" file is created this will stay as a target
.PHONY: clean
clean:
	$(RM) *.o
	$(RM) AddressBook


