#include <stdio.h>
#include <stdlib.h>
#include "greeting.h"
void greeting()
{
    system("dir");
}
int main(int argc, char **argv)
{
    fprintf(stdout, "%s\nHello world!!\n%s", argv[1], greeting());
    //fprintf(stdout, "Hello world\n");
    //greeting();

    return 0;
}