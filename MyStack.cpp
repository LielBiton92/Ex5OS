#include "MyStack.hpp"
using namespace std;

int fd;
struct flock lock;

void ex4::MyStack::create_file()
{
    fd = open("lock.txt", O_WRONLY | O_CREAT);

    if (fd == -1)
    {
        printf("Error Number % d\n", errno);

        perror("Failed");
    }
    memset(&lock, 0, sizeof(lock));
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
        throw ::invalid_argument("DEBUG:string size has to be less then 1025.");
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &lock);
    }
    ex4::Node *newnode = (ex4::Node *)malloc(sizeof(ex4::Node));
    newnode->data = (char *)malloc(strlen(val) + 1);
    strcpy(newnode->data, val);
    newnode->next = this->head;
    this->head = newnode;
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
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
        free(temp->data);
        free(temp);
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
}

char *ex4::MyStack::TOP()
{
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);
    if (this->head == NULL)
    {
        char *ans = (char *)malloc(25);
        strcpy(ans, "DEBUG:Stack is empty.");
        return ans;
    }
    else
    {
        char *ans = (char *)malloc(strlen(this->head->data) + 10);
        strcpy(ans, "OUTPUT:");
        strcat(ans, this->head->data);
        return ans;
    }
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
}