/*
测试c语言中宏的使用的程序
#if #ifdef  #ifndef
*/
#include <stdio.h>
#include <stdlib.h>

int main()
{
#if DAMN
    printf("DAMN if true!");
#else
    printf("DAMN is false");
#endif
}