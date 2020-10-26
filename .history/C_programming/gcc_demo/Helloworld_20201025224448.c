#include <stdio.h>
#include <stdlib.h>
#include "greeting.h"
void greeting()
{
    system("dir");
}
int main(int argc, char **argv)
{
    if (argc < 1)
    {
        fprintf(stderr, "Too few input!\n");
        return -1;
    }
    if(argv[1]=="CPU")
    {
        fprintf(stdout)
    }
    printf("Hello world\n");
    greeting();

    return 0;
}