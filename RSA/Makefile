rsa: rsa.o liste.o
	gcc rsa.o liste.o -o rsa -lgmp

rsa.o: rsa.c liste.h
	gcc -g -c rsa.c

liste.o: liste.c
	gcc -g -c liste.c

clean:
	rm -f *.o
	rm -f rsa