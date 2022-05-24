

CC=g++
FLAGS=-Wall -pthread

all: ex4

ex4: server.o MyMemory.o MyStack.o client
	$(CC) $(FLAGS)  server.o MyMemory.o MyStack.o -o server

server.o: server.cpp
	$(CC) $(FLAGS) -c server.cpp

MyStack.o: MyStack.cpp MyStack.hpp
	$(CC) $(FLAGS) -c MyStack.cpp

MyMemory.o: MyMemory.cpp MyMemory.hpp
	$(CC) $(FLAGS) -c MyMemory.cpp

client: client.cpp
	$(CC) $(FLAGS) -o client client.cpp MyMemory.cpp 



clean:
	rm -f *.o .a server client ex4 