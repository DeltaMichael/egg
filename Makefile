FLAGS = -lc -Wall -Wextra
O_FILES = main.o hmap.o util.o

egg: $(O_FILES)
	cc $(FLAGS) -o egg $(O_FILES)

main.o: main.c hmap.h
hmap.o: hmap.c hmap.h
util.o: util.c util.h

.PHONY: clean
clean:
	rm $(O_FILES)
