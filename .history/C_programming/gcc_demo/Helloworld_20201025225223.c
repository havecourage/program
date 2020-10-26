#include <stdio.h>
#include <stdlib.h>
#include "greeting.h"
void greeting()
{
    system("dir");
}
int main(int argc, char **argv)
{
    fprintf(stdout, "%s\n", argv[1]);
    //fprintf(stdout, "Hello world\n");
    //greeting();

    return 0;
}