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
double Mw_2_M0(double Mw)
{
    double M0;
    M0 = exp((Mw + 10.7) / M);
    return M0;
}
int main(int argc, char **argv)
{
    double M0, Mw;
    if (argc < 1)
    {
        fprintf(stderr, "Usage:%s M0/Mw,\nPlease choose one to continue!\n", argv[0]);
        return -1;
    }
    else if{if (argv[1] == "M0")
    {
        fscanf(stdin, "Input M0=%d\n", M0);
        Mw = M0_2_Mw(M0);
        fprintf(stdout, "The corresponding Mw is %d\n", Mw);
        return 0;
    }
    else if (argv[1] == "Mw")
    {
        fscanf(stdin, "Input Mw=%d\n", Mw);
        M0 = Mw_2_M0(Mw);
        fprintf(stdout, "The corresponding M0 is %d", M0);
        return 0;
    }
    else
    {
        fprintf(stderr, "Bad input!\n");
        return -1;
    }
}