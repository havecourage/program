#include <stdio.h>
int main()
{
    FILE *fp;                         //创建文件指针
    fp = fopen("file_demo.txt", "w"); //打开文件
    fprintf(stdout, "abcd\n");        //将结果输出到标准输出
    fprintf(fp, "edf\n");
    fclose(fp); //关闭文件

    return 0;
}