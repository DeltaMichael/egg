FLAGS = -lc -Wall -Wextra
O_FILES = main.o hmap.o util.o sbuilder.o

egg: $(O_FILES)
	cc $(FLAGS) -o egg $(O_FILES)

main.o: hmap.h util.h
sbuilder.o: sbuilder.h
hmap.o: hmap.h
util.o: util.h sbuilder.h

.PHONY: clean
clean:
	rm $(O_FILES)
