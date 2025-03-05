egg: main.o
	cc -lc -o egg main.o

main.o: main.c
	cc -lc -Wall -Wextra -c main.c

clean:
	rm main.o egg
