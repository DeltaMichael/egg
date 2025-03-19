FLAGS = -g -lc -Wall -Wextra # -fsanitize=address
O_FILES = main.o hmap.o util.o sbuilder.o arglist.o

egg: $(O_FILES)
	cc $(FLAGS) -o egg $(O_FILES)

main.o: hmap.h util.h sbuilder.h
sbuilder.o: sbuilder.h
hmap.o: hmap.h
util.o: util.h sbuilder.h arglist.h
arglist.o: arglist.h

.PHONY: clean
clean:
	rm $(O_FILES)
