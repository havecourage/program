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
    double M0;
    if(argc<1)fprintf(stderr,"Usage:%s M0/Mw")
    fscanf(stdin, "Input M0=%d\n", M0);
}