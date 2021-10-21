#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define pi 3.1415926

double **malloc2(int n1,int n2);
double ***malloc3(int n1,int n2,int n3);
void free2(double **memory2);
void free3(double ***memory3,int n1,int n2,int n3);
void free4(double ****memory4,int n1,int n2,int n3,int n4);


int main()
{
    int i,j,k,m,y;

/************************************参数设置*****************************************************/
    double tmax=60;      /*波场记录时间*/
    double dt=0.004;     /*采样间隔*/
    double f=20;   /*子波主频*/
    double T;    /*子波周期*/
    int nt;    /*采样点数*/
    int nx=37*5;   /*x方向网格点数*/
    int ny=10;   /*y方向网格点数*/
    int nz=100;   /*z方向网格点数*/
    int bx=5;     /*x方向步长（网格间距）*/
    int by=1;     /*x方向步长（网格间距）*/
    int bz=1;     /*z方向步长（网格间距）*/
    int sx=10;   /*震源x坐标*/
    int sy=2;   /*震源y坐标*/
    int sz=0;   /*震源z坐标*/
    int ry=0;   /*接收线深度y坐标*/
    int rz=0;   /*接收线深度z坐标*/
    int n=12;  /*差分空间阶数*/
    int H=5;   /*NPML边界厚度*/
    double d0=3000;    /*最大衰减参数*/
    double mt=0.01;   /*波场快照间隔/s*/


/****************************地质建模*********************/
    double ***vp,***vs,***p,***lame1,***lame2;
    FILE *fp_vp;
    FILE *fp_vs;
    FILE *fp_p;
    FILE *fp_lame1;
    FILE *fp_lame2;

    fp_vp=fopen("vp.dat","w+");
    fp_vs=fopen("vs.dat","w+");
    fp_p=fopen("p.dat","w+");
    fp_lame1=fopen("lame1.dat","w+");
    fp_lame2=fopen("lame2.dat","w+");
	double elev[] = {0.248, 0, 0.093, 0.065, 0.086, 0.17, 0.155, 0.17, 0.57, 1.012, 1.479, 1.933, 2.354, 2.535, 2.576, 2.488, 2.553, 2.59, 2.576, 2.62, 2.575, 2.594, 2.558, 2.569, 2.56, 2.546, 2.506, 2.53, 2.516, 2.575, 2.55, 2.473, 2.541, 2.623, 2.605, 2.593, 2.606};

    vp=(double***)malloc3(nx,ny,nz);
    vs=(double***)malloc3(nx,ny,nz);
    p=(double***)malloc3(nx,ny,nz);
    lame1=(double***)malloc3(nx,ny,nz);
    lame2=(double***)malloc3(nx,ny,nz);

    for (i = 0; i < nx; i++)
    {
        for (y = 0; y < ny; y++)
        {
            for (j = 0; j < nz; j++)
            {
                if (j < H)
                {
                    vp[i][y][j] = 600;
                    vs[i][y][j] = 400;
                    p[i][y][j] = 500;
                }
                else if (j < nz / 3)
                {
                    vp[i][y][j] = 600;
                    vs[i][y][j] = 400;
                    p[i][y][j] = 1000;
                }
                else if (j < 2 * nz / 3)
                {
                    vp[i][y][j] = 900;
                    vs[i][y][j] = 700;
                    p[i][y][j] = 1200;
                }
                else
                {
                    vp[i][y][j] = 1200;
                    vs[i][y][j] = 800;
                    p[i][y][j] = 1600;
                }
                fprintf(fp_vp, "%lf ", vp[i][y][j]); /*绾垫尝閫熷害鏂囦欢*/
                fprintf(fp_vs, "%lf ", vs[i][y][j]); /*妯尝閫熷害鏂囦欢*/
                fprintf(fp_p, "%lf ", p[i][y][j]);   /*瀵嗗害鏂囦欢*/

                lame1[i][y][j] = vs[i][y][j] * vs[i][y][j] * p[i][y][j];
                lame2[i][y][j] = vp[i][y][j] * vp[i][y][j] * p[i][y][j] - 2 * vs[i][y][j] * vs[i][y][j] * p[i][y][j]; /*涓や釜鎷夋甯告暟*/
            }
            fprintf(fp_vp, "\n");
            fprintf(fp_vs, "\n");
            fprintf(fp_p, "\n");
        }
        fprintf(fp_vp, "\n");
        fprintf(fp_vs, "\n");
        fprintf(fp_p, "\n");
    }

    fclose(fp_vp);
    fclose(fp_vs);
    free3(vp,nx,ny,nz);
    free3(vs,nx,ny,nz);
    printf("地质建模完成\n");


/************************************地震子波*********************************************/
    double t;  /*采样时间*/
    double *src;/*地震雷克子波函数*/
    double ntf; /*中间变量*/
    FILE *fp_src;
    FILE *fp_t;

    fp_src=fopen("src.dat","w+");
    fp_t=fopen("t.dat","w+");
    ntf = tmax / dt;
    nt=(int)ntf +1 ;
    T=1/f;
    printf("%d\n",nt);
    printf("%f\n",T);
    src=(double*)malloc(nt*sizeof(double));
    for(i=0;i<nt;i++)
    {
       t=i*dt;/*采样时间=采样点*采样间隔*/
       src[i]=(1-2*pow(pi*f*(t-T),2))*exp(-(pow(pi*f*(t-T),2)));/*雷克子波表达式*/
    }
    printf("地震子波完成\n");

/******************************************差分系数************************************************/
    double **am;
    double am0;
    FILE *fp_am;

    am=(double**)malloc2((n*2),(n*2));
    fp_am=fopen("am.dat","w+");

    for(i=1; i<n+1; i++)
    {
        for(j=1; j<i+1; j++)
        {
            am[i][j]=0;
        }
    }
    if(n==1)
    {
        am[1][1]=1.0;
    }

    if(n==2)
    {
        am[1][1]=1.0;
        am[2][1]=9.0/8.0;
        am[2][2]=-(1.0/24.0);
    }

    if(n>2)
    {
        am[1][1]=1.0;
        am[2][1]=9.0/8.0;
        am[2][2]=-(1.0/24.0);

        for(j=3; j<n+1; j++)
        {
            for(m=1; m<j+1; m++)
            {
                am0=1.0;
                for(i=1; i<j+1; i++)
                {
                    if(i!=m)
                    {
                        am0=am0*fabs((2.0*i-1.0)*(2.0*i-1.0)/((2.0*m-1.0)*(2.0*m-1.0)-(2.0*i-1.0)*(2.0*i-1.0)));
                    }
                }
                am[j][m]=(pow((-1.0),(m+1.0)))/(2.0*m-1.0)*am0;
            }
        }
    }

        printf("差分系数完成\n");



/******************************************NPML边界衰减因子************************************************/
    int h;
    double *dx1,*dz1,*dx2,*dz2,*dy1,*dy2;

    dx1=(double*)malloc(nx*sizeof(double));
    dz1=(double*)malloc(nz*sizeof(double));
    dx2=(double*)malloc(nx*sizeof(double));
    dz2=(double*)malloc(nz*sizeof(double));
    dy1=(double*)malloc(ny*sizeof(double));
    dy2=(double*)malloc(ny*sizeof(double));


    printf("%lf\n",d0);
    for(i=0;i<nx;i++)
    {
        if(i<H+1)
        {
             h=H-i;
             dx1[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
        if(i>H&i<nx-H-1)
        {
            h=0;
            dx1[i]=0;
        }
        if(i>nx-H-2)
        {
            h=i-nx+H+1;
            dx1[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
    }
    for(i=0;i<ny;i++)
    {
        if(i<H+1)
        {
             h=H-i;
             dy1[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
        if(i>H&i<ny-H-1)
        {
            h=0;
            dy1[i]=0;
        }
        if(i>ny-H-2)
        {
            h=i-ny+H+1;
            dy1[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
    }

     for(i=0;i<nz;i++)
    {
        if(i<H+1)
        {
             h=H-i;
             dz1[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
        if(i>H&i<nz-H-1)
        {
            h=0;
            dz1[i]=0;
        }
        if(i>nz-H-2)
        {
            h=i-nz+H+1;
            dz1[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
    }
    for(i=0;i<nx-1;i++)
    {
        if(i<H+1)
        {
             h=H-i;
             dx2[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
        if(i>H&i<nx-H-2)
        {
            h=0;
            dx2[i]=0;
        }
        if(i>nx-H-3)
        {
            h=i-nx+H;
            dx2[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
    }
    for(i=0;i<ny-1;i++)
    {
        if(i<H+1)
        {
             h=H-i;
             dy2[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
        if(i>H&i<ny-H-2)
        {
            h=0;
            dy2[i]=0;
        }
        if(i>ny-H-3)
        {
            h=i-ny+H;
            dy2[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
    }

     for(i=0;i<nz-1;i++)
    {
        if(i<H+1)
        {
             h=H-i;
             dz2[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
        if(i>H&i<nz-H-2)
        {
            h=0;
            dz2[i]=0;
        }
        if(i>nz-H-3)
        {
            h=i-nz+H;
            dz2[i]=d0*pow(((h*1.0)/(H*1.0)),4);
        }
    }
    printf("衰减因子完成\n");


/******************************************波动方程************************************************/
    double dt_snap0=mt/dt;
    int dt_snap=(int)dt_snap0+1;
    double ***taoxxzz;
    taoxxzz=(double***)malloc3(nx,ny,nz);

    double ***taoxx,***taozz,***taozx,***taoyy,***taoyx,***taoyz,***vx,***vz,***vy;
    double ***d_taoxx,***d_taoxy,***d_taoxz,***d_taozz,***d_taozx,***d_taozy,***d_taoyy,***d_taoyx,***d_taoyz;
    double ***d_vxx,***d_vxz,***d_vxy,***d_vzz,***d_vzx,***d_vzy,***d_vyx,***d_vyy,***d_vyz;
    double ***pxx,***pxz,***pxy,***pzz,***pzx,***pzy,***pyx,***pyy,***pyz;
    double ***qxx,***qxz,***qxy,***qzz,***qzx,***qzy,***qyx,***qyy,***qyz;
    double ***ux,***uy,***uz;
    double ***wyx,***wzx,***wyz;
    double **rec_vx,**rec_vy,**rec_vz,**rec_wzx,**rec_wyx,**rec_wyz;

    FILE *fp_vx_snap;
    FILE *fp_vy_snap;
    FILE *fp_vz_snap;
    FILE *fp_wzx_snap;
    FILE *fp_wyx_snap;
    FILE *fp_wyz_snap;
    FILE *fp_rec_vx;
    FILE *fp_rec_vy;
    FILE *fp_rec_vz;
    FILE *fp_rec_wzx;
    FILE *fp_rec_wyx;
    FILE *fp_rec_wyz;
    FILE *rec_vx_bin;
    FILE *rec_vy_bin;
    FILE *rec_vz_bin;
    fp_rec_vx=fopen("rec_vx.dat","w+");
    fp_rec_vy=fopen("rec_vy.dat","w+");
    fp_rec_vz=fopen("rec_vz.dat","w+");
    fp_rec_wzx=fopen("rec_wzx.dat","w+");
    fp_rec_wyx=fopen("rec_wyx.dat","w+");
    fp_rec_wyz=fopen("rec_wyz.dat","w+");
    rec_vx_bin=fopen("rec_vx_bin.dat","wb+");
    rec_vy_bin=fopen("rec_vy_bin.dat","wb+");
    rec_vz_bin=fopen("rec_vz_bin.dat","wb+");
    fp_vx_snap=fopen("vx_snap.dat","w+");
    fp_vy_snap=fopen("vy_snap.dat","w+");
    fp_vz_snap=fopen("vz_snap.dat","w+");
    fp_wzx_snap=fopen("wzx_snap.dat","w+");
    fp_wyx_snap=fopen("wyx_snap.dat","w+");
    fp_wyz_snap=fopen("wyz_snap.dat","w+");
    printf("文件建立成功\n");

    taoxx=(double***)malloc3(nx,ny,nz);
    taozz=(double***)malloc3(nx,ny,nz);
    taoyy=(double***)malloc3(nx,ny,nz);
    taozx=(double***)malloc3(nx,ny,nz);
    taoyx=(double***)malloc3(nx,ny,nz);
    taoyz=(double***)malloc3(nx,ny,nz);
    vx=(double***)malloc3(nx,ny,nz);
    vy=(double***)malloc3(nx,ny,nz);
    vz=(double***)malloc3(nx,ny,nz);
    ux=(double***)malloc3(nx,ny,nz);
    uy=(double***)malloc3(nx,ny,nz);
    uz=(double***)malloc3(nx,ny,nz);
    wyx=(double***)malloc3(nx,ny,nz);
    wyz=(double***)malloc3(nx,ny,nz);
    wzx=(double***)malloc3(nx,ny,nz);

    printf("tao_v\n");

    d_taoxx=(double***)malloc3(nx,ny,nz);
    d_taoxy=(double***)malloc3(nx,ny,nz);
    d_taoxz=(double***)malloc3(nx,ny,nz);
    d_taoyx=(double***)malloc3(nx,ny,nz);
    d_taoyy=(double***)malloc3(nx,ny,nz);
    d_taoyz=(double***)malloc3(nx,ny,nz);
    d_taozx=(double***)malloc3(nx,ny,nz);
    d_taozy=(double***)malloc3(nx,ny,nz);
    d_taozz=(double***)malloc3(nx,ny,nz);
    d_vxx=(double***)malloc3(nx,ny,nz);
    d_vxy=(double***)malloc3(nx,ny,nz);
    d_vxz=(double***)malloc3(nx,ny,nz);
    d_vyx=(double***)malloc3(nx,ny,nz);
    d_vyy=(double***)malloc3(nx,ny,nz);
    d_vyz=(double***)malloc3(nx,ny,nz);
    d_vzx=(double***)malloc3(nx,ny,nz);
    d_vzy=(double***)malloc3(nx,ny,nz);
    d_vzz=(double***)malloc3(nx,ny,nz);
    printf("偏导内存分配完成\n");

    pxx=(double***)malloc3(nx,ny,nz);
    pxy=(double***)malloc3(nx,ny,nz);
    pxz=(double***)malloc3(nx,ny,nz);
    pyx=(double***)malloc3(nx,ny,nz);
    pyy=(double***)malloc3(nx,ny,nz);
    pyz=(double***)malloc3(nx,ny,nz);
    pzx=(double***)malloc3(nx,ny,nz);
    pzy=(double***)malloc3(nx,ny,nz);
    pzz=(double***)malloc3(nx,ny,nz);
    qxx=(double***)malloc3(nx,ny,nz);
    qxy=(double***)malloc3(nx,ny,nz);
    qxz=(double***)malloc3(nx,ny,nz);
    qyx=(double***)malloc3(nx,ny,nz);
    qyy=(double***)malloc3(nx,ny,nz);
    qyz=(double***)malloc3(nx,ny,nz);
    qzx=(double***)malloc3(nx,ny,nz);
    qzy=(double***)malloc3(nx,ny,nz);
    qzz=(double***)malloc3(nx,ny,nz);
    printf("衰减量内存分配完成\n");

    rec_vx=(double**)malloc2(nx,nt);
    rec_vy=(double**)malloc2(nx,nt);
    rec_vz=(double**)malloc2(nx,nt);
    rec_wzx=(double**)malloc2(nx,nt);
    rec_wyx=(double**)malloc2(nx,nt);
    rec_wyz=(double**)malloc2(nx,nt);

    printf("内存分配完成");



    for(i=0; i<nx; i++)
    {
        for(y=0; y<ny; y++)
        {
            for(j=0; j<nz; j++)
            {
                pxx[i][y][j]=0;
                pxy[i][y][j]=0;
                pxz[i][y][j]=0;
                pyx[i][y][j]=0;
                pyy[i][y][j]=0;
                pyz[i][y][j]=0;
                pzx[i][y][j]=0;
                pzy[i][y][j]=0;
                pzz[i][y][j]=0;
                qxx[i][y][j]=0;
                qxy[i][y][j]=0;
                qxz[i][y][j]=0;
                qyx[i][y][j]=0;
                qyy[i][y][j]=0;
                qyz[i][y][j]=0;
                qzx[i][y][j]=0;
                qzy[i][y][j]=0;
                qzz[i][y][j]=0;


                vx[i][y][j]=0;
                vy[i][y][j]=0;
                vz[i][y][j]=0;
                ux[i][y][j]=0;
                uy[i][y][j]=0;
                uz[i][y][j]=0;
                taoxx[i][y][j]=0;
                taoyy[i][y][j]=0;
                taozz[i][y][j]=0;
                taozx[i][y][j]=0;
                taoyx[i][y][j]=0;
                taoyz[i][y][j]=0;
            }
        }

    }
    printf("初始值赋0完成");
    for(k=0; k<nt; k++)
    {

/*****************************************************************TAOXXZZ***********************************************************/
        for(i=0; i<nx; i++)
        {
            for(y=0; y<ny; y++)
            {
                for(j=0; j<nz; j++)
                {
                    if(i==sx&&y==sy&&j==sz)
                    {
                        taoxxzz[sx][sy][sz]=src[k];
                    }
                    else
                    taoxxzz[i][y][j]=0;
                }
            }

        }
        printf("taoxxzz %\n",k);
/***********************************************************************************VX*******************************************************************/
        for(i=0; i<nx; i++)
        {
            for(y=0; y<ny; y++)
            {
                for(j=0; j<nz; j++)
                {
                    d_taoxx[i][y][j]=0;
                    d_taoxy[i][y][j]=0;
                    d_taoxz[i][y][j]=0;

                    if(i<n)
                    {
                        for(m=1; m<i+1; m++)
                        {
                            d_taoxx[i][y][j]=d_taoxx[i][y][j]+(1.0/bx)*am[i][m]*(taoxx[i+m*bx-bx][y][j]-taoxx[i-m*bx][y][j]);

                        }
                    }
                    if(i>=n&&i<nx-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_taoxx[i][y][j]=d_taoxx[i][y][j]+(1.0/bx)*am[n][m]*(taoxx[i+m*bx-bx][y][j]-taoxx[i-m*bx][y][j]);
                        }
                    }
                    if(i>=nx-n&&i<nx)
                    {
                        for(m=1; m<nx-i; m++)
                        {
                            d_taoxx[i][y][j]=d_taoxx[i][y][j]+(1.0/bx)*am[nx-i-1][m]*(taoxx[i+m*bx-bx][y][j]-taoxx[i-m*bx][y][j]);
                        }
                    }
                    if(y<n)
                    {
                        for(m=1; m<y+1; m++)
                        {
                            d_taoxy[i][y][j]=d_taoxy[i][y][j]+(1.0/by)*am[y][m]*(taoyx[i][y+m*by-by][j]-taoyx[i][y-m*by][j]);
                        }
                    }
                    if(y>=n&&y<nx-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_taoxy[i][y][j]=d_taoxy[i][y][j]+(1.0/by)*am[n][m]*(taoyx[i][y+m*by-by][j]-taoyx[i][y-m*by][j]);
                        }
                    }
                    if(y>=nx-n&&y<nx)
                    {
                        for(m=1; m<ny-y; m++)
                        {
                            d_taoxy[i][y][j]=d_taoxy[i][y][j]+(1.0/by)*am[ny-y-1][m]*(taoyx[i][y+m*by-by][j]-taoyx[i][y-m*by][j]);
                        }
                    }
                    if(j<n)
                    {
                        for(m=1; m<j+1; m++)
                        {
                            d_taoxz[i][y][j]=d_taoxz[i][y][j]+(1.0/bz)*am[j][m]*(taozx[i][y][j+m*bz-bz]-taozx[i][y][j-m*bz]);
                        }
                    }
                    if(j>=n&&j<nz-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_taoxz[i][y][j]=d_taoxz[i][y][j]+(1.0/bz)*am[n][m]*(taozx[i][y][j+m*bz-bz]-taozx[i][y][j-m*bz]);
                        }
                    }
                    if(j>=nz-n&&j<nz)
                    {
                        for(m=1; m<nz-j; m++)
                        {
                            d_taoxz[i][y][j]=d_taoxz[i][y][j]+(1.0/bz)*am[nz-j-1][m]*(taozx[i][y][j+m*bz-bz]-taozx[i][y][j-m*bz]);
                        }
                    }
                    pxx[i][y][j]=((2.0*dx1[i]*d_taoxx[i][y][j]*dt*1.0)+((2.0-(dt*dx1[i]*1.0))*pxx[i][y][j]*1.0))/(2.0+(dt*dx1[i]*1.0));
                    pxy[i][y][j]=((2.0*dy1[y]*d_taoxy[i][y][j]*dt*1.0)+((2.0-(dt*dy1[y]*1.0))*pxy[i][y][j]*1.0))/(2.0+(dt*dy1[y]*1.0));
                    pxz[i][y][j]=((2.0*dz1[j]*d_taoxz[i][y][j]*dt*1.0)+((2.0-(dt*dz1[j]*1.0))*pxz[i][y][j]*1.0))/(2.0+(dt*dz1[j]*1.0));
                    vx[i][y][j]=vx[i][y][j]+dt/p[i][y][j]*(d_taoxx[i][y][j]+d_taoxy[i][y][j]+d_taoxz[i][y][j]-pxx[i][y][j]-pxy[i][y][j]-pxz[i][y][j]);
                    ux[i][y][j]=ux[i][y][j]+dt*vx[i][y][j];
                }

            }
        }
/***********************************************************************************VY*******************************************************************/
        for(i=0; i<nx; i++)
        {
            for(y=0; y<ny; y++)
            {
                for(j=0; j<nz; j++)
                {
                    d_taoyx[i][y][j]=0;
                    d_taoyy[i][y][j]=0;
                    d_taoyz[i][y][j]=0;

                    if(i<n)
                    {
                        for(m=1; m<i+1; m++)
                        {
                            d_taoyx[i][y][j]=d_taoyx[i][y][j]+(1.0/bx)*am[i][m]*(taoyx[i+m*bx-bx][y][j]-taoyx[i-m*bx][y][j]);
                        }
                    }
                    if(i>=n&&i<nx-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_taoyx[i][y][j]=d_taoyx[i][y][j]+(1.0/bx)*am[n][m]*(taoyx[i+m*bx-bx][y][j]-taoyx[i-m*bx][y][j]);
                        }
                    }
                    if(i>=nx-n&&i<nx)
                    {
                        for(m=1; m<nx-i; m++)
                        {
                            d_taoyx[i][y][j]=d_taoyx[i][y][j]+(1.0/bx)*am[nx-i-1][m]*(taoyx[i+m*bx-bx][y][j]-taoyx[i-m*bx][y][j]);
                        }
                    }
                    if(y<n)
                    {
                        for(m=1; m<y+1; m++)
                        {
                            d_taoyy[i][y][j]=d_taoyy[i][y][j]+(1.0/by)*am[y][m]*(taoyy[i][y+m*by-by][j]-taoyy[i][y-m*by][j]);
                        }
                    }
                    if(y>=n&&y<nx-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_taoyy[i][y][j]=d_taoyy[i][y][j]+(1.0/by)*am[n][m]*(taoyy[i][y+m*by-by][j]-taoyy[i][y-m*by][j]);
                        }
                    }
                    if(y>=nx-n&&y<nx)
                    {
                        for(m=1; m<ny-y; m++)
                        {
                            d_taoyy[i][y][j]=d_taoyy[i][y][j]+(1.0/by)*am[ny-y-1][m]*(taoyy[i][y+m*by-by][j]-taoyy[i][y-m*by][j]);
                        }
                    }
                    if(j<n)
                    {
                        for(m=1; m<j+1; m++)
                        {
                            d_taoyz[i][y][j]=d_taoyz[i][y][j]+(1.0/bz)*am[j][m]*(taoyz[i][y][j+m*bz-bz]-taoyz[i][y][j-m*bz]);
                        }
                    }
                    if(j>=n&&j<nz-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_taoyz[i][y][j]=d_taoyz[i][y][j]+(1.0/bz)*am[n][m]*(taoyz[i][y][j+m*bz-bz]-taoyz[i][y][j-m*bz]);
                        }
                    }
                    if(j>=nz-n&&j<nz)
                    {
                        for(m=1; m<nz-j; m++)
                        {
                            d_taoyz[i][y][j]=d_taoyz[i][y][j]+(1.0/bz)*am[nz-j-1][m]*(taoyz[i][y][j+m*bz-bz]-taoyz[i][y][j-m*bz]);
                        }
                    }
                    pyx[i][y][j]=((2.0*dx1[i]*d_taoyx[i][y][j]*dt*1.0)+((2.0-(dt*dx1[i]*1.0))*pyx[i][y][j]*1.0))/(2.0+(dt*dx1[i]*1.0));
                    pyy[i][y][j]=((2.0*dy1[y]*d_taoyy[i][y][j]*dt*1.0)+((2.0-(dt*dy1[y]*1.0))*pyy[i][y][j]*1.0))/(2.0+(dt*dy1[y]*1.0));
                    pyz[i][y][j]=((2.0*dz1[j]*d_taoyz[i][y][j]*dt*1.0)+((2.0-(dt*dz1[j]*1.0))*pyz[i][y][j]*1.0))/(2.0+(dt*dz1[j]*1.0));
                    vy[i][y][j]=vy[i][y][j]+dt/p[i][y][j]*(d_taoyx[i][y][j]+d_taoyy[i][y][j]+d_taoyz[i][y][j]-pyx[i][y][j]-pyy[i][y][j]-pyz[i][y][j]);
                    uy[i][y][j]=uy[i][y][j]+dt*vy[i][y][j];
                }

            }
        }

/******************************************************************************VZ*******************************************************************************************************/
        for(i=0; i<nx; i++)
        {
            for(y=0; y<ny; y++)
            {
                for(j=0; j<nz; j++)
                {
                    d_taozx[i][y][j]=0;
                    d_taozy[i][y][j]=0;
                    d_taozz[i][y][j]=0;

                    if(j<n)
                    {
                        for(m=1; m<j+1; m++)
                        {
                            d_taozz[i][y][j]=d_taozz[i][y][j]+(1.0/bz)*am[j][m]*(taozz[i][y][j+m*bz]-taozz[i][y][j-m*bz+bz]);
                        }
                    }
                    if(j>=n&&j<nz-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_taozz[i][y][j]=d_taozz[i][y][j]+(1.0/bz)*am[n][m]*(taozz[i][y][j+m*bz]-taozz[i][y][j-m*bz+bz]);
                        }
                    }
                    if(j>=nz-n&&j<nz)
                    {
                        for(m=1; m<nz-j; m++)
                        {
                            d_taozz[i][y][j]=d_taozz[i][y][j]+(1.0/bz)*am[nz-j-1][m]*(taozz[i][y][j+m*bz]-taozz[i][y][j-m*bz+bz]);
                        }
                    }
                    if(y<n)
                    {
                        for(m=1; m<y+1; m++)
                        {
                            d_taozy[i][y][j]=d_taozy[i][y][j]+(1.0/by)*am[y][m]*(taoyz[i][y+m*by][j]-taoyz[i][y-m*by+by][j]);

                        }
                    }
                    if(y>=n&&y<ny-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_taozy[i][y][j]=d_taozy[i][y][j]+(1.0/by)*am[n][m]*(taoyz[i][y+m*by][j]-taoyz[i][y-m*by+by][j]);

                        }
                    }
                    if(y>=ny-n&&y<ny)
                    {
                        for(m=1; m<ny-y; m++)
                        {
                            d_taozy[i][y][j]=d_taozy[i][y][j]+(1.0/by)*am[ny-y-1][m]*(taoyz[i][y+m*by][j]-taoyz[i][y-m*by+by][j]);
                        }
                    }
                    if(i<n)
                    {
                        for(m=1; m<i+1; m++)
                        {
                            d_taozx[i][y][j]=d_taozx[i][y][j]+(1.0/bx)*am[i][m]*(taozx[i+m*bx][y][j]-taozx[i-m*bx+bx][y][j]);

                        }
                    }
                    if(i>=n&&i<nx-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_taozx[i][y][j]=d_taozx[i][y][j]+(1.0/bx)*am[n][m]*(taozx[i+m*bx][y][j]-taozx[i-m*bx+bx][y][j]);

                        }
                    }
                    if(i>=nx-n&&i<nx)
                    {
                        for(m=1; m<nx-i; m++)
                        {
                            d_taozx[i][y][j]=d_taozx[i][y][j]+(1.0/bx)*am[nx-i-1][m]*(taozx[i+m*bx][y][j]-taozx[i-m*bx+bx][y][j]);
                        }
                    }

                    pzz[i][y][j]=((2.0*dz2[j]*d_taozz[i][y][j]*dt*1.0)+((2.0-(dt*dz2[j]*1.0))*pzz[i][y][j]*1.0))/(2.0+(dt*dz2[j]*1.0));
                    pzy[i][y][j]=((2.0*dy2[y]*d_taozy[i][y][j]*dt*1.0)+((2.0-(dt*dy2[y]*1.0))*pzy[i][y][j]*1.0))/(2.0+(dt*dy2[y]*1.0));
                    pzx[i][y][j]=((2.0*dx2[i]*d_taozx[i][y][j]*dt*1.0)+((2.0-(dt*dx2[i]*1.0))*pzx[i][y][j]*1.0))/(2.0+(dt*dx2[i]*1.0));
                    vz[i][y][j]=vz[i][y][j]+dt/p[i][y][j]*(d_taozz[i][y][j]+d_taozy[i][y][j]+d_taozx[i][y][j]-pzz[i][y][j]-pzy[i][y][j]-pzx[i][y][j]);
                    uz[i][y][j]=uz[i][y][j]+dt*vz[i][y][j];
                }

            }
        }
/***************************************************w*************************************************************/
        for(i=0; i<nx-1; i++)
        {
            for(y=0; y<ny-1; y++)
            {
                for(j=0; j<nz-1; j++)
                {
                    wzx[i][y][j]=0.5*(((vx[i][y][j+1]-vx[i][y][j])/bz)-((vz[i+1][y][j]-vz[i][y][j])/bx));
                    wyx[i][y][j]=0.5*(((vx[i][y+1][j]-vx[i][y][j])/by)-((vy[i+1][y][j]-vy[i][y][j])/bx));
                    wyz[i][y][j]=0.5*(((vz[i][y+1][j]-vz[i][y][j])/by)-((vy[i][y][j+1]-vy[i][y][j])/bz));
                }
            }
        }
/********************************************************************TAOXX TAOYY TAOZZ***************************************************************************************/
        for(i=0; i<nx; i++)
        {
            for(y=0; y<ny; y++)
            {
                for(j=0; j<nz; j++)
                {
                    d_vxx[i][y][j]=0;
                    d_vyy[i][y][j]=0;
                    d_vzz[i][y][j]=0;

                    if(i<n)
                    {
                        for(m=1; m<i+1; m++)
                        {
                            d_vxx[i][y][j]=d_vxx[i][y][j]+(1.0/bx)*am[i][m]*(vx[i+m*bx][y][j]-vx[i-m*bx+bx][y][j]);
                        }
                    }
                    if(i>=n&&i<nx-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_vxx[i][y][j]=d_vxx[i][y][j]+(1.0/bx)*am[n][m]*(vx[i+m*bx][y][j]-vx[i-m*bx+bx][y][j]);
                        }
                    }
                    if(i>=nx-n&&i<nx)
                    {
                        for(m=1; m<nx-i; m++)
                        {
                            d_vxx[i][y][j]=d_vxx[i][y][j]+(1.0/bx)*am[nx-i-1][m]*(vx[i+m*bx][y][j]-vx[i-m*bx+bx][y][j]);
                        }
                    }
                    if(y<n)
                    {
                        for(m=1; m<y+1; m++)
                        {
                            d_vyy[i][y][j]=d_vyy[i][y][j]+(1.0/by)*am[y][m]*(vy[i][y+m*by][j]-vy[i][y-m*by+by][j]);
                        }
                    }
                    if(y>=n&&y<ny-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_vyy[i][y][j]=d_vyy[i][y][j]+(1.0/by)*am[n][m]*(vy[i][y+m*by][j]-vy[i][y-m*by+by][j]);
                        }
                    }
                    if(y>=ny-n&&y<ny)
                    {
                        for(m=1; m<ny-y; m++)
                        {
                            d_vyy[i][y][j]=d_vyy[i][y][j]+(1.0/by)*am[ny-y-1][m]*(vy[i][y+m*by][j]-vy[i][y-m*by+by][j]);
                        }
                    }
                    if(j<n)
                    {
                        for(m=1; m<j+1; m++)
                        {
                            d_vzz[i][y][j]=d_vzz[i][y][j]+(1.0/bz)*am[j][m]*(vz[i][y][j+m*bz-bz]-vz[i][y][j-m*bz]);
                        }
                    }

                    if(j>=n&&j<nz-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_vzz[i][y][j]=d_vzz[i][y][j]+(1.0/bz)*am[n][m]*(vz[i][y][j+m*bz-bz]-vz[i][y][j-m*bz]);
                        }
                    }
                    if(j>=nz-n&&j<nz)
                    {
                        for(m=1; m<nz-j; m++)
                        {
                            d_vzz[i][y][j]=d_vzz[i][y][j]+(1.0/bz)*am[nz-j-1][m]*(vz[i][y][j+m*bz-bz]-vz[i][y][j-m*bz]);
                        }
                    }

                    qxx[i][y][j]=((2.0*dx1[i]*d_vxx[i][y][j]*dt*1.0)+((2.0-(dt*dx1[i]*1.0))*qxx[i][y][j]*1.0))/(2.0+(dt*dx1[i]*1.0));
                    qyy[i][y][j]=((2.0*dy1[y]*d_vyy[i][y][j]*dt*1.0)+((2.0-(dt*dy1[y]*1.0))*qyy[i][y][j]*1.0))/(2.0+(dt*dy1[y]*1.0));
                    qzz[i][y][j]=((2.0*dz1[j]*d_vzz[i][y][j]*dt*1.0)+((2.0-(dt*dz1[j]*1.0))*qzz[i][y][j]*1.0))/(2.0+(dt*dz1[j]*1.0));

                    taoxx[i][y][j]=taoxx[i][y][j]+dt*(lame2[i][y][j]+2*lame1[i][y][j])*(d_vxx[i][y][j]-qxx[i][y][j])+dt*lame2[i][y][j]*(d_vzz[i][y][j]-qzz[i][y][j]+d_vyy[i][y][j]-qyy[i][y][j])+taoxxzz[i][y][j];/*必须有-qxx[k][i][j]-qzz[k][i][j]*/
                    taoyy[i][y][j]=taoyy[i][y][j]+dt*(lame2[i][y][j]+2*lame1[i][y][j])*(d_vyy[i][y][j]-qyy[i][y][j])+dt*lame2[i][y][j]*(d_vzz[i][y][j]-qzz[i][y][j]+d_vxx[i][y][j]-qxx[i][y][j])+taoxxzz[i][y][j];/*必须有-qxx[k][i][j]-qzz[k][i][j]*/
                    taozz[i][y][j]=taozz[i][y][j]+dt*(lame2[i][y][j]+2*lame1[i][y][j])*(d_vzz[i][y][j]-qzz[i][y][j])+dt*lame2[i][y][j]*(d_vxx[i][y][j]-qxx[i][y][j]+d_vyy[i][y][j]-qyy[i][y][j])+taoxxzz[i][y][j];/*必须有-qxx[k][i][j]-qzz[k][i][j]*/
                }

            }
        }

/*******************************************************************TAOZX*********************************************************************************************/
        for(i=0; i<nx; i++)
        {
            for(y=0; y<ny; y++)
            {
                for(j=0; j<nz; j++)
                {
                    d_vxz[i][y][j]=0;
                    d_vzx[i][y][j]=0;

                    if(j<n)
                    {
                        for(m=1; m<j+1; m++)
                        {
                            d_vxz[i][y][j]=d_vxz[i][y][j]+(1.0/bz)*am[j][m]*(vx[i][y][j+m*bz]-vx[i][y][j-m*bz+bz]);
                        }
                    }
                    if(j>=n&&j<nz-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_vxz[i][y][j]=d_vxz[i][y][j]+(1.0/bz)*am[n][m]*(vx[i][y][j+m*bz]-vx[i][y][j-m*bz+bz]);
                        }
                    }
                    if(j>=nz-n&&j<nz)
                    {
                        for(m=1; m<nz-j; m++)
                        {
                            d_vxz[i][y][j]=d_vxz[i][y][j]+(1.0/bz)*am[nz-j-1][m]*(vx[i][y][j+m*bz]-vx[i][y][j-m*bz+bz]);
                        }
                    }
                    if(i<n)
                    {
                        for(m=1; m<i+1; m++)
                        {
                            d_vzx[i][y][j]=d_vzx[i][y][j]+(1.0/bx)*am[i][m]*(vz[i+m*bx-bx][y][j]-vz[i-m*bx][y][j]);
                        }
                    }
                    if(i>=n&&i<nx-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_vzx[i][y][j]=d_vzx[i][y][j]+(1.0/bx)*am[n][m]*(vz[i+m*bx-bx][y][j]-vz[i-m*bx][y][j]);
                        }
                    }
                    if(i>=nx-n&&i<nx)
                    {
                        for(m=1; m<nx-i; m++)
                        {
                            d_vzx[i][y][j]=d_vzx[i][y][j]+(1.0/bx)*am[nx-i-1][m]*(vz[i+m*bx-bx][y][j]-vz[i-m*bx][y][j]);
                        }
                    }

                    qzx[i][y][j]=((2.0*dx2[i]*d_vzx[i][y][j]*dt*1.0)+((2.0-(dt*dx2[i]*1.0))*qzx[i][y][j]*1.0))/(2.0+(dt*dx2[i]*1.0));
                    qxz[i][y][j]=((2.0*dz2[j]*d_vxz[i][y][j]*dt*1.0)+((2.0-(dt*dz2[j]*1.0))*qxz[i][y][j]*1.0))/(2.0+(dt*dz2[j]*1.0));
                    taozx[i][y][j]=taozx[i][y][j]+dt*lame1[i][y][j]*(d_vxz[i][y][j]+d_vzx[i][y][j]-qzx[i][y][j]-qxz[i][y][j]);
                }

            }
        }
//        printf("taozx %d\n",k);

/*******************************************************************TAOYX*********************************************************************************************/
        for(i=0; i<nx; i++)
        {
            for(y=0; y<ny; y++)
            {
                for(y=0; y<ny; y++)
                {
                    d_vxy[i][y][j]=0;
                    d_vyx[i][y][j]=0;

                    if(y<n)
                    {
                        for(m=1; m<y+1; m++)
                        {
                            d_vxy[i][y][j]=d_vxy[i][y][j]+(1.0/by)*am[y][m]*(vx[i][y+m*by][j]-vx[i][y-m*by+by][j]);
                        }
                    }
                    if(y>=n&&y<ny-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_vxy[i][y][j]=d_vxy[i][y][j]+(1.0/by)*am[n][m]*(vx[i][y+m*by][j]-vx[i][y-m*by+by][j]);
                        }
                    }
                    if(y>=ny-n&&y<ny)
                    {
                        for(m=1; m<ny-y; m++)
                        {
                            d_vxy[i][y][j]=d_vxy[i][y][j]+(1.0/by)*am[ny-y-1][m]*(vx[i][y+m*by][j]-vx[i][y-m*by+by][j]);
                        }
                    }
                    if(i<n)
                    {
                        for(m=1; m<i+1; m++)
                        {
                            d_vyx[i][y][j]=d_vyx[i][y][j]+(1.0/bx)*am[i][m]*(vy[i+m*bx-bx][y][j]-vy[i-m*bx][y][j]);
                        }
                    }
                    if(i>=n&&i<nx-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_vyx[i][y][j]=d_vyx[i][y][j]+(1.0/bx)*am[n][m]*(vy[i+m*bx-bx][y][j]-vy[i-m*bx][y][j]);
                        }
                    }
                    if(i>=nx-n&&i<nx)
                    {
                        for(m=1; m<nx-i; m++)
                        {
                            d_vyx[i][y][j]=d_vyx[i][y][j]+(1.0/bx)*am[nx-i-1][m]*(vy[i+m*bx-bx][y][j]-vy[i-m*bx][y][j]);
                        }
                    }

                    qyx[i][y][j]=((2.0*dx2[i]*d_vyx[i][y][j]*dt*1.0)+((2.0-(dt*dx2[i]*1.0))*qyx[i][y][j]*1.0))/(2.0+(dt*dx2[i]*1.0));
                    qxy[i][y][j]=((2.0*dy2[y]*d_vxy[i][y][j]*dt*1.0)+((2.0-(dt*dy2[y]*1.0))*qxy[i][y][j]*1.0))/(2.0+(dt*dy2[y]*1.0));
                    taoyx[i][y][j]=taoyx[i][y][j]+dt*lame1[i][y][j]*(d_vxy[i][y][j]+d_vyx[i][y][j]-qyx[i][y][j]-qxy[i][y][j]);
                }

            }
        }
/*******************************************************************TAOYZ*********************************************************************************************/
        for(i=0; i<nx; i++)
        {
            for(y=0; y<ny; y++)
            {
                for(j=0; j<nz; j++)
                {
                    d_vyz[i][y][j]=0;
                    d_vzy[i][y][j]=0;

                    if(j<n)
                    {
                        for(m=1; m<j+1; m++)
                        {
                            d_vyz[i][y][j]=d_vyz[i][y][j]+(1.0/bz)*am[j][m]*(vy[i][y][j+m*bz]-vy[i][y][j-m*bz+bz]);
                        }
                    }
                    if(j>=n&&j<nz-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_vyz[i][y][j]=d_vyz[i][y][j]+(1.0/bz)*am[n][m]*(vy[i][y][j+m*bz]-vy[i][y][j-m*bz+bz]);
                        }
                    }
                    if(j>=nz-n&&j<nz)
                    {
                        for(m=1; m<nz-j; m++)
                        {
                            d_vyz[i][y][j]=d_vyz[i][y][j]+(1.0/bz)*am[nz-j-1][m]*(vy[i][y][j+m*bz]-vy[i][y][j-m*bz+bz]);
                        }
                    }
                    if(y<n)
                    {
                        for(m=1; m<y+1; m++)
                        {
                            d_vzy[i][y][j]=d_vzy[i][y][j]+(1.0/by)*am[y][m]*(vz[i][y+m*by-by][j]-vz[i][y-m*by][j]);
                        }
                    }
                    if(y>=n&&y<ny-n)
                    {
                        for(m=1; m<n+1; m++)
                        {
                            d_vzy[i][y][j]=d_vzy[i][y][j]+(1.0/by)*am[n][m]*(vz[i][y+m*by-by][j]-vz[i][y-m*by][j]);
                        }
                    }
                    if(y>=ny-n&&y<ny)
                    {
                        for(m=1; m<ny-y; m++)
                        {
                            d_vzy[i][y][j]=d_vzy[i][y][j]+(1.0/by)*am[ny-y-1][m]*(vz[i][y+m*by-by][j]-vz[i][y-m*by][j]);
                        }
                    }

                    qzy[i][y][j]=((2.0*dy2[y]*d_vzy[i][y][j]*dt*1.0)+((2.0-(dt*dy2[y]*1.0))*qzy[i][y][j]*1.0))/(2.0+(dt*dy2[y]*1.0));
                    qyz[i][y][j]=((2.0*dz2[j]*d_vyz[i][y][j]*dt*1.0)+((2.0-(dt*dz2[j]*1.0))*qyz[i][y][j]*1.0))/(2.0+(dt*dz2[j]*1.0));
                    taoyz[i][y][j]=taoyz[i][y][j]+dt*lame1[i][y][j]*(d_vyz[i][y][j]+d_vzy[i][y][j]-qzy[i][y][j]-qyz[i][y][j]);
                }
            }
        }
        for(i=0; i<nx; i++)
        {
            rec_vx[i][k]=vx[i][ry][rz];
            rec_vy[i][k]=vy[i][ry][rz];
            rec_vz[i][k]=vz[i][ry][rz];
        }
        for(i=0; i<nx-1; i++)
        {
            rec_wzx[i][k]=wzx[i][ry][rz];
            rec_wyx[i][k]=wyx[i][ry][rz];
            rec_wyz[i][k]=wyz[i][ry][rz];
        }

        if((k%dt_snap)==0)
        {
            for (i=0; i<nx-1; i++)
            {
                for(y=0; y<ny-1; y++)
                {
                    for (j=0; j<nz-1; j++)
                    {
                        fprintf(fp_vx_snap,"%e    ",vx[i][y][j]);
                        fprintf(fp_vy_snap,"%e    ",vy[i][y][j]);
                        fprintf(fp_vz_snap,"%e    ",vz[i][y][j]);
                        fprintf(fp_wzx_snap,"%e    ",wzx[i][y][j]);
                        fprintf(fp_wyx_snap,"%e    ",wyx[i][y][j]);
                        fprintf(fp_wyz_snap,"%e    ",wyz[i][y][j]);
                    }

                    fprintf(fp_vx_snap,"\n");
                    fprintf(fp_vy_snap,"\n");
                    fprintf(fp_vz_snap,"\n");
                    fprintf(fp_wzx_snap,"\n");
                    fprintf(fp_wyx_snap,"\n");
                    fprintf(fp_wyz_snap,"\n");
                }

            }
        }
        printf("end %d\n",k);

    }


    printf("波动方程完成\n");

/******************************************地震记录************************************************/
    for(i=0; i<nx; i++)
    {
        for(k=0; k<nt; k++)
        {

            fprintf(fp_rec_vx,"%e    ",rec_vx[i][k]);
            fprintf(fp_rec_vy,"%e    ",rec_vy[i][k]);
            fprintf(fp_rec_vz,"%e    ",rec_vz[i][k]);
            fprintf(fp_rec_wzx,"%e    ",rec_wzx[i][k]);
            fprintf(fp_rec_wyx,"%e    ",rec_wyx[i][k]);
            fprintf(fp_rec_wyz,"%e    ",rec_wyz[i][k]);

        }
        fprintf(fp_rec_vx,"\n");
        fprintf(fp_rec_vy,"\n");
        fprintf(fp_rec_vz,"\n");
        fprintf(fp_rec_wzx,"\n");
        fprintf(fp_rec_wyx,"\n");
        fprintf(fp_rec_wyz,"\n");
    }
    printf("地震记录完成");


/******************************************关闭文件释放指针************************************************/
//    free(src);
//    free3(taoxxzz,nx,ny,nz);
//    fclose(fp_p);
//    fclose(fp_src);
//    fclose(fp_t);
//    free(dx1);
//    free(dy1);
//    free(dz1);
//    free(dx2);
//    free(dy2);
//    free(dz2);
//    free3(taoxx,nx,ny,nz);
//    free3(taoyy,nx,ny,nz);
//    free3(taozz,nx,ny,nz);
//    free3(taozx,nx,ny,nz);
//    free3(taoyx,nx,ny,nz);
//    free3(taoyz,nx,ny,nz);
//    free3(vx,nx,ny,nz);
//    free3(vy,nx,ny,nz);
//    free3(vz,nx,ny,nz);
//    free3(ux,nx,ny,nz);
//    free3(uy,nx,ny,nz);
//    free3(uz,nx,ny,nz);
//    free3(wyx,nx,ny,nz);
//    free3(wyz,nx,ny,nz);
//    free3(wzx,nx,ny,nz);
//    free3(d_taoxx,nx,ny,nz);
//    free3(d_taoxy,nx,ny,nz);
//    free3(d_taoxz,nx,ny,nz);
//    free3(d_taoyx,nx,ny,nz);
//    free3(d_taoyy,nx,ny,nz);
//    free3(d_taoyz,nx,ny,nz);
//    free3(d_taozx,nx,ny,nz);
//    free3(d_taozy,nx,ny,nz);
//    free3(d_taozz,nx,ny,nz);
//    free3(d_vxx,nx,ny,nz);
//    free3(d_vxy,nx,ny,nz);
//    free3(d_vxz,nx,ny,nz);
//    free3(d_vyx,nx,ny,nz);
//    free3(d_vyy,nx,ny,nz);
//    free3(d_vyz,nx,ny,nz);
//    free3(d_vzx,nx,ny,nz);
//    free3(d_vzy,nx,ny,nz);
//    free3(d_vzz,nx,ny,nz);
//    free3(pxx,nx,ny,nz);
//    free3(pxy,nx,ny,nz);
//    free3(pxz,nx,ny,nz);
//    free3(pyx,nx,ny,nz);
//    free3(pyy,nx,ny,nz);
//    free3(pyz,nx,ny,nz);
//    free3(pzx,nx,ny,nz);
//    free3(pzy,nx,ny,nz);
//    free3(pzz,nx,ny,nz);
//    free3(qxx,nx,ny,nz);
//    free3(qxy,nx,ny,nz);
//    free3(qxz,nx,ny,nz);
//    free3(qyx,nx,ny,nz);
//    free3(qyy,nx,ny,nz);
//    free3(qyz,nx,ny,nz);
//    free3(qzx,nx,ny,nz);
//    free3(qzy,nx,ny,nz);
//    free3(qzz,nx,ny,nz);
//    free2(rec_vx);
//    free2(rec_vy);
//    free2(rec_vz);
//    fclose(fp_rec_vz);
//    fclose(fp_rec_vz);
//    fclose(rec_vx_bin);
//    fclose(rec_vz_bin);
    return 0;
    printf("1");
}


/*******************************************分配二维动态内存*************************************************/
double **malloc2(int n1,int n2)
{
    double **memory2;
    int i;
    memory2=(double**)malloc(n1*sizeof(double*));
    for(i=0;i<n1;i++)
    memory2[i]=(double*)malloc(n2*sizeof(double));
    return memory2;
}
/*******************************************释放二维动态内存*************************************************/
void free2(double **memory2)
{
    free(memory2[0]);
    free(memory2);
}
/*******************************************分配三维动态内存*************************************************/
double ***malloc3(int n1,int n2,int n3)
{
    double ***memory3;
    int i,j;
    memory3=(double***)malloc(n1*sizeof(double**));
    for(i=0;i<n1;i++)
        memory3[i]=(double**)malloc(n2*sizeof(double*));
    for(i=0;i<n1;i++)
    {
        for(j=0;j<n2;j++)
        {
            memory3[i][j]=(double*)malloc(n3*sizeof(double));
        }
    }
    return memory3;
}
/*******************************************释放三维动态内存*************************************************/
void free3(double ***memory3,int n1,int n2,int n3)
{
    int i,j;
    if(memory3!=NULL)
    {
        for(i=0;i<n1;i++)
           for(j=0;j<n2;j++)
           free(memory3[i][j]);
        for(i=0;i<n1;i++)
           free(memory3[i]);
        free(memory3);
    }
}
