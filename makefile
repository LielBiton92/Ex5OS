CC=g++
FLAGS=-Wall -pthread

all: ex4

ex4: server.o MyStack.o client
	$(CC) $(FLAGS)  server.o MyStack.o -o server

server.o: server.cpp
	$(CC) $(FLAGS) -c server.cpp

MyStack.o: MyStack.cpp MyStack.hpp
	$(CC) $(FLAGS) -c MyStack.cpp

client: client.cpp
	$(CC) $(FLAGS) -o client client.cpp 


clean:
	rm -f *.o .a server client ex4 lock.txt