#include <iostream>
#include <pthread.h>
#include <string.h>
#include "MyStack.hpp"
using namespace std;
using namespace ex4;
//https://www.informit.com/articles/article.aspx?p=23618&seqNum=4
int fd;
struct flock lock;
int createFile(){
    fd = open("Helper2.txt", O_WRONLY | O_CREAT);


    if (fd == -1)
    {
        printf("Error Number % d\n", errno);
        perror("Program");
    }
    memset(&lock, 0, sizeof(lock));
    return fd;
}


ex4::MyStack::MyStack()
{
    ex4::Node *p = NULL;
    this->head = p;
}

void ex4::MyStack::PUSH(char *val)
{
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);
    if (strlen(val) > 1024)
    {
        lock.l_type = F_UNLCK;
        fcntl (fd, F_SETLKW, &lock);
        throw ::invalid_argument("DEBUG:string size has to be less then 1025.");
    }
    ex4::Node *newnode = (ex4::Node *)MyMemory::my_malloc(sizeof(ex4::Node));
    newnode->data = (char *)MyMemory::my_malloc(strlen(val) + 1);
    strcpy(newnode->data, val);
    newnode->next = this->head;
    this->head = newnode;
    lock.l_type = F_UNLCK;
    fcntl (fd, F_SETLKW, &lock);
}

void ex4::MyStack::POP()
{
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);
    if (this->head == NULL)
        cout << "DEBUG:stack is empty." << endl;
    else
    {
        ex4::Node *temp = this->head;
        this->head = this->head->next;
        MyMemory::my_free(temp->data);
        MyMemory::my_free(temp);
    }
    lock.l_type = F_UNLCK;
    fcntl (fd, F_SETLKW, &lock);
}

char *ex4::MyStack::TOP()
{
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);
    if (this->head == NULL)
    {
        char *ans = (char *)MyMemory::my_malloc(25);
        strcpy(ans, "DEBUG:Stack is empty.");
        lock.l_type = F_UNLCK;
        fcntl (fd, F_SETLKW, &lock);
        return ans;
    }
    else
    {
        char *ans = (char *)MyMemory::my_malloc(strlen(this->head->data)+10);
        strcpy(ans, "OUTPUT:");
        strcat(ans, this->head->data);
        lock.l_type = F_UNLCK;
        fcntl (fd, F_SETLKW, &lock);
        return ans;
    }
}