FLAGS = -lc -Wall -Wextra
O_FILES = main.o

egg: $(O_FILES)
	cc $(FLAGS) -o egg main.o

main.o: main.c
	cc $(FLAGS) -c main.c

.PHONY: clean
clean:
	rm $(O_FILES)
