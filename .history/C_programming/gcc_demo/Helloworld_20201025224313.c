#include <stdio.h>
#include <stdlib.h>
#include "greeting.h"
void greeting()
{
    system("dir");
}
int main(int argc,char **argv)
{
    printf("Hello world\n");
    greeting();
    if(argc)
    return 0;
}