#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include "MyMemory.hpp"

namespace ex4
{
    int createFile();    
    class Node
    {
    public:
        char* data;
        Node *next;
    };



    class MyStack
    {

    private:
        Node *head;

    public:
        MyStack();
        void PUSH(char* val);
        void POP();
        char* TOP();
    };
}
