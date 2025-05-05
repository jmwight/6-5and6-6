ARGS := -g3
#OBJ := getword.o getch.o  # easier way
OBJ := $(patsubst %.c,%.o,$(wildcard *.c))  # alternative replace all .c with .o matching any .c
					    # according to wildcard documentation 4.4.3

.PHONY: all
all: ${OBJ} 

getword.o: getword.c getch.h getword.h
	gcc ${ARGS} -c getword.c

getch.o: getch.c
	gcc ${ARGS} -c getch.c

.PHONY: clean
clean:
	rm -rf ${OBJ}
