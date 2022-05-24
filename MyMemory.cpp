#include <iostream>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "MyMemory.hpp"
using namespace ex4;

int fd1;
struct flock lock1;
void createFile2(){
    fd1= open("Helper1.txt", O_WRONLY | O_CREAT);


    if (fd1 == -1)
    {
        printf("Error Number % d\n", errno);
        perror("Program");
    }
    memset(&lock1, 0, sizeof(lock1));
}




static free_block free_block_list_head = {0, 0};
static const size_t overhead = sizeof(size_t);
static const size_t align_to = 16;

void *MyMemory::my_malloc(size_t size)
{
    lock1.l_type = F_WRLCK;
    fcntl(fd1, F_SETLKW, &lock1);
    void *to_return = my_malloc_helper(size);
    lock1.l_type = F_UNLCK;
    fcntl (fd1, F_SETLKW, &lock1);
    return to_return;
}

void *MyMemory::my_malloc_helper(size_t size)
{
    size = (size + sizeof(size_t) + (align_to - 1)) & ~(align_to - 1);
    free_block *block = free_block_list_head.next;
    free_block **head = &(free_block_list_head.next);
    while (block != 0)
    {
        if (block->size >= size)
        {
            *head = block->next;
            return ((char *)block) + sizeof(size_t);
        }
        head = &(block->next);
        block = block->next;
    }

    block = (free_block *)sbrk(size);
    block->size = size;

    return ((char *)block) + sizeof(size_t);
}

void MyMemory::my_free(void *ptr)
{
    lock1.l_type = F_WRLCK;
    fcntl(fd1, F_SETLKW, &lock1);
    my_free_helper(ptr);
    lock1.l_type = F_UNLCK;
    fcntl (fd1, F_SETLKW, &lock1);
}

void MyMemory::my_free_helper(void *ptr)
{
    free_block *block = (free_block *)(((char *)ptr) - sizeof(size_t));
    block->next = free_block_list_head.next;
    free_block_list_head.next = block;
}



void *MyMemory::my_calloc(size_t n, size_t size)
{
    size_t total = n * size;
    void *p = MyMemory::my_malloc(total);

    if (!p)
        return NULL;

    return memset(p, 0, total);
}
