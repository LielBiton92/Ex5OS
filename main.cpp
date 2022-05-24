#include "MyStack.hpp"
#include <iostream>
# include <stdlib.h>
#include <string.h>

using namespace ex4;


int main() {
    MyStack s;
    char* a = (char*)ex4::MyMemory::my_malloc(2);
    a[0] = 'h';
    a[1] = 'i';
    s.PUSH(a);

    std::cout<<s.TOP();
    std::cout<<strlen(s.TOP());
    s.POP();
    std::cout<<s.TOP();
    
}