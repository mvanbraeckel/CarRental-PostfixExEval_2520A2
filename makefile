all: q1 q2

q1: q1.c
	gcc -Wall -pedantic -std=c99 q1.c -o q1

q2: q2.c
	gcc -Wall -pedantic -std=c99 q2.c -o q2

clean:
	rm -f *.o q2 all
