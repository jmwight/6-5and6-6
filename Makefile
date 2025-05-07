OBJ = dict.o getword/*.o
ARG = -g3

6-6: dict.o getword main.c
	gcc ${ARG} -o 6-6 ${OBJ} main.c 

dict.o: dict.c dict.h
	gcc ${ARG} -c dict.c

.PHONY: getword
getword:
	$(MAKE) -C getword/

.PHONY: clean
clean:
	rm -rf *.o
	$(MAKE) -C getword/ clean
