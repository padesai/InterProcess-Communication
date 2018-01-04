#makefile file for the client server application showcasing a server 
#and 2 clients calling the server in sync and aync mode 

#variable declaration
cc=gcc
MAKE=make
RM=rm
 
#targets
all:Server Client

Server: Server_Final.c 
	$ cc -o Server Server_Final.c snappy.c library.c -lrt

Client: Client_Final.c Server_Final.c
	$ cc -o Client Client_Final.c snappy.c library.c -lrt




clean:
	@rm -f Client Server
