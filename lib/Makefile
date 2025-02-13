#This one creates the shared library

VERSION = 1.0.0
LIB_NAME = linked_list
CPPFLAGS = -I../include
CFLAGS = -Wall -fPIC

.PHONY: all clean

MAJOR_VERSION = $(shell expr substr $(VERSION) 1 1)
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: lib$(LIB_NAME).so.$(VERSION)

# Needed .o files will be create with the implicit rule for .o creation
lib$(LIB_NAME).so.$(VERSION): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -shared -Wl,-soname,lib$(LIB_NAME).so -o lib$(LIB_NAME).so.$(VERSION)
	(ln -sf lib$(LIB_NAME).so.$(VERSION) lib$(LIB_NAME).so)
	(ln -sf lib$(LIB_NAME).so.$(VERSION) lib$(LIB_NAME).so.$(MAJOR_VERSION))


echo_libname:
	@echo $(LIB_NAME)
echo_version:
	@echo $(VERSION)

clean:
	$(RM) *.o
	$(RM) lib$(LIB_NAME).so.$(VERSION)
	$(RM) lib$(LIB_NAME).so
	$(RM) lib$(LIB_NAME).so.$(MAJOR_VERSION)
	