all: server client

server: server.o Card.o
	gcc server.o Card.o -o server

client: client.o Card.o
	gcc client.o Card.o -o client

server.o: server.c
	gcc -c server.c Card.c

client.o: client.c
	gcc -c client.c

Card.o: Card.h
	gcc -c Card.h

clean:
	rm *.o server client
