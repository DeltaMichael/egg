FLAGS = -g -lc -Wall -Wextra # -fsanitize=address
# O_FILES = main.o hmap.o util.o sbuilder.o arglist.o interpreter.o
O_FILES = hmap.o util.o sbuilder.o arglist.o interpreter.o

egg: scanner.l parser.y $(O_FILES)
	bison -d parser.y
	flex scanner.l
	cc $(FLAGS) -o egg parser.tab.c lex.yy.c $(O_FILES) -lfl

# egg: $(O_FILES)
# 	cc $(FLAGS) -o egg $(O_FILES)
#
# main.o: hmap.h util.h sbuilder.h
sbuilder.o: sbuilder.h
hmap.o: hmap.h
util.o: util.h sbuilder.h arglist.h
arglist.o: arglist.h
interpreter.o: interpreter.h util.h


.PHONY: clean
clean:
	rm $(O_FILES) parser.tab.c parser.tab.h lex.yy.c
