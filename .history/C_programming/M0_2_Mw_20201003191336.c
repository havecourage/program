#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define M 1.6666667 // 2/3 的近似

double M0_2_Mw(double M0)
{
    double Mw;
    Mw = M * log(M0) - 10.7;
    return Mw;
}
int main(int argc, char **argv)
{
    double M0, Mw;
    if (argc < 1)
        fprintf(stderr, "Usage:%s M0/Mw,\nPlease choose one to continue!\n", argv[0]);
    if (argv[1] == "M0")
        fscanf(stdin, "Input M0=%d\n", M0);
    else if (argv[1] == "Mw")
        fscanf(stdin, "Input Mw=%d\n", Mw);
        else fprintf("Bad ")
}