#include "Std.h"
#include "StdExt.h"

void Func1();
void Func2();
void (*Func)() = &Func1;

void Func1()
{
    printf("Inited\n");
    printf("Calc\n");
    Func = Func2;
}

void Func2()
{
    printf("Calc\n");
}

int main()
{
    Func();
    Func();
    Func();
    Func();

    return 0;
}
