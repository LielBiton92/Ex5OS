/*
** server.c -- a stream socket server demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include "MyStack.hpp"
using namespace ex4;
#define PORT "3498" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

ex4::MyStack *server_stack;

int fd3;
struct flock lock3;
int createFile3(){
    fd3 = open("Helper.txt", O_WRONLY | O_CREAT);


    if (fd3 == -1)
    {
        printf("Error Number % d\n", errno);
        perror("Program");
    }
    memset(&lock3, 0, sizeof(lock3));
    return fd3;
}

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

bool prefix(const char *pre, const char *str)
{
    char cp;
    char cs;

    if (!*pre)
        return true;

    while ((cp = *pre++) && (cs = *str++))
    {
        if (cp != cs)
            return false;
    }

    if (!cs)
        return false;

    return true;
}

void *PROCESS_FUNCTION(void *new_fdcl)
{
    int th_cl = *(int *)new_fdcl;
    printf("new client connect to server %d\n", th_cl);
    if (send(th_cl, "Hello client!", 13, 0) == -1)
    {
        perror("send");
    }
    char *input;
    size_t buf_size = 1024;
    while (true)
    {
        input = (char *)ex4::MyMemory::my_calloc(buf_size, sizeof(char));
        if (recv(th_cl, input, buf_size, 0) != 0)
        {
            if (prefix("PUSH", input))
            {
                char* substr = (char*)ex4::MyMemory::my_malloc(strlen(input));
                strncpy(substr, input+4, strlen(input)-4);
                server_stack->PUSH(substr);
            }
            else if (prefix("POP", input))
            {
                server_stack->POP();
            }
            else if (prefix("TOP", input))
            {
                std::cout << server_stack->TOP() << std::endl;
            }
            else if (prefix("QUIT", input))
            {
                ex4::MyMemory::my_free(input);
                break;

            }else {
                std::cout <<"received : " << input <<std::endl ;
            }
        }
            
    }
    close(th_cl);
    printf("finish task %d\n", th_cl);
    return NULL;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void)
{
     fd3 = createFile3();

    server_stack = (MyStack*)mmap(NULL, sizeof(MyStack), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1)
    { // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);


        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            PROCESS_FUNCTION(&new_fd);
            close(new_fd);
            close(fd3);
            exit(0);
        }
        close(new_fd);

    close(fd3);
    }
    return 0;
}