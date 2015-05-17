### RC Makefile ###
### --CRC 2015  ###

CC = gcc
DEBUG = -g
CFLAGS = -Wall -c 
CLIENT_DIR = client/
SERVER_DIR = server/
OBJ_DIR = obj/

all: client server

client: client.o
	$(CC) client.o -o $(OBJ_DIR)client

client.o: $(CLIENT_DIR)client.c
	$(CC) $(CFLAGS) $(CLIENT_DIR)client.c

server: server.o
	$(CC) server.o -o $(OBJ_DIR)server

server.o: $(SERVER_DIR)server.c
	$(CC) $(CFLAGS) $(SERVER_DIR)server.c

clean:
	rm $(OBJ_DIR)*
