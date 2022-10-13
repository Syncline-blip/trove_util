# Makefile for trove utility program

PROJECT	= trove
HEADERS = $(PROJECT).h
OBJ 	= trove.o finder.o hashtable.o list.o support.o

C11 = cc -std=c11
CFLAGS = -Wall -Werror

$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ)


%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)