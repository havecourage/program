#include <stdio.h>
#include <stdlib.h>
#include "greeting.h"
int main()
{
    greeting();
    hello();
    printf("PATH=%s", getenv("PATH"));
}