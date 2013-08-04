CC=g++
CFLAGS=
LDFLAGS=-lboost_serialization

all: client server clean

server: server.o 
	$(CC) -o $@ $^ $(LDFLAGS)

client: client.o 
	$(CC) -o $@ $^ $(LDFLAGS)

server.o: server.cpp person.hpp crc.hpp
	$(CC) -c $(CFLAGS) $<

client.o: client.cpp person.hpp crc.hpp
	$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm *.o
