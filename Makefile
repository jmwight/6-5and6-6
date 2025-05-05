$OBJ = dict.o 

6-6: dict.o getword

.PHONY: getword
getword:
	$(MAKE) -C getword/

.PHONY: clean
clean:
	rm -rf *.o
	$(MAKE) -C getword/ clean
