CC=gcc
CFLAGS=-Wall -g

OBJ=client.o server.o

all: client server

client: $(OBJ)
	$(CC) -o client client.o 

server: $(OBJ)
	$(CC) -pthread -lpthread -o server server.o 

client.o: client.c
	$(CC) -c $(CFLAGS) -pthread -lpthread  client.c -o client.o

server.o: server.c
	$(CC) -c $(CFLAGS) server.c -o server.o 


.PHONY: clean
clean:
	rm -f $(OBJ) client server