main:main.o socket.o database.o
	gcc main.o socket.o database.o -o main -lsqlite3 -lpthread
socket.o:socket.c
	gcc -c socket.c
main.o:main.c
	gcc -c main.c
database.o:database.c
	gcc -c database.c
