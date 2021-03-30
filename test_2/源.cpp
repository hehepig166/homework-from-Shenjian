#include <cstdio>

void fun(int x)
{
    printf("fun %d\n",x);
}

int main()
{
    void (*a)(int);
    a = fun;
}