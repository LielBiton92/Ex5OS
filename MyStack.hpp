#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

namespace ex4
{
    class Node
    {
    public:
        char *data;
        Node *next;
    };

    class MyStack
    {

    private:
        Node *head;

    public:
        MyStack();
        void PUSH(char *val);
        void POP();
        char *TOP();
        static void create_file();
    };
}
