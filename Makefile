default: c

all: c

CC=gcc-9

c: server-c client-c

client-c: client-c.c
	$(CC) client-c.c -o client-c

server-c: server-c.c
	$(CC) server-c.c -o server-c

clean:
	rm -f server-c client-c *.o
