#This one compiles every .c file in the src folder

CPPFLAGS = -I../include
CFLAGS = -Wall

.PHONY: all clean

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(OBJ)

clean:
	$(RM) *.o

