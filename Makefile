CC:= gcc
CFLAGS:= -Wall -O2
LDFLAGS:= -lpthread -ldl

.PHONY: clean all

all: server client 

server: server.o fun.o queue.o thread_pool.o
	$(CC) -o $@ $^ $(LDFLAGS)

client: client.o 
	$(CC) -o $@ $^ $(LDFLAGS)

%*.o: %*.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm server client *.o


