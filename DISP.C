#include "znc.h"
#include <mem.h>
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <graphics.h>
#define LNYL 20
#define LNCO0 (long)0x45    /*LN00前景色*/
#define LNCO1 (long)0x45    /*LN1前景色,提示*/
#define LNCO3 (long)0x27    /*选择框内前景色*/
#define LNCO4 (long)0x0     /*选择框内前景色 按键*/
#define LNCO5 (long)0x26    /*选择框内前景色 选中*/
#define LNCO6 (long)(0x6A)  /*选择框内前景色 无效*/
#define LNBC0N (long)0x37   /*     空白色*/
#define LNBC0S (long)0x28   /*LN00 弹出色*/
#define LNBC0P (long)0x2E   /*LN00 按下色*/
#define LNBC3 (long)0x60    /*选择框内背景色*/
#define LNBC4 (long)0x1c    /*选择框内背景色 按键*/
#define LNBC5 (long)0xc1    /*选择框内背景色 选中*/
long position[10];
void ShowWin();
void CheckTab();
void movmem1();
void ShowTB();
void Dispebar();
void Dispcbar();
void Dispbar3d();
void ShowDP1(char flag);
void getpix(int hb,int lb,char *buf);
void ShowFace1(int i,int color);
void Disp(void)
{
    int i,j,k;
    unsigned char far *pi;
    long    ll;
    CloseScr(0);
    OpenFileM();
    ShowWin();
    ShowFlag(0);
    ShowXYZ(0);
    ShowTime(0);
    ShowTimeD(0);
    ShowFL(0);
    ShowTable(0);
    ShowKey(0,0);
    ShowSWF();
    ShowDP(0);
    ShowDP1(0);
    ShowHT();
    ShowPZ(0);
    ShowVer(VER);
    ShowMess(0);
    OpenScr();
}
void ShowFace1(int moves,int color)
{
    unsigned char a[]=
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x07\xff\xc0"
        "\x00\x00\x00\x0f\xff\x80\x00\x00\x00\x0f\xff\x80"
        "\x00\x00\x00\x1f\xff\x00\x00\x00\x00\x1f\xff\x00"
        "\x00\x00\x00\x3f\x80\x00\x00\x00\x00\x3f\x80\x00"
        "\x00\x00\x00\x7f\x00\x00\x00\x3f\x80\x7f\x00\xfe"
        "\x00\x7f\x00\xfe\x01\xfc\x00\x7f\x00\xfe\x01\xfc"
        "\x00\xfe\x01\xfc\x03\xf8\x00\xfe\x01\xfc\x03\xf8"
        "\x01\xfc\x03\xf8\x07\xf0\x01\xff\xff\xff\xff\xf0"
        "\x03\xff\xff\xff\xff\xe0\x03\xff\xff\xff\xff\xe0"
        "\x07\xff\xff\xff\xff\xc0\x07\xff\xff\xff\xff\xc0"
        "\x0f\xff\xff\xff\xff\x80\x0f\xe0\x1f\xc0\x3f\x80"
        "\x1f\xc0\x3f\x80\x7f\x00\x1f\xc0\x3f\x80\x7f\x00"
        "\x3f\x80\x7f\x00\xfe\x00\x3f\x80\x7f\x00\xfe\x00"
        "\x7f\x00\xfe\x01\xfc\x00\x00\x00\xfe\x00\x00\x00"
        "\x00\x01\xfc\x00\x00\x00\x00\x01\xfc\x00\x00\x00"
        "\x00\x03\xf8\x00\x00\x00\x00\x03\xf8\x00\x00\x00"
        "\x00\x07\xf0\x00\x00\x00\x00\x07\xf0\x00\x00\x00"
        "\x00\x0f\xe0\x00\x00\x00\x00\x00\x00\x00\x00\x00";
    int ix,iy,ix1,ix2;
    register unsigned j,i;
    int IX0,IY0;
    IX0=GX0+577+moves;
    IY0=GY0+17+moves;
    for(iy=0; iy<36; iy++)
        for(ix=0,ix1=IX0; ix<6; ix++,ix1+=7)
        {
            j=a[iy*6+ix];
            for(i=0x80,ix2=0; ix2<8; i>>=1,ix2++)
            {
                if((i&j))
                {
                    putpixel(ix+ix1+ix2,iy+IY0,color);
                }
            }
        }
}
void ShowWin()
{
    setcolor(0xb);
    rectangle(X0,Y0,X1,Y1);
    setfillstyle(1,BKCOLORA);
    bar(AX0+1,AY0+1,AX1,AY1);
    rectangle(AX0,AY0,AX1+1,AY1+1);
    setfillstyle(1,BKCOLORB);
    bar(BX0+1,BY0+1,BX1,BY1);
    rectangle(BX0,BY0,BX1+1,BY1+1);
    setfillstyle(1,BKCOLORB);
    bar(BX0+1,BY0+1,BX1,BY0+20);
    rectangle(BX0,BY0,BX1+1,BY0+21);
    setfillstyle(1,BKCOLORC);
    bar(CX0+1,CY0+1,CX1,CY1);
    rectangle(EX0,EY0,EX1+1,EY1+1);
    setfillstyle(1,BKCOLORE);
    bar(EX0+1,EY0+1,EX1,EY1);
    setfillstyle(1,BKCOLORG);
    bar(GX0+1,GY0+1,GX1,GY1);
    rectangle(GX0,GY0,GX1+1,GY1+1);
}
void ShowHT()
{
    char str[5]="\0\0\0\0";
    int color,bkcolor,i;
    Dispbar3d(EX0+4,EY0+4+47*5,EX1-4,EY0+4+47*5+40,BKCOLORE1|COLORE1<<4,0,0x119,1,"跳   高");
    Dispebar(EX0+4+5,EY0+4+47*5+21,EX0+4+5+8,EY0+4+47*5+21+16,COLORE1,BKCOLORE1,1,"H");
    Dispebar((EX0+EX1)/2+1+5,EY0+4+47*5+21,(EX0+EX1)/2+1+5+8,EY0+4+47*5+21+16,COLORE1,BKCOLORE1,1,"T");
    if(F[7]==1)
    {
        color=15-COLORE2;
        bkcolor=15-BKCOLORE2;
    }
    else
    {
        color=COLORE2;
        bkcolor=BKCOLORE2;
    }
    str[0]=F8H/10+'0';
    str[1]=F8H%10+'0';
    i=0;
    if(str[0]=='0')
    {
        i++;
    }
    Dispebar(EX0+4+5+8,EY0+4+47*5+21,(EX0+EX1)/2-1-9,EY0+4+47*5+21+16,color,bkcolor,1,str+i);
    if(F[7]==2)
    {
        color=15-COLORE2;
        bkcolor=15-BKCOLORE2;
    }
    else
    {
        color=COLORE2;
        bkcolor=BKCOLORE2;
    }
    str[0]=F8T/10+'0';
    str[1]=F8T%10+'0';
    i=0;
    if(str[0]=='0')
    {
        i++;
    }
    Dispebar((EX0+EX1)/2+1+5+8,EY0+4+47*5+21,EX1-4-9,EY0+4+47*5+21+16,color,bkcolor,1,str+i);
}
void ShowPZ(int flag)
{
    char str[12];
    long Velocity_x,Velocity_y,Velocity_z;
    if(!flag||flag==1)
    {
        Velocity_x=abs(Read_Velocity_x(0));
        ltoa(Velocity_x,str,10);
        Dispebar(AX0+10+38,5+36,AX0+10+62,5+36+8+1,COLORA,BKCOLORA,0,str);
    }
    if(!flag||flag==2)
    {
        Velocity_y=abs(Read_Velocity_y(0));
        ltoa(Velocity_y,str,10);
        Dispebar(AX0+10+38,5+36+60,AX0+10+62,5+36+8+60+1,COLORA,BKCOLORA,0,str);
    }
    if(!flag||flag==3)
    {
        Velocity_z=abs(Read_Velocity_z(0));
        ltoa(Velocity_z,str,10);
        Dispebar(AX0+10+38,5+36+120,AX0+10+62,5+36+8+120+1,COLORA,BKCOLORA,0,str);
    }
}
void ShowDP1(char flag)
{
    char xyzs[12];
    char xyzs1[5]= {'.',48,48,48,0};
    long l;
    register i;
    if(!flag)
    {
        Dispcbar(DX0+1,DY0+23*4-1,DX1-1,EY0+23*4+20,COLORD1,BKCOLORD,12,"剩余坐标");
        Dispcbar(DX0+1,DY0+23*5-1,DX0+10,EY0+23*5+20,COLORD1,BKCOLORD,3,"X");
        Dispcbar(DX0+1,DY0+23*6-1,DX0+10,EY0+23*6+20,COLORD1,BKCOLORD,3,"Y");
        Dispcbar(DX0+1,DY0+23*7-1,DX0+10,EY0+23*7+20,COLORD1,BKCOLORD,3,"Z");
    }
    if(!flag||flag==1)
    {
        l=position_x-position_px;
        l*=DZC;
        if(l>=0)
        {
            xyzs[0]=' ';
        }
        else
        {
            xyzs[0]='-';
        }
        i=(long)l/1000;
        i=abs(i);
        itoa(i,xyzs+1,10);
        if(strlen(xyzs)>5)
        {
            strcpy(xyzs+1,xyzs+2);
        }
        i=l%1000;
        i=abs(i);
        xyzs1[1]=i/100+'0';
        i%=100;
        xyzs1[2]=i/10+'0';
        xyzs1[3]=i%10+'0';
        strcat(xyzs,xyzs1);
        Dispebar(DX0+13,DY0+23*5-1,DX1-1,EY0+23*5+20,COLORD,BKCOLORD,3,xyzs);
    }
    if(!flag||flag==2)
    {
        l=position_y-position_py;
        l*=DZC;
        if(l>=0)
        {
            xyzs[0]=' ';
        }
        else
        {
            xyzs[0]='-';
        }
        i=(long)l/1000;
        i=abs(i);
        itoa(i,xyzs+1,10);
        if(strlen(xyzs)>5)
        {
            strcpy(xyzs+1,xyzs+2);
        }
        i=l%1000;
        i=abs(i);
        xyzs1[1]=i/100+'0';
        i%=100;
        xyzs1[2]=i/10+'0';
        xyzs1[3]=i%10+'0';
        strcat(xyzs,xyzs1);
        Dispebar(DX0+13,DY0+23*6-1,DX1-1,EY0+23*6+20,COLORD,BKCOLORD,3,xyzs);
    }
    if(!flag||flag==3)
    {
        l=position_z-position_pz;
        l*=DZC;
        if(l>=0)
        {
            xyzs[0]=' ';
        }
        else
        {
            xyzs[0]='-';
        }
        i=(long)l/1000;
        i=abs(i);
        itoa(i,xyzs+1,10);
        if(strlen(xyzs)>5)
        {
            strcpy(xyzs+1,xyzs+2);
        }
        i=l%1000;
        i=abs(i);
        xyzs1[1]=i/100+'0';
        i%=100;
        xyzs1[2]=i/10+'0';
        xyzs1[3]=i%10+'0';
        strcat(xyzs,xyzs1);
        Dispebar(DX0+13,DY0+23*7-1,DX1-1,EY0+23*7+20,COLORD,BKCOLORD,3,xyzs);
    }
}
void ShowDP(char flag)
{
    static long positiond=0x8fffffff,positionp=0x8fffffff;
    char str[10];
    char str1[]=".000";
    int bkcolor;
    register i;
    long l;
    if(positiond!=position_d)
    {
        l=position_d*DZC;
        i=l/1000;
        if(!i&&l<0)
        {
            str[0]='-';
            itoa(l/1000,str+1,10);
        }
        else
        {
            itoa(l/1000,str,10);
        }
        str1[1]=48;
        str1[2]=48;
        str1[3]=48;
        i=l%1000;
        i=abs(i);
        str1[1]+=i/100;
        i%=100;
        str1[2]+=i/10;
        i%=10;
        str1[3]+=i;
        strcat(str,str1);
        if(!flag)
        {
            Dispbar3d(EX0+4,EY0+4+47*2,EX1-4,EY0+4+47*2+40,BKCOLORE1|COLORE1<<4,0,0x114,1,"当前深度");
        }
        Dispebar(EX0+4+18,EY0+4+47*2+21,EX1-4-18,EY0+4+47*2+21+16,COLORE2,BKCOLORE2,1,str);
        if(F12_flag>4&&F12_flag<9)
        {
            ShowFL(7);
            ShowFL(5);
            ShowFL(4);
        }
    }
    if(positionp!=position_p)
    {
        l=position_p*DZC;
        i=l/1000;
        if(!i&&l<0)
        {
            str[0]='-';
            itoa(l/1000,str+1,10);
        }
        else
        {
            itoa(l/1000,str,10);
        }
        str1[1]=48;
        str1[2]=48;
        str1[3]=48;
        i=l%1000;
        i=abs(i);
        str1[1]+=i/100;
        i%=100;
        str1[2]+=i/10;
        i%=10;
        str1[3]+=i;
        strcat(str,str1);
        if(!flag)
        {
            Dispbar3d(EX0+4,EY0+4+47*3,EX1-4,EY0+4+47*3+40,BKCOLORE1|COLORE1<<4,0,0x114,1,"目标深度");
        }
        Dispebar(EX0+4+18,EY0+4+47*3+21,EX1-4-18,EY0+4+47*3+21+16,COLORE2,BKCOLORE2,1,str);
        ShowDP1(3);
    }
}
void ShowFlag(char flags)
{
    int color,bkcolor,i;
    char flag=0;
    char str[3][5]= {" OFF"," ON ","G5\0\0"};
    char str1[4][5]= {"x1 ","x10","x100","*1"};
    if(flags==1||!flags)
    {
        if(F[1])
        {
            flag=1;
        }
        else
        {
            flag=0;
        }
        if(!flags)
        {
            Dispbar3d(EX0+4,EY0+4+47*6,(EX1+EX0)/2-1,EY0+4+47*6+40,BKCOLORE1|COLORE1<<4,0,0x100,1,"副电源");
        }
        Dispebar(EX0+4+9,EY0+4+47*6+21,(EX1+EX0)/2-1-9,EY0+4+47*6+21+16,COLORE2,BKCOLORE2,1,str[flag]);
    }
    if(flags==2||!flags)
    {
        flag=Pump_flag;
        if(!flags)
        {
            Dispbar3d((EX1+EX0)/2+1,EY0+4+47*6,EX1-4,EY0+4+47*6+40,BKCOLORE1|COLORE1<<4,0,0x100,1," 油泵 ");
        }
        Dispebar((EX1+EX0)/2+1+9,EY0+4+47*6+21,EX1-4-9,EY0+4+47*6+21+16,COLORE2,BKCOLORE2,1,str[flag]);
    }
    if(flags==3||!flags)
    {
        if(K[8])
        {
            flag=1;
        }
        else
        {
            flag=0;
        }
        if(!flags)
        {
            Dispbar3d(EX0+4,EY0+4+47*7,(EX1+EX0)/2-1,EY0+4+47*7+40,BKCOLORE1|COLORE1<<4,0,0x100,1,"反加工");
        }
        Dispebar(EX0+4+9,EY0+4+47*7+21,(EX1+EX0)/2-1-9,EY0+4+47*7+21+16,COLORE2,BKCOLORE2,1,str[flag]);
    }
    if(flags==4||!flags)
    {
        if(K[6])
        {
            flag=1;
        }
        else
        {
            flag=0;
        }
        if(!flags)
        {
            Dispbar3d((EX1+EX0)/2+1,EY0+4+47*7,EX1-4,EY0+4+47*7+40,BKCOLORE1|COLORE1<<4,0,0x100,1," 锁定 ");
        }
        Dispebar((EX1+EX0)/2+1+9,EY0+4+47*7+21,EX1-4-9,EY0+4+47*7+21+16,COLORE2,BKCOLORE2,1,str[flag]);
    }
    if(flags==5||!flags)
    {
        if(K[8])
        {
            flag=1;
        }
        else
        {
            flag=0;
        }
        if(!flags)
        {
            Dispbar3d(EX0+4,EY0+4+47*8,(EX1+EX0)/2-1,EY0+4+47*8+40,BKCOLORE1|COLORE1<<4,0,0x100,1," 速度 ");
        }
        Dispebar(EX0+4+9,EY0+4+47*8+21,(EX1+EX0)/2-1-9,EY0+4+47*8+21+16,COLORE2,BKCOLORE2,1,str1[SWF_flag]);
    }
    if(flags==6||!flags)
    {
        if(F[9]==1)
        {
            color=15-COLORE2;
            bkcolor=15-BKCOLORE2;
        }
        else
        {
            color=COLORE2;
            bkcolor=BKCOLORE2;
        }
        str[2][2]=XYZGroup%10+'0';
        str[2][3]=0;
        if(!flags)
        {
            Dispbar3d((EX1+EX0)/2+1,EY0+4+47*8,EX1-4,EY0+4+47*8+40,BKCOLORE1|COLORE1<<4,0,0x100,1," 坐标 ");
        }
        i=2;
        Dispebar((EX1+EX0)/2+1+9,EY0+4+47*8+21,EX1-4-9,EY0+4+47*8+21+16,color,bkcolor,1,str[2]+i);
    }
}
void ShowSWF()
{
    char str[4]= {"\x02\x04\x06\x00"};
    OTPS1&=0xf9;
    OTPS1|=str[SWF_flag];
    outb(OSC,0x01);
    outb(OSC+2,OTPS1);
    ShowFlag(5);
}
void ShowKey(int kn,int ke)
{
    int i,j,k,l;
    int ii=0;
    char str[10];
    static int kn1,ke1;
    if(kn==0xff)
    {
        ShowKey(kn1,ke1);
    }
    else
    {
        kn1=kn;
        ke1=ke;
    }
    if(!kn)
    {
        for(j=0; j<8; j++)
        {
            setcolor(COLORG);
            setfillstyle(1,BKCOLORG);
            rectangle(GX0+1+80*j,GY0+1,GX0+80*j+78,GY0+14);
            Disp8x16(GX0+80*(j+1)-8,GY0+7,2,COLORG,KeyStr(0,j));
        }
    }
    ii=0x1;
    if(kn==0)
    {
        kn=1;
    }
    k=0;
    if(!ke)
    {
        j=1;
        i=8;
    }
    else
    {
        j=ke;
        i=ke;
    }
    for(l=kn<<8; j<=i; j++)
    {
        l&=0xff00;
        l|=j;
        k=0;
        switch(l)
        {
        case 0x101:
            k=F[1];
            break;
        case 0x102:
            k=F[2];
            break;
        case 0x107:
            k=K[8];
            break;
        case 0x201:
            k=F[4];
            break;
        case 0x202:
            k=F[8];
            break;
        case 0x203:
            k=F[9];
            break;
        case 0x204:
            k=F[5];
            break;
        case 0x205:
            k=K[3];
            break;
        case 0x302:
            k=F[8];
            break;
        case 0x303:
            k=F[9];
            break;
        case 0x304:
            k=K[5];
            break;
        case 0x305:
            k=F[7];
            break;
        case 0x306:
            k=P[8];
            break;
        case 0x402:
            k=P[4];
            break;
        case 0x403:
            k=K[4];
            break;
        case 0x404:
            k=P[3];
            break;
        case 0x405:
            k=P[2];
            break;
        case 0x406:
            k=K[6];
            break;
        case 0x501:
            k=F[3];
            break;
        case 0x502:
            k=K[1];
            break;
        case 0x503:
            k=K[2];
            break;
        case 0x504:
            k=K[9];
            break;
        case 0x505:
            k=P[1];
            break;
        case 0x506:
            k=P[9];
            break;
        case 0x507:
            k=P[14];
            break;
        case 0x604:
            k=P[10];
            break;
        case 0x801:
            k=P[5];
            break;
        case 0x802:
            k=P[6];
            break;
        case 0x803:
            k=P[7];
            break;
        case 0x901:
            k=C[3];
            break;
        case 0x902:
            k=C[4];
            break;
        case 0x903:
            k=C[5];
            break;
        case 0x904:
            k=C[6];
            break;
        case 0x905:
            k=C[7];
            break;
        case 0x906:
            k=C[8];
            break;
        case 0xb05:
            k=P[12];
            break;
				case 0xA01:k=C[0];break;
        case 0xA02:k=C[1];break;
        case 0xA03:k=C[2];break;
        }
        if(l!=0x108&&l!=0xc08)
        {
            if(j==8)
            {
                if (SIONV)
                {
                    ShowFace1(-1,0x13);
                    ShowFace1(1,0x13);
                    ShowFace1(0,0x13);
                }
                else
                {
                    ShowFace1(-1,7);
                    ShowFace1(1,7);
                    ShowFace1(0,7);
                }
            }
            Dispbar3d(GX0+3+80*(j-1),GY0+18,X0-3+80*(j-1)+80,GY1-2,ii|0<<4|BKCOLORG<<8,k,1,2,KeyStr(kn,j-1));
        }
        else
        {
            Dispbar3d(GX0+3+80*(j-1),GY0+18,X0-3+80*(j-1)+80,GY1-2,ii|0<<4|BKCOLORG<<8,k,1,2,"");
            ShowFace1(-1,0xf);
            ShowFace1(1,8);
            ShowFace1(0,0xc);
        }
    }
}
void ShowTable(int lines)
{
    int i,j,k,k1,k0,y;
    int z;
    int color;
    register int k2;
    long l;
    char str[12];
    char str1[]= {'.',48,48,48,0};
    char str2[10];
    char str3[10];
    if(!lines||lines<Showline+1||lines>Showline+9)
    {
        if(lines<Showline+1)
        {
            Showline=lines-1;/*Showline--;*/
            if(Showline<0)
            {
                Showline=0;
            }
        }
        else if(lines>Showline+10)
        {
            Showline=lines-10;/*Showline++;*/
            if(Showline>5)
            {
                Showline=5;
            }
        }
        setfillstyle(1,BKCOLORB);
        bar(BX0+6,BY0+1+18*0,BX1,BY0+2+18*1);
        for(k1=BX0+4,j=0; j<14; j++)
        {
            List16x16M(k1+TabWidth1[j]/2+1,BY0+2+9,0xf,KeyStr(0x10,j));
            k1+=TabWidth[j];
        }
        k=0;
        z=10;
    }
    else
    {
        z=lines-Showline;
        k=lines-1-Showline;
    }
    if(lines>15)
    {
        return;
    }
    for(i=k; i<z; i++)
    {
        for(k1=BX0+3,k2=0,j=0; j<14; j++)
        {
            setfillstyle(1,8);
            color=0xf;
            if(((Dis_flag&&Dis_lines==i+Showline)^(F[6]&&i==YY-Showline&&(j==XX+1)))||(i>=Dis_start-Showline&&i<=Dis_end-Showline&&!j))
            {
                setfillstyle(1,0xf);
                color=0x4;
            }
            if(Table.Index[i+Showline]==1)
            {
                switch(j)
                {
                case    0:
                    itoa(i+Showline,str,10);
                    break;
                case    1:
                    l=Table.Shendu[i+Showline];
                    l*=DZC;
                    k0=l/1000;
                    if(!k0&&l<0)
                    {
                        str[0]='-';
                        itoa(l/1000,str+1,10);
                    }
                    else
                    {
                        itoa(l/1000,str,10);
                    }
                    str1[1]=48;
                    str1[2]=48;
                    str1[3]=48;
                    k0=l%1000;
                    k0=abs(k0);
                    str1[1]+=k0/100;
                    k0%=100;
                    str1[2]+=k0/10;
                    k0%=10;
                    str1[3]+=k0;
                    strcat(str,str1);
                    break;
                case    2:
                    strcpy(str,JixinS[Table.Jixin[i+Showline]]);
                    break;
                case    3:
                    itoa(Table.Dianliu[i+Showline]/2,str,10);
                    if(Table.Dianliu[i+Showline]&1)
                    {
                        if(strlen(str)==1)
                        {
                            str[1]='.';
                            str[2]='5';
                            str[3]=0;
                        }
                    }
                    break;
                case    4:
                    itoa(Table.Maikuan[i+Showline],str,10);
                    break;
                case    5:
                    itoa(Table.Xiuzhi[i+Showline],str,10);
                    break;
                case    6:
                    itoa(Table.Jianxi[i+Showline],str,10);
                    break;
                case    7:
                    itoa(Table.Sudu[i+Showline],str,10);
                    break;
                case    8:
                    itoa(Table.Shenggao[i+Showline],str,10);
                    break;
                case    9:
                    itoa(Table.Gongshi[i+Showline],str,10);
                    break;
                case    10:
                    itoa(LV[Table.LV[i+Showline]-1],str,10);
                    break;/*去掉,换成低压LV*/
                case    11:
                    itoa(Table.Gaoya[i+Showline],str3,10);
                    y=atoi(str3);
                    if(y<10)
                    {
                        strcpy(str,"0");
                    }
                    else
                    {
                        strcpy(str,"");
                    }
                    itoa(Table.Gaoya[i+Showline],str2,10);
                    strcat(str,str2);
                    break;
                case    12:
                    itoa(Table.PP[i+Showline],str3,10);
                    y=atoi(str3);
                    if(y<10)
                    {
                        strcpy(str,"0");
                    }
                    else
                    {
                        strcpy(str,"");
                    }
                    itoa(Table.PP[i+Showline],str2,10);
                    strcat(str,str2);
                    break;/*PP*/
                case    13:
                    itoa(Table.WcLc[i+Showline],str3,10);
                    y=atoi(str3);
                    if(y<10)
                    {
                        strcpy(str,"0");
                    }
                    else
                    {
                        strcpy(str,"");
                    }
                    itoa(Table.WcLc[i+Showline],str2,10);
                    strcat(str,str2);
                    break;/*LC*/
                }
            }
            else
            {
                strcpy(str,KeyStr(0x0f,0x00));
            }
            k2=TabWidth[j]-TabWidth1[j]/2;
            bar(k1+TabWidth1[j],BY0+(i+1)*18+6,k1+k2,BY0+(i+2)*18+3);
            Disp8x16(k1+k2-TabWidth2[j],BY0+18*(i+1)+13,2,color,str);
            k1+=TabWidth[j];
        }
    }

    if(KEYL==12)
    {
/*        ShowLong(KEYL);*/
        DispF6(0);
        ShowF6(YY+1);
    }

}
void ShowFL(int flag)
{
    if(flag)
    {
        return;
    }
}
void ShowTimeD(int flag)
{
    char xyzs[11]=" 00:00:00";
    unsigned long ll;
    unsigned i;
    if(flag==0x80)
    {
        ll=position_tt/60;
        ll%=(long)60*24*1000;
        i=ll/(60*24);
        xyzs[0]='0'+i/100;
        i%=100;
        xyzs[1]+=i/10;
        i%=10;
        xyzs[2]+=i;
        ll%=60*24;
        i=ll/60;
        xyzs[4]+=i/10;
        i%=10;
        xyzs[5]+=i;
        i=ll%60;
        xyzs[7]+=i/10;
        i%=10;
        xyzs[8]+=i;
    }
    else
    {
        ll=position_t;
        ll%=(long)3600*1000;
        i=ll/3600;
        xyzs[0]=' ';
        i%=100;
        xyzs[1]+=i/10;
        i%=10;
        xyzs[2]+=i;
        ll%=3600;
        i=ll/60;
        xyzs[4]+=i/10;
        i%=10;
        xyzs[5]+=i;
        i=ll%60;
        xyzs[7]+=i/10;
        i%=10;
        xyzs[8]+=i;
    }
    if(!flag)
    {
        Dispbar3d(EX0+4,EY0+4+48*1,EX1-4,EY0+4+48*1+40,BKCOLORE1|COLORE1<<4,0,0x114,1,"放电时间");
    }
    Dispebar(EX0+4+18,EY0+4+48*1+21,EX1-4-18,EY0+4+48*1+21+16,COLORE2,BKCOLORE2,1,xyzs);
}
void ShowTime(char flag)
{
    static long stime=0;
    long ll;
    int i;
    char xyzs[11]=" 00:00:00";
    ll=GetSTime();
    if(stime!=ll)
    {
        stime=ll;
        i=ll>>24;
        i&=0xff;
        xyzs[1]+=i/0x10;
        xyzs[2]+=i%0x10;
        i=ll>>16;
        i&=0xff;
        xyzs[4]+=i/0x10;
        xyzs[5]+=i%0x10;
        i=ll>>8;
        i&=0xff;
        xyzs[7]+=i/0x10;
        xyzs[8]+=i%0x10;
        if(!flag)
        {
            Dispbar3d(EX0+4,EY0+4+48*0,EX1-4,EY0+4+48*0+40,BKCOLORE1|COLORE1<<4,0,0x114,1,"当前时间");
        }
        Dispebar(EX0+4+18,EY0+4+48*0+21,EX1-4-18,EY0+4+48*0+21+16,COLORE2,BKCOLORE2,1,xyzs);
    }
}
void ShowXYZ(int flag)
{
    unsigned long ll;
    register long l,l1;
    register i,j;
    int color;
    int bkcolor;
    static long positionx=0x8ffffffa,positiony=0x8ffffffa,positionz=0x8ffffffa,positiont=0x8ffffffa;
    static int a=-1,v=-1,op=0;
    char xyzs[12],av[5],xyzss[12];
    char xyzs1[5]= {'.',48,48,48,0};
    char wxyzs[12];
    static char x1[12]= {0,0,0,0,0,0,0,0,0,0,0,0};
    static char y1[12]= {0,0,0,0,0,0,0,0,0,0,0,0};
    static char z1[12]= {0,0,0,0,0,0,0,0,0,0,0,0};
    if(flag>=0&&flag<=3)
    {
        for(i=0; i<9; i++)
        {
            x1[i]=y1[i]=z1[i]=1;
        }
        if(!flag||flag==1)
        {
            if(XYZ[0])
            {
                color=COLORA2;
            }
            else
            {
                color=COLORA1;
            }
            List40x40(AX0+10,5,color,'X');
            List40x40(AX1-6-38,5,COLORA1,'M');
        }
        if(!flag||flag==2)
        {
            if(XYZ[1])
            {
                color=COLORA2;
            }
            else
            {
                color=COLORA1;
            }
            List40x40(AX0+10,5+60,color,'Y');
            List40x40(AX1-6-38,5+60,COLORA1,'M');
        }
        if(!flag||flag==3)
        {
            if(XYZ[2])
            {
                color=COLORA2;
            }
            else
            {
                color=COLORA1;
            }
            List40x40(AX0+10,5+120,color,'Z');
            List40x40(AX1-6-38,5+120,COLORA1,'M');
        }
        if(!flag||flag==4)
        {
            Dispcbar(DX0+1,DY0+1+23*0,DX1-1,EY0+1+22*0+21,COLORD1,BKCOLORD,12,"机床坐标");
            Dispcbar(DX0+1,DY0+1+23*1,DX0+10,EY0+1+22*1+21,COLORD1,BKCOLORD,3,"X");
            Dispcbar(DX0+1,DY0+1+23*2,DX0+10,EY0+1+22*2+21,COLORD1,BKCOLORD,3,"Y");
            Dispcbar(DX0+1,DY0+1+23*3,DX0+10,EY0+1+22*3+21,COLORD1,BKCOLORD,3,"Z");
        }
    }
    if(positionx!=position_x)
    {
        l=position_x*DZC;
        i=l/1000;
        if(!i&&l<0)
        {
            xyzs[0]='-';
            itoa(l/1000,xyzs+1,10);
        }
        else
        {
            itoa(l/1000,xyzs,10);
        }
        i=strlen(xyzs);
        if(xyzs[0]=='-'&&i>5)
        {
            strcpy(xyzs+1,xyzs+i-4);
        }
        if(xyzs[0]!='-'&&i>4)
        {
            strcpy(xyzs,xyzs+i-4);
        }
        xyzs1[1]=48;
        xyzs1[2]=48;
        xyzs1[3]=48;
        i=l%1000;
        i=abs(i);
        xyzs1[1]+=i/100;
        i%=100;
        xyzs1[2]+=i/10;
        i%=10;
        xyzs1[3]+=i;
        strcat(xyzs,xyzs1);
        i=strlen(xyzs);
        for(i--,j=0; j<=i; j++)
        {
            xyzss[j]=xyzs[i-j];
        }
        for(; j<12; j++)
        {
            xyzss[j]=0;
        }
        for(i=0,j=0; xyzss[i]; i++)
        {
            if(!x1[i])
            {
                j++;
            }
            if(xyzss[i]!=x1[i]||j)
                if(i<4)
                {
                    List40x40(AX1-6-38*2-i*38,5,COLORA,xyzss[i]);
                }
                else
                {
                    List40x40(AX1-6-38*2-i*38+16,5,COLORA,xyzss[i]);
                }
        }
        if(strlen(x1)>strlen(xyzss))
            for(; x1[i]; i++)
            {
                if(i<4)
                {
                    List40x40(AX1-6-38*2-i*38,5,COLORA,' ');
                }
                else
                {
                    List40x40(AX1-6-38*2-i*38+16,5,COLORA,' ');
                }
            }
        strcpy(x1,xyzss);
        positionx=position_x;
        l=ox*DZC;
        if(l>=0)
        {
            xyzs[0]=' ';
        }
        else
        {
            xyzs[0]='-';
        }
        i=(long)l/1000;
        i=abs(i);
        itoa(i,xyzs+1,10);
        if(strlen(xyzs)>5)
        {
            strcpy(xyzs+1,xyzs+2);
        }
        i=l%1000;
        i=abs(i);
        xyzs1[1]=i/100+'0';
        i%=100;
        xyzs1[2]=i/10+'0';
        xyzs1[3]=i%10+'0';
        strcat(xyzs,xyzs1);
        Dispebar(DX0+13,DY0+1+23*1,DX1-1,EY0+1+23*1+21,COLORD,BKCOLORD,3,xyzs);
        ShowDP1(1);
        if(Dis_flag&&P[2])
        {
            ErrXY(1);
        }
    }
    if(positiony!=position_y)
    {
        l=position_y*DZC;
        i=l/1000;
        if(!i&&l<0)
        {
            xyzs[0]='-';
            itoa(l/1000,xyzs+1,10);
        }
        else
        {
            itoa(l/1000,xyzs,10);
        }
        i=strlen(xyzs);
        if(xyzs[0]=='-'&&i>5)
        {
            strcpy(xyzs+1,xyzs+i-4);
        }
        if(xyzs[0]!='-'&&i>4)
        {
            strcpy(xyzs,xyzs+i-4);
        }
        xyzs1[1]=48;
        xyzs1[2]=48;
        xyzs1[3]=48;
        i=l%1000;
        i=abs(i);
        xyzs1[1]+=i/100;
        i%=100;
        xyzs1[2]+=i/10;
        i%=10;
        xyzs1[3]+=i;
        strcat(xyzs,xyzs1);
        i=strlen(xyzs);
        for(i--,j=0; j<=i; j++)
        {
            xyzss[j]=xyzs[i-j];
        }
        for(; j<12; j++)
        {
            xyzss[j]=0;
        }
        for(i=0,j=0; xyzss[i]; i++)
        {
            if(!y1[i])
            {
                j++;
            }
            if(xyzss[i]!=y1[i]||j)
                if(i<4)
                {
                    List40x40(AX1-6-38*2-i*38,5+60,COLORA,xyzss[i]);
                }
                else
                {
                    List40x40(AX1-6-38*2-i*38+16,5+60,COLORA,xyzss[i]);
                }
        }
        if(strlen(y1)>strlen(xyzss))
            for(; y1[i]; i++)
            {
                if(i<4)
                {
                    List40x40(AX1-6-38*2-i*38,5+60,COLORA,' ');
                }
                else
                {
                    List40x40(AX1-6-38*2-i*38+16,5+60,COLORA,' ');
                }
            }
        strcpy(y1,xyzss);
        positiony=position_y;
        l=oy*DZC;
        if(l>=0)
        {
            xyzs[0]=' ';
        }
        else
        {
            xyzs[0]='-';
        }
        i=(long)l/1000;
        i=abs(i);
        itoa(i,xyzs+1,10);
        if(strlen(xyzs)>5)
        {
            strcpy(xyzs+1,xyzs+2);
        }
        i=l%1000;
        i=abs(i);
        xyzs1[1]=i/100+'0';
        i%=100;
        xyzs1[2]=i/10+'0';
        xyzs1[3]=i%10+'0';
        strcat(xyzs,xyzs1);
        Dispebar(DX0+13,DY0+1+23*2,DX1-1,EY0+1+23*2+21,COLORD,BKCOLORD,3,xyzs);
        ShowDP1(2);
        if(Dis_flag&&P[2])
        {
            ErrXY(2);
        }
    }
    if(positionz!=position_z)
    {
        l=position_z*DZC;
        i=l/1000;
        if(!i&&l<0)
        {
            xyzs[0]='-';
            itoa(l/1000,xyzs+1,10);
        }
        else
        {
            itoa(l/1000,xyzs,10);
        }
        i=strlen(xyzs);
        if(xyzs[0]=='-'&&i>5)
        {
            strcpy(xyzs+1,xyzs+i-4);
        }
        if(xyzs[0]!='-'&&i>4)
        {
            strcpy(xyzs,xyzs+i-4);
        }
        xyzs1[1]=48;
        xyzs1[2]=48;
        xyzs1[3]=48;
        i=l%1000;
        i=abs(i);
        xyzs1[1]+=i/100;
        i%=100;
        xyzs1[2]+=i/10;
        i%=10;
        xyzs1[3]+=i;
        strcat(xyzs,xyzs1);
        i=strlen(xyzs);
        for(i--,j=0; j<=i; j++)
        {
            xyzss[j]=xyzs[i-j];
        }
        for(; j<12; j++)
        {
            xyzss[j]=0;
        }
        for(i=0,j=0; xyzss[i]; i++)
        {
            if(!z1[i])
            {
                j++;
            }
            if(xyzss[i]!=z1[i]||j)
            {
                if(i<4)
                {
                    List40x40(AX1-6-38*2-i*38,5+120,COLORA,xyzss[i]);
                }
                else
                {
                    List40x40(AX1-6-38*2-i*38+16,5+120,COLORA,xyzss[i]);
                }
            }
        }
        if(strlen(z1)>strlen(xyzss))
            for(; z1[i]; i++)
            {
                if(i<4)
                {
                    List40x40(AX1-6-38*2-i*38,5+120,COLORA,' ');
                }
                else
                {
                    List40x40(AX1-6-38*2-i*38+16,5+120,COLORA,' ');
                }
            }
        strcpy(z1,xyzss);
        positionz=position_z;
        if(op==0)
        {
            lpz=Read_spi(0x3c);
            lpz-=Read_spi(0x38);
            oz=ZPosi(0);
            op=1;
        }
        diff_oz=oz-lpz;
        l=diff_oz*DZC;
        if(l>=0)
        {
            xyzs[0]=' ';
        }
        else
        {
            xyzs[0]='-';
        }
        i=(long)l/1000;
        i=abs(i);
        itoa(i,xyzs+1,10);
        if(strlen(xyzs)>5)
        {
            strcpy(xyzs+1,xyzs+2);
        }
        i=l%1000;
        i=abs(i);
        xyzs1[1]=i/100+'0';
        i%=100;
        xyzs1[2]=i/10+'0';
        xyzs1[3]=i%10+'0';
        strcat(xyzs,xyzs1);
        Dispebar(DX0+13,DY0+1+23*3,DX1-1,EY0+1+23*3+21,COLORD,BKCOLORD,3,xyzs);
        ShowDP1(3);
        if(!Origin_Flag&&!(ORgin&0x80)&&ZPosi(0))
        {
            Origin_Flag=1;
        }
    }
    else
    {
        if((ORgin&0x80)&&(ORgin&0x40))
        {
            ORgin&=0x3f;
            if(ORgin==1)
            {
                ShowMess(45);
                offset_z=F_positionz-(s_OZh-s_OZl+lpz);
                for(i=0; i<10; i++)
                {
                    offset_zz[i]=position[i]-(s_OZh-s_OZl+lpz);
                }
                Write_spi(0x3c,lpz);
            }
            else
            {
                ShowMess(46);
            }
            ORgin=0;
        }
    }
    if(v!=V)
    {
        itoa(V,av,10);
        v=V;
        if(!flag)
        {
            Dispbar3d(EX0+4,EY0+4+47*4,(EX1+EX0)/2-1,EY0+4+47*4+40,BKCOLORE1|COLORE1<<4,0,0x100,1," 电压 ");
        }
        Dispebar(EX0+4+9,EY0+4+47*4+21,(EX1+EX0)/2-1-9,EY0+4+47*4+21+16,COLORE2,BKCOLORE2,1,av);
    }
    if(a!=A)
    {
        itoa(A,av,10);
        a=A;
        if(!flag)
        {
            Dispbar3d((EX1+EX0)/2+1,EY0+4+47*4,EX1-4,EY0+4+47*4+40,BKCOLORE1|COLORE1<<4,0,0x100,1," 电流 ");
        }
        Dispebar((EX1+EX0)/2+1+9,EY0+4+47*4+21,EX1-4-9,EY0+4+47*4+21+16,COLORE2,BKCOLORE2,1,av);
    }
    if(positiont!=position_t)
    {
        ShowTimeD(1);
        positiont=position_t;
    }
}
char *KeyStr(int y,int x)
{
    unsigned char a;
    char stron1[]="清除",stron2[]="停机",stroff[]="关闭";
    char * keystr[0x12][14]=
    {
        /*0*/   {"   F1   ","   F2   ","   F3   ","   F4   ","   F5   ","   F6   ","   F7   ","   F8   "},
        /*1*/   {"副 电 源"," 油  泵 "," 坐  标 "," 放  电 "," 安  全 "," 系  统 ","反 加 工",""},
        /*2*/   {" 归  零 ","组 别 00"," 坐标 0 "," 中  心 "," 校  模 "," 归原点","  移动 "," 返  回 "},
        /*3*/   {"参 数 表","组 别 00"," 坐标 0 "," 位  置"," 高  度 ","钢蚀钢00",""," 返  回 "},
        /*4*/   {" 积  炭 "," 火  警 "," 油  位 "," 深  孔 ","XY 保 护"," 锁  定 ",""," 返  回 "},
        /*5*/   {" 声 音 0"," 睡  眠 ","上 升 00"," 关  机 "," 清  时 ","        "," 设  置 "," 返  回 "},
        /*6*/   {" 定  段 ","自 动 表","删 除 表"," 修  底 ","","","","确定返回"},
        /*7*/   {" 确  定 "," 取  消 ","","","","","","取消返回"},
        /*8*/   {"时间 000","高度 000","积炭关闭","","","",""," 返  回 "},
        /*9*/   {" X+归零 "," X-归零 "," Y+归零 "," Y-归零 "," Z+归零 "," Z-归零 ","","取消返回"},
        /*A*/   {"   X  ","   Y  ","   Z  ","","","","","取消返回"},
        /*B*/   {" 确  定 "," 取  消 "," 删  除 ","","多点加工","","","确定返回"},
        /*C*/   {""},
        /*D*/   {""},
        /*E*/   {""},
        /*F*/   {""},
        {"组00"," 设定深度 "," PL"," IP"," ON","OFF"," SV"," S"," UP"," DN"," LV" ," HV" ," PP"," LC"},
        {"  ","          ","    ","    ","    ","    ","    ","    ","    ","    ","    " ,"    "}
    };
    if(y==2&&x==1)
    {
        keystr[y][x][6]=Group/10+'0';
        keystr[y][x][7]=Group%10+'0';
    }
    if(y==2&&x==2)
    {
        keystr[y][x][6]=XYZGroup%10+'0';
    }
    if(y==3&&x==2)
    {
        keystr[y][x][6]=XYZGroup%10+'0';
    }
    if(y==3&&x==1)
    {
        keystr[y][x][6]=Group/10+'0';
        keystr[y][x][7]=Group%10+'0';
    }
    if(y==3&&x==5)
    {
        keystr[y][x][6]=Steel_flag/10+'0';
        keystr[y][x][7]=Steel_flag%10+'0';
    }
    if(y==5&&x==0)
    {
        keystr[y][x][7]=soundV%10+'0';
    }
    if(y==5&&x==2)
    {
        keystr[y][x][6]=K_3/10+'0';
        keystr[y][x][7]=K_3%10+'0';
    }
    if(y==8&&x==0)
    {
        keystr[y][x][5]=AI_time/100+'0';
        keystr[y][x][6]=(AI_time%100)/10+'0';
        keystr[y][x][7]=AI_time%10+'0';
    }
    if(y==8&&x==1)
    {
        keystr[y][x][5]=AI_high/100+'0';
        keystr[y][x][6]=(AI_high%100)/10+'0';
        keystr[y][x][7]=AI_high%10+'0';
    }
    if(y==8&&x==2)
    {
        keystr[y][x][4]=0;
        if(!P[7])
        {
            strcat(keystr[y][x],stroff);
        }
        else
        {
            if(P[7]==1)
            {
                strcat(keystr[y][x],stron1);
            }
            if(P[7]==2)
            {
                strcat(keystr[y][x],stron2);
            }
        }
    }
    outb(OSC,0xdf);
    a=inportb(OSC+1);
    a|=0xfd;
    if(y==5&&x==5)
    {
        if((a&=0x02)||(PassWordF==0))
        {
            strcpy(keystr[y][x],"  口  令");    /*显示口令*/
        }
    }
    if(y==0x10&&x==0)
    {
        keystr[y][x][2]=Group/10+'0';
        keystr[y][x][3]=Group%10+'0';
    }
    return keystr[y][x];
}
void ShowLong(long value)
{
    char str[]="Ver:         \0";
    itoa(value,str,10);
    Dispebar(CX1-73,CY0+17,CX1-1,CY1-1,COLORC-1,BKCOLORC,2,str);
}
void ShowVer(char *ver)
{
    char str[]="Ver:     \0";
    str[4]=ver[0];
    str[5]=ver[1];
    str[6]=ver[2];
    str[7]=ver[3];
    str[8]=ver[4];
    Dispebar(CX1-73,CY0+17,CX1-1,CY1-1,COLORC-1,BKCOLORC,2,str);
}
void ShowMess(int value)
{
    static oldvalue,oldvalue1;
    char *showinput[60]=
    {
        "",
        "设定深度 \"-9999.999~9999.999\"",
        "电压极性 +/-",
        "峰值电流 0~9.5~35 (A)",
        "加工脉宽 1~999 (us)",
        "休止时间 3~999 (us)",
        "间隙电压 20~120 (V)",
        "加工速度 1~8",
        "跳升高度 0~999 (1/10mm)",
        "加工时间 1~99 (1/10s)",
        "主电压 1(90V) 2(120V)",/* 10 */
        "高压 1(160V)2(200V)3(260V)//1(0.5A)2(1A)3(2A)",
        "",
        "",
        "",
        "无效操作",
        "跳升高度 1~99(mm)",
        "跳升次数 1~99",
        "组 1~99",
        "上升 1~99(mm)",
        "速度 5~50",         /* 20 */
        "钢蚀钢 1~99",
        "修底!",
        "Error at HTCL-1100!!",
        "Error at U24!",
        "Memory Error!",
        "坐标组 1~9",
        "等待油位",
        "加工完毕",
        "请输入口令:",
        "口令错误!",          /*30*/
        "正在关机..",
        "无参数",
        "需要放电密码!",
        "硬件错误!",
        "X轴被移动",
        "Y轴被移动",
        "Z轴被移动",
        "输入积炭时: 1~999(sec)",
        "输入积炭高: 1~999(um)",
        "分辨率1um,5um",     /*40*/
        "+正向,-负向",
        "1清零0不清零",
        "提示，三个运动轴不能一致!",
        "三个运动轴不能一致!",
        "归原点成功!",
        "归原点失败!",
        "短路，归原点失败!",
        "Z轴已被移动，请重启机器后再归原点",
        "解锁成功,正在重新启动..",
        "35A/75A",/*50*/
        "输入X坐标值 \"-9999.999~9999.999\"",
        "输入Y坐标值 \"-9999.999~9999.999\"",
        "输入Z坐标值 \"-9999.999~9999.999\"",
        "脉冲波形 短路保护 0关1开//等能量 0关1开",
        "回路 电感 1关2低3高//电容 1~9",
        "X轴增益 0~100",
        "Y轴增益 0~100",
        "Z轴增益 0~100",
        "锁定已解除.."
    };
    char * showinput1[9]=
    {
        "",
        "-9999.999~9999.999",
        "-9999.999~9999.999",/*130*/
        "电流——1~191A",
        "面积——1~99cm",
        "材料——Cu-St Gr-St Cu-Wc",
        "效果——1~4",
        "形状——平面 深孔 清角",
        "制程——粗 正常"
    };
    char * showinput2[5]=
    {
        "剩余次数:",
        "您的ID是:",
        "请输入密码:",
        "正在重新启动..",
        "您的使用期限还剩1天!"
    };
    if(DZC==5)
    {
        showinput[1][27]='5';
        showinput[1][18]='5';
        showinput[12][30]='5';
        showinput[12][21]='5';
        showinput[13][30]='5';
        showinput[13][21]='5';
        showinput[14][30]='5';
        showinput[14][21]='5';
        showinput1[2][8]='5';
        showinput1[2][17]='5';
        strcpy(showinput[19],"速度 10~99");
    }
    if(sl5==1)
    {
        strcpy(showinput[3],"峰值电流 0~9.5~75 (A)");
    }
    if(value<0x80&&value!=15)
    {
        oldvalue=value;
    }
    if(value==0xfd)
    {
        oldvalue1=oldvalue;
        return;
    }
    if(value==0xfe)
    {
        ShowMess(oldvalue1);
        return;
    }
    if(value==0xff)
    {
        ShowMess(oldvalue);
        return;
    }
    if(!value)
    {
        setfillstyle(1,0x8);
        bar(CX0+1,CY0+1,CX1,CY1-17);   /*-80 Ver*/
        List16x16M(CX0+1,CY0+10,0xf,"讯息:");
        SMessage=0;
        return;
    }
    if(value<0x80)
    {
        setfillstyle(1,0x8);
        bar(CX0+40,CY0+1,CX1,CY1-17);
        List16x16M(CX0+40,CY0+10,0xf,showinput[value]);
        if(value==15)
        {
            SMessage=20;
        }
    }
    else
    {
        value&=0x7f;
        if(value<0x10)
        {
            setfillstyle(1,0x8);
            bar(CX0+40,CY0+1,CX1,CY1-17);
            List16x16M(CX0+40,CY0+10,0xf,showinput1[value]);
        }
        else
        {
            value-=0x10;
            if(value<4)
            {
                Dispcbar(F9X+12,F9Y+5+(value)*20+30,F9X+112,F9Y+5+(value)*20+16+30,0,7,1,showinput2[value]);
            }
            else
            {
                List16x16M(CX0+1,CY0+27,0xf,showinput2[value]);
            }
        }
    }
}

void ShowAuto(int value)
{
    int current[3][3]= {{95,63,31},{191,63,31},{31,31,31}};
    char cur[5];
    int i=0;
    char * showinput1[9]=
    {
        "",
        "-9999.999~9999.999",
        "-9999.999~9999.999",/*130*/
        "电流——1~       ",
        "面积——1~99cm",
        "材料——铜-钢 石墨-钢 铜-钨钢",
        "效果——1~4",
        "形状——平面 深孔 清角 ",
        "制程——粗 精"
    };
    itoa(current[cMaterial-1][cShape-1],cur,10);
    for(i=0; i<5; i++)
    {
        if(cur[i]=='\0')
        {
            break;
        }
        showinput1[3][10+i]=cur[i];
    }
    showinput1[3][10+i]='A';
    i++;
    showinput1[3][10+i]='\0';
    /*strcat(showinput1[3],cur);*/
    if(DZC==5)
    {
        showinput1[2][8]='5';
        showinput1[2][17]='5';
    }
    if(value<0x80)
    {
        return;
    }
    else
    {
        value&=0x7f;
        if(value<0x10)
        {
            setfillstyle(1,0x8);
            bar(CX0+40,CY0+1,CX1,CY1-17);
            List16x16M(CX0+40,CY0+10,0xf,showinput1[value]);
        }
    }
}

void windows(int left,int top,int right,int bottom,int newattr)
{
    window(left,top,right,bottom);
    textattr(newattr);
    clrscr();
}
void CheckTab()
{
    int i;
    for(i=0; i<10; i++)
    {
        if(Table.Index[i]!=1)
        {
            continue;
        }
        if(Table.Shendu[i]<StrTable.Shendu[0])
        {
            Table.Shendu[i]=StrTable.Shendu[0];
        }
        if(Table.Shendu[i]>StrTable.Shendu[1])
        {
            Table.Shendu[i]=StrTable.Shendu[1];
        }
        if(Table.Dianliu[i]<StrTable.Dianliu[0])
        {
            Table.Dianliu[i]=StrTable.Dianliu[0];
        }
        if(Table.Dianliu[i]>StrTable.Dianliu[1])
        {
            Table.Dianliu[i]=StrTable.Dianliu[1];
        }
        if(Table.Maikuan[i]<StrTable.Maikuan[0])
        {
            Table.Maikuan[i]=StrTable.Maikuan[0];
        }
        if(Table.Maikuan[i]>StrTable.Maikuan[1])
        {
            Table.Maikuan[i]=StrTable.Maikuan[1];
        }
        if(Table.Xiuzhi[i]<StrTable.Xiuzhi[0])
        {
            Table.Xiuzhi[i]=StrTable.Xiuzhi[0];
        }
        if(Table.Xiuzhi[i]>StrTable.Xiuzhi[1])
        {
            Table.Xiuzhi[i]=StrTable.Xiuzhi[1];
        }
        if(!Table.Jianxi[i]==0)
            if(Table.Jianxi[i]<StrTable.Jianxi[0])
            {
                Table.Jianxi[i]=StrTable.Jianxi[0];
            }
        if(Table.Jianxi[i]>StrTable.Jianxi[1])
        {
            Table.Jianxi[i]=StrTable.Jianxi[1];
        }
        if(Table.Sudu[i]<StrTable.Sudu[0])
        {
            Table.Sudu[i]=StrTable.Sudu[0];
        }
        if(Table.Sudu[i]>StrTable.Sudu[1])
        {
            Table.Sudu[i]=StrTable.Sudu[1];
        }
        if(Table.Shenggao[i]<StrTable.Shenggao[0])
        {
            Table.Shenggao[i]=StrTable.Shenggao[0];
        }
        if(Table.Shenggao[i]>StrTable.Shenggao[1])
        {
            Table.Shenggao[i]=StrTable.Shenggao[1];
        }
        if(Table.Gongshi[i]<StrTable.Gongshi[0])
        {
            Table.Gongshi[i]=StrTable.Gongshi[0];
        }
        if(Table.Gongshi[i]>StrTable.Gongshi[1])
        {
            Table.Gongshi[i]=StrTable.Gongshi[1];
        }
        if(Table.LV[i]<StrTable.LV[0])
        {
            Table.LV[i]=StrTable.LV[0];
        }
        if(Table.LV[i]>StrTable.LV[1])
        {
            Table.LV[i]=StrTable.LV[1];
        }
        if(Table.Jixin[i]<StrTable.Jixin[0])
        {
            Table.Jixin[i]=StrTable.Jixin[0];
        }
        if(Table.Jixin[i]>StrTable.Jixin[1])
        {
            Table.Jixin[i]=StrTable.Jixin[1];
        }
        if(Table.Gaoya[i]<StrTable.Gaoya[0])
        {
            Table.Gaoya[i]=StrTable.Gaoya[0];
        }
        if(Table.Gaoya[i]>StrTable.Gaoya[1])
        {
            Table.Gaoya[i]=StrTable.Gaoya[1];
        }
        /*3.7取值范围*/
        if(Table.Gaoya[i]%10>3)
        {
            Table.Gaoya[i]=(Table.Gaoya[i]/10)*10;
            Table.Gaoya[i]+=3;
        }
        if(Table.PP[i]<StrTable.PP[0])
        {
            Table.PP[i]=StrTable.PP[0];
        }
        if(Table.PP[i]>StrTable.PP[1])
        {
            Table.PP[i]=StrTable.PP[1];
        }
        /*3.7取值范围*/
        if(Table.PP[i]%10>1)
        {
            Table.PP[i]=(Table.PP[i]/10)*10;
            Table.PP[i]+=1;
        }
        if(Table.WcLc[i]<StrTable.WcLc[0])
        {
            Table.WcLc[i]=StrTable.WcLc[0];
        }
        if(Table.WcLc[i]>StrTable.WcLc[1])
        {
            Table.WcLc[i]=StrTable.WcLc[1];
        }
        /*3.7取值范围*/
        if(Table.WcLc[i]%10>9)
        {
            Table.WcLc[i]=(Table.WcLc[i]/10)*10;
            Table.WcLc[i]+=9;
        }
    }
}
/*把group所指定组别的数据从SPI传递给全局变量Table*/
void Read_Table(unsigned char group)
{
    char far *pi;
    int i,j;
    j=0x130+group*0x130;
    pi=(char far * )&Table;
    for(i=0; i<sizeof(Table); i++)
    {
        *pi++=ReadSPI(j++);
    }
    for(i=0; i<15; i++)
    {
        if(!Table.Index[i])
        {
            Table.Shendu[i]=StrTable.Shendu[2];
            Table.Jixin[i]=StrTable.Jixin[2];
            Table.Dianliu[i]=StrTable.Dianliu[2];
            Table.Maikuan[i]=StrTable.Maikuan[2];
            Table.Xiuzhi[i]=StrTable.Xiuzhi[2];
            Table.Jianxi[i]=StrTable.Jianxi[2];
            Table.Sudu[i]=StrTable.Sudu[2];
            Table.Shenggao[i]=StrTable.Shenggao[2];
            Table.Gongshi[i]=StrTable.Gongshi[2];
            Table.Gaoya[i]=StrTable.Gaoya[2];
        }
    }
    CheckTab();
}
/*把group所指定组别的数据从SPI传递给全局变量Table*/
void Write_Table(unsigned char group)
{
    char far *pi;
    char a;
    int i,j;
    j=0x130+group*0x130;
    pi=(char far * )&Table;
    for(i=0; i<sizeof(Table); i++)
    {
        a=*pi;
        pi++;
        WriteSPI(j++,a);
    }
}
/*把group所指定组别的数据从SPI传递给全局变量Node*/
void Read_Nodes(unsigned char group)
{
    char far *pi;
    int i,j;
    j=0x77f0+group*0x130;
    pi=(char far * )&Node;
    for(i=0; i<sizeof(Node); i++)
    {
        *pi++=ReadSPI(j++);
    }
    for(i=0; i<20; i++)
    {
        if(!Node.flag[i])
        {
            Node.x_pos[i]=-1;
            Node.y_pos[i]=-1;
        }
    }
}
/*把group所指定组别的数据从SPI传递给全局变量Node*/
void Write_Nodes(unsigned char group)
{
    char far *pi;
    char a;
    int i,j;
    j=0x77f0+group*0x130;
    pi=(char far * )&Node;
    for(i=0; i<sizeof(Node); i++)
    {
        a=*pi;
        pi++;
        WriteSPI(j++,a);
    }
}
void ResetXYZ()
{
    long far * pl;
    int i,k;
    register long ll;
    unsigned long l1;
    unsigned char ktime[6];
    long lc_time,lo_time,ks;
    ll=Read_spi(0x40);
    if(ll<-9999999/DZC)
    {
        ll=-9999999/DZC;
    }
    if(ll>9999999/DZC)
    {
        ll=9999999/DZC;
    }
    offset_rx=XPosi(0);
    offset_x=ll-offset_rx;
    ll=Read_spi(0x44);
    if(ll<-9999999/DZC)
    {
        ll=-9999999/DZC;
    }
    if(ll>9999999/DZC)
    {
        ll=9999999/DZC;
    }
    offset_ry=YPosi(0);
    offset_y=ll-offset_ry;
    ll=Read_spi(0x48);
    F_positionz=ll;
    if(ll<-9999999/DZC)
    {
        ll=-9999999/DZC;
    }
    if(ll>9999999/DZC)
    {
        ll=9999999/DZC;
    }
    offset_rz=ZPosi(0);
    R_positionz=ZPosi(0);
    offset_z=ll-offset_rz;
    position_t=Read_spi(0x30);
    position_tt=Read_spi(0x34);
    for(i=0; i<10; i++)
    {
        ll=Read_spi(0x50+i*16);
        if(ll<-9999999/DZC)
        {
            ll=-9999999/DZC;
        }
        if(ll>9999999/DZC)
        {
            ll=9999999/DZC;
        }
        offset_xx[i]=ll-XPosi(0);
        ll=Read_spi(0x54+i*16);
        if(ll<-9999999/DZC)
        {
            ll=-9999999/DZC;
        }
        if(ll>9999999/DZC)
        {
            ll=9999999/DZC;
        }
        offset_yy[i]=ll-YPosi(0);
        ll=Read_spi(0x58+i*16);
        if(ll<-9999999/DZC)
        {
            ll=-99999999/DZC;
        }
        if(ll>9999999/DZC)
        {
            ll=9999999/DZC;
        }
        offset_zz[i]=ll-ZPosi(0);
        position[i]=ll;
    }
    Group=ReadSPI(0x0d);
    ll=Read_spi(0x20);
    oz=ZPosi(0);
    s_OZh=Read_spi(0x38);
    s_OZl=Read_spi(0x3c);
    OldID=ll;
    ll>>=16;
    OldDATE=ll;
    XYZGroup=ReadSPI(0x0e);
    if(XYZGroup<1||XYZGroup>9)
    {
        XYZGroup=1;
    }
    if(Group<1||Group>99)
    {
        Group=1;
    }
    XYPosition();
    position_d=position_z;
    position_dx=position_x;
    position_dy=position_y;
    position_dz=position_z;
    position_px=position_x;
    position_py=position_y;
    position_sx=Read_spi(0x100);
    position_sy=Read_spi(0x104);
    Dis_pos=ReadSPI(0x108);
    P[12]=ReadSPI(0x10a);
    x_Pid_kp=ReadSPI(0x110);
    y_Pid_kp=ReadSPI(0x114);
    z_Pid_kp=ReadSPI(0x118);
    if(x_Pid_kp==0x00&&y_Pid_kp==0x00&&z_Pid_kp==0x00)
    {
        x_Pid_kp=0x10;
        y_Pid_kp=0x10;
        z_Pid_kp=0x20;
        WriteSPI(0x110,x_Pid_kp);
        WriteSPI(0x114,y_Pid_kp);
        WriteSPI(0x118,z_Pid_kp);
    }
    lo_time = ReadSPI(0x6);
    lo_time <<= 24;
    ks = ReadSPI(0x7);
    lo_time += (ks<<16);
    ks = ReadSPI(0x8);
    lo_time += (ks<<8);
    ks = ReadSPI(0x9);
    lo_time += ks;
    lc_time = Get_password_time();
    if(lc_time >= lo_time)
    {
        dis_time = lc_time - lo_time;
        dis_time *= 15;                /*转换为4s*/
    }
    else
    {
        dis_time = 0xffffffff;
    }
    for(i=0; i<6; i++)
    {
        ktime[i] = ReadSPI(0x0+i);
    }
    outb(OSC,0x53);                     /*写时间*/
    outb(OSC+2,ktime[5]);
    outb(OSC+1,ktime[4]);
    outb(OSC,0x43);
    outb(OSC+2,ktime[3]);
    outb(OSC+1,ktime[2]);
    outb(OSC,0x33);
    outb(OSC+2,ktime[1]);
    outb(OSC+1,ktime[0]);
    outb(OSC,0x34);                     /*写关机时间差*/
    outb(OSC+1,dis_time);
    outb(OSC+2,dis_time>>8);
    outb(OSC+3,dis_time>>16);
    delay(0x50);
    OTPS0|=0x20;
    outb(OSC,0x01);
    outportb(OSC+1,OTPS0);
    delay(0x50);
    OTPS0&=0xdf;
    outb(OSC,0x01);
    outportb(OSC+1,OTPS0);
}
void SaveXYZ(char flag)
{
    int i;
    long l,lt;
    register long ol;
    static long positionx=0x7fffffff,positiony=0x7fffffff,positionz=0x7fffffff,positiont=0x7fffffff;
    unsigned char a,ktime[6],temp[10];
    XYPosition();
    if(!flag||positionx!=position_x)
    {
        ol=position_x-offset_x;
        l=labs(positionx-position_x);
        Write_spi(0x40,position_x);
        for(i=0; i<10; i++)
        {
            Write_spi(0x50+i*16,offset_xx[i]+ol);
        }
        positionx=position_x;
        if(Dis_flag)if(labs(position_x-position_xdis)>1)
            {
                /*ErrXY(1);*/position_xdis=position_x;
            }
        offset_ds=0;
    }
    if(!flag||positiony!=position_y)
    {
        ol=position_y-offset_y;
        l=labs(positiony-position_y);
        Write_spi(0x44,position_y);
        for(i=0; i<10; i++)
        {
            Write_spi(0x54+i*16,offset_yy[i]+ol);
        }
        positiony=position_y;
        if(Dis_flag)if(labs(position_y-position_ydis)>1)
            {
                /*ErrXY(2);*/position_ydis=position_y;
            }
        offset_ds=0;
    }
    if(!flag||positionz!=position_z)
    {
        ol=position_z-offset_z;
        l=labs(positionz-position_z);
        Write_spi(0x48,position_z);
        for(i=0; i<10; i++)
        {
            Write_spi(0x58+i*16,offset_zz[i]+ol);
        }
        positionz=position_z;
        if(lock_z&&l>=5/DZC)
        {
            Position_control_z(offset_rz);
        }
        if(Dis_flag)
        {
            if(positionz<=position_d&&!K[8])
            {
                if(position_d<=position_d)
                {
                    position_d=position_z;
                }
                if(Dis_flag)
                {
                    ShowDP(1);
                }
            }
            if(positionz>=position_d&&K[8])
            {
                if(position_d>=position_d)
                {
                    position_d=position_z;
                }
                if(Dis_flag)
                {
                    ShowDP(1);
                }
            }
        }
        oz=ZPosi(0);
        Write_spi(0x38,oz);
    }
    if(positiont!=position_t)
    {
        Write_spi(0x30,position_t);
        positiont=position_t;
        Write_spi(0x34,position_tt);
    }
}
long XPosi(char flag)   /*读取X轴计数器的数据*/
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(ICXaddr,0x08);
    i2=inportb(ICXaddr+3);
    i1=inportb(ICXaddr+2);
    i0=inportb(ICXaddr+1);
    if(i2&0x80)
    {
        i3=0xff;
    }
    else
    {
        i3=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
long YPosi(char flag)   /*读取Y轴计数器的数据*/
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(ICYaddr,0x08);
    i2=inportb(ICYaddr+3);
    i1=inportb(ICYaddr+2);
    i0=inportb(ICYaddr+1);
    if(i2&0x80)
    {
        i3=0xff;
    }
    else
    {
        i3=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
long ZPosi(char flag)  /*读取Z轴计数器的数据*/
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(Address_Z,0x08);
    i2=inportb(Address_Z+3);
    i1=inportb(Address_Z+2);
    i0=inportb(Address_Z+1);
    if(i2&0x80)
    {
        i3=0xff;
    }
    else
    {
        i3=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
long OSCPosi4(char flag)
{
    unsigned char   i0,i1,i2=0,i3;
    long    ll;
    outb(OSC,0x10);
    i1=inportb(OSC+2);
    i0=inportb(OSC+1);
    if(i2&0x80)
    {
        i3=0xff;
        i2=0xff;
    }
    else
    {
        i3=0;
        i2=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
long OSCPosi5(char flag)
{
    unsigned char   i0,i1,i2=0,i3;
    long    ll;
    outb(OSC,0x20);
    i1=inportb(OSC+2);
    i0=inportb(OSC+1);
    if(i2&0x80)
    {
        i3=0xff;
        i2=0xff;
    }
    else
    {
        i3=0;
        i2=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
long OSCPosi6(char flag)
{
    unsigned char   i0,i1,i2=0,i3;
    long    ll;
    outb(OSC,0x40);
    i1=inportb(OSC+2);
    i0=inportb(OSC+1);
    if(i2&0x80)
    {
        i3=0xff;
        i2=0xff;
    }
    else
    {
        i3=0;
        i2=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
long OSCPosi7(char flag)
{
    unsigned char   i0,i1,i2=0,i3;
    long    ll;
    outb(OSC,0x80);
    i1=inportb(OSC+2);
    i0=inportb(OSC+1);
    if(i2&0x80)
    {
        i3=0xff;
        i2=0xff;
    }
    else
    {
        i3=0;
        i2=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
long Read_Velocity_x(char flag)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(ICXaddr,0x40);
    i1=inportb(ICXaddr+2);
    i0=inportb(ICXaddr+1);
    if(i1&0x80)
    {
        i3=0xff;
        i2=0xff;
    }
    else
    {
        i3=0;
        i2=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
long Read_Velocity_y(char flag)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(ICYaddr,0x40);
    i1=inportb(ICYaddr+2);
    i0=inportb(ICYaddr+1);
    if(i1&0x80)
    {
        i3=0xff;
        i2=0xff;
    }
    else
    {
        i3=0;
        i2=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
long Read_Velocity_z(char flag)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(Address_Z,0x40);
    i1=inportb(Address_Z+2);
    i0=inportb(Address_Z+1);
    if(i1&0x80)
    {
        i3=0xff;
        i2=0xff;
    }
    else
    {
        i3=0;
        i2=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
long ZPosi_origin(char flag)    /*读取Z轴原点的数据*/
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(Address_Z,0x20);
    i2=inportb(Address_Z+3);
    i1=inportb(Address_Z+2);
    i0=inportb(Address_Z+1);
    if(i2&0x80)
    {
        i3=0xff;
    }
    else
    {
        i3=0;
    }
    ll=i3;
    ll<<=8;
    ll+=i2;
    ll<<=8;
    ll+=i1;
    ll<<=8;
    ll+=i0;
    if(ll>9999999||ll<-9999999)
    {
        return 0;
    }
    if(flag)
    {
        return ll;
    }
    else
    {
        return ll;
    }
}
void XYPosition()   /*设置X,Y轴,Z轴的位置*/
{
    unsigned long far *pi;
    unsigned long lt;
    register int i;
    ox=XPosi(0);
    oy=YPosi(0);
    oz=ZPosi(0);
    position_x=ox+offset_x;
    position_y=oy+offset_y;
    position_z=offset_z+oz;
    if(F12_flag>=1&&F12_flag<10)
    {
        pi=MK_FP(0x40,0x6c);
        lt=(*pi)/18.2;
        if(offset_t<lt-2)
        {
            return;
        }
        if(offset_t>lt)
        {
            offset_t=lt;
        }
        if(offset_t<lt)
        {
            position_t+=lt-offset_t;
            position_tt+=lt-offset_t;
            offset_t=lt;
        }
    }
}
unsigned char Current(void)     /*读取电流值*/
{
    int a,i,b;
    if(!OCSF)
    {
        return 0;
    }
    outb(OSC,0x02);
    a=inb(OSC+1);
    switch(SGY)
    {
    case 0:
        i=80;
        break;
    case 1:
        i=120;
        break;
    case 2:
        i=160;
        break;
    case 3:
        i=200;
        break;
    case 4:
        i=120;
        break;
    case 5:
        i=160;
        break;
    case 6:
        i=200;
        break;
    case 7:
        i=120;
        break;
    case 8:
        i=160;
        break;
    case 9:
        i=200;
        break;
    }
    b=i*MX;
    b+=20;
    if(a>=b)
    {
        return 0;
    }
    i=(b-a)*SDL/b;
    return i;
}
unsigned char Voltage(void)     /*读取电压值*/
{
    unsigned char a;
    outb(OSC,0x02);
    a=inb(OSC+1);
    return a;
}
long GetInput(void)
{
    long l;
    long i;
    if(!MaxL)
    {
        switch(Inputstr[0])
        {
        case ' ':
            KeyN=0x7fffffff;
            break;
        case '+':
            KeyN=0;
            break;
        case '-':
            KeyN=1;
            break;
        }
        return KeyN;
    }
    l=atoi(Inputstr+1);
    if(MaxL>3)
    {
        l*=1000;
        i=atoi(Inputstr1+1);
        if(!Inputstr1[1])
        {
            i=0;
        }
        else if(!Inputstr1[2])
        {
            i*=100;
        }
        else if(!Inputstr1[3])
        {
            i*=10;
        }
        l+=i;
    }
    if(Inputstr[0]=='-')
    {
        l=-l;
    }
    KeyN=0x7fffffff;
    if(!Inputstr[1])
    {
        l=KeyN;
    }
    Inputstr[1]=0;
    Inputstr1[1]=0;
    if(l>=MinN&&l<=MaxN)
    {
        KeyN=l;
    }
    return l;
}
void ShowInput(char value)
{
    int i;
    int j;
    static int k=0;
    char a[]="\0\0\0\0";
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    int x, y;
    char flag;
    if(MaxL==-1)
    {
        return;
    }
    if(MaxL>3)
    {
        j=MaxL-3;
    }
    else
    {
        j=MaxL;
    }
    a[0]=value;
    if(!MaxL&&Inputstr[0]!='-')
    {
        Inputstr[0]='+';
    }
    switch(value)
    {
    case '=':
    case '+':
        Inputstr[0]='+';
        break;
    case '-':
        Inputstr[0]='-';
        break;
    case 0:
        Inputstr[0]=' ';
        Inputstr[1]=0;
        Inputstr1[1]=0;
        k=0;
        MaxL=-1;
        KeyN=0x7fffffff;
        if(F[6]!=3)
        {
            Dispebar(CX1-84-73,CY0+17,CX1-8-73,CY1-1,7,8,0+8,Inputstr);
        }
        else
        {
            Dispebar(F7X+150,F7Y+5+18,F7X+290,F7Y+5+18+17,0xc,3,1,"");
        }
        return;
    case '.':
        if(!k&&MaxL>3)
        {
            a[0]='0';
            if(strlen(Inputstr)==1)
            {
                strcat(Inputstr,a);
            }
            k=1;
        }
        break;
    default:
        if(value<'0'||value>'9')
        {
            if(!k)
            {
                i=strlen(Inputstr);
                if(i>=2)
                {
                    Inputstr[--i]=0;
                }
            }
            else
            {
                i=strlen(Inputstr1);
                if(i>1)
                {
                    Inputstr1[i-1]=0;
                }
                if(i==1)
                {
                    k=0;
                }
            }
            break;
            /*return;*/
        }
        if(!k)
        {
            strcat(Inputstr,a);
            if(strlen(Inputstr+1)>j)
            {
                strcpy(Inputstr+1,Inputstr+2);
            }
        }
        else
        {
            strcat(Inputstr1,a);
            if(strlen(Inputstr1)>4)
            {
                strcpy(Inputstr1+1,Inputstr1+2);
            }
        }
    }
    if(P[13])
    {
        x=12;
        y=Y7Y-2;
        if(flag)
        {
            l<<=16;
        }
        x*=8;
        y*=LNYL;
        y+=8;
        Dispebar(F9X+70+x+2,F9Y+45+y,F9X+70+x+38+2+42,F9Y+41+y+18,6,1,0+8," ");
        Dispebar(F9X+70+x+2,F9Y+45+y,F9X+70+x+48+2,F9Y+41+y+18,6,1,0+8,Inputstr);
        if(k)
        {
            Dispcbar(F9X+70+x+33+2+7,F9Y+45+y,F9X+70+x+33+2+42,F9Y+41+y+18,6,1,0,Inputstr1);
        }
        /*Listbar3d(F9X+70+x-1,F9Y+43+y,F9X+100+x+8*3+30,F9Y+43+y+18,(long)l,1,0,1,Inputstr);
        if(k)Listbar3d(F9X+70+x-1,F9Y+43+y,F9X+100+x+8*3+30,F9Y+43+y+18,(long)l,1,0,1,Inputstr1);*/
    }
    else if(F[6]!=3)
    {
        Dispebar(CX1-84-74,CY0+17,CX1-44+8-74,CY1-1,7,8,0+8,Inputstr);
        if(k)
        {
            Dispcbar(CX1-43-74,CY0+17,CX1-8-74,CY1-1,7,8,0,Inputstr1);
        }
    }
    else
    {
        Dispebar(F7X+150,F7Y+5+18,F7X+250+8,F7Y+5+18+17,0xc,3,1+8,Inputstr);
        if(k)
        {
            Dispcbar(F7X+250,F7Y+5+18,F7X+290,F7Y+5+18+17,0xc,3,1,Inputstr1);
        }
    }
}
void ShowTB(char value)
{
    int i;
    char *TBstr[0x10]= {"    ","Z+ ","Z- ","Y+ ","Y- ","X+ ","X- ","火警","油位","短路","积炭","归零","limit!","油泵"};
    setfillstyle(1,BKCOLORC);
    i=value&0xf;
    if(!i)
    {
        bar(CX0+1,CY0+18,CX1-75,CY0+32+1);
        return;
    }
    if(value>0xf)
    {
        value=0;
    }
    if(i>6)
        if(i==0xd)
        {
            Dispcbar(CX0+20+203,CY0+18+1,CX0+20+203+34,CY0+32+1,COLORC,BKCOLORC,1,TBstr[value]);
        }
        else
        {
            Dispcbar(CX0+20+(i-6)*38+203,CY0+18+1,CX0+20+(i-6)*38+203+34,CY0+32+1,COLORC,BKCOLORC,1,TBstr[value]);
        }
    else
    {
        Dispcbar(CX0+20+(i-1)*25,CY0+18+1,CX0+20+(i-1)*25+24,CY0+32+1,COLORC,BKCOLORC,1,TBstr[value]);
        if(value)
        {
            value=0xc;
        }
        Dispcbar(CX0+20+203-58,CY0+18+1,CX0+20+203+34-58+10,CY0+32+1,COLORC,BKCOLORC,1,TBstr[value]);
    }
}
void Ck8255(void)
{
    static int fires=0,fires1=4,fires2=4,fires3=5,fires4=0,fires6=0,fires7=4,fires8=5,fires9=0,fires10=0;
    unsigned char a,b,c,d;
#if TESTING
    return;
#endif
    outb(OSC,0x01);
    a=inb(OSC+3);
    outb(Address_Z,0x01);
    b=inb(Address_Z+3);
    outb(ICYaddr,0x01);
    d=inb(ICYaddr+3);
    outb(ICXaddr,0x01);
    c=inb(ICXaddr+3);
    if(Dis_flag&&!(a&0x80)&&P[4])
    {
        /*火警*/
        if(fires>5)
        {
            SoundTime=SDST;
            Ck8255_flag|=0x80;
            FireFlag=1;
            ShowTB(0x7);
            if(Dis_flag)
            {
                F11();
                Delay1(0x1000);
                return;
            }
        }
        else
        {
            fires++;
        }
    }
    else
    {
        if(Ck8255_flag&0x80)
        {
            Ck8255_flag&=0x7f;
        }
        fires=0;
    }
    if(!(a&0x02))
    {
        /*油泵*/
        if(fires6>5)
        {
            SoundTime=SDST;
            Ck8256_flag|=0x80;
            Pum_flag=1;
            if(Pump_flag)
            {
                Close_Pump();
            }
            Pumn_flag=1;
            ShowTB(0xd);
            if(Dis_flag)
            {
                F11();
                Delay1(0x1000);
                return;
            }
        }
        else
        {
            fires6++;
        }
    }
    else
    {
        if(Ck8256_flag&0x80)
        {
            Ck8256_flag&=0x7f;
        }
        Pumn_flag=0;
        fires6=0;
    }
    if(!(b&0x80))
    {
        /*上限*/
        if(fires1>5)
        {
            if(UP_K)
            {
                UP_K=0;
                Stop_z(0);
            }
            SoundTime=TBST;
            if(Dis_flag)
            {
                F11();
                SoundTime=SDST;
            }
            if(F[4]==5)
            {
                F5(5);
            }
            Ck8255_flag|=0x10;
            ShowTB(0x1);
            fires1=0;
        }
        else
        {
            fires1++;
        }
    }
    else
    {
        if(Ck8255_flag&0x10)
        {
            ShowTB(0x11);
            Ck8255_flag&=0xef;
        }
        fires1=4;
    }
    if(!(b&0x40))
    {
        /*下限*/
        if(fires2>5)
        {
            if(DN_K)
            {
                DN_K=0;
                Stop_z(0);
            }
            SoundTime=TBST;
            if(Dis_flag)
            {
                F11();
                SoundTime=SDST;
            }
            if(F[4]==6)
            {
                F5(6);
            }
            Ck8255_flag|=0x20;
            ShowTB(0x2);
            fires2=0;
        }
        else
        {
            fires2++;
        }
    }
    else
    {
        if(Ck8255_flag&0x20)
        {
            ShowTB(0x12);
            Ck8255_flag&=0xdf;
        }
        fires2=4;
    }
    if(!(c&0x40))
    {
        /*左限X+*/
        if(fires7>5)
        {
            if(LF_K)
            {
                LF_K=0;
                Stop_x(0);
            }
            SoundTime=TBST;
            if(Dis_flag)
            {
                F11();
                SoundTime=SDST;
            }
            if(F[4]==5)
            {
                F5(5);
            }
            Ck8255_flag|=0x02;
            ShowTB(0x5);
            fires7=0;
        }
        else
        {
            fires7++;
        }
    }
    else
    {
        if(Ck8255_flag&0x02)
        {
            ShowTB(0x15);
            Ck8255_flag&=0xfd;
        }
        fires7=4;
    }
    if(!(c&0x80))
    {
        /*右限X-*/
        if(fires8>5)
        {
            if(RT_K)
            {
                RT_K=0;
                Stop_x(0);
            }
            SoundTime=TBST;
            if(Dis_flag)
            {
                F11();
                SoundTime=SDST;
            }
            if(F[4]==6)
            {
                F5(6);
            }
            Ck8255_flag|=0x01;
            ShowTB(0x6);
            fires8=0;
        }
        else
        {
            fires8++;
        }
    }
    else
    {
        if(Ck8255_flag&0x01)
        {
            ShowTB(0x16);
            Ck8255_flag&=0xfe;
        }
        fires8=4;
    }
    if(!(d&0x40))
    {
        /*前限Y+*/
        if(fires9>5)
        {
            if(FR_K)
            {
                FR_K=0;
                Stop_y(0);
            }
            SoundTime=TBST;
            if(Dis_flag)
            {
                F11();
                SoundTime=SDST;
            }
            if(F[4]==5)
            {
                F5(5);
            }
            Ck8255_flag|=0x08;
            ShowTB(0x3);
            fires9=0;
        }
        else
        {
            fires9++;
        }
    }
    else
    {
        if(Ck8255_flag&0x08)
        {
            ShowTB(0x13);
            Ck8255_flag&=0xf7;
        }
        fires9=4;
    }
    if(!(d&0x80))
    {
        /*后限Y-*/
        if(fires10>5)
        {
            if(BK_K)
            {
                BK_K=0;
                Stop_y(0);
            }
            SoundTime=TBST;
            if(Dis_flag)
            {
                F11();
                SoundTime=SDST;
            }
            if(F[4]==6)
            {
                F5(6);
            }
            Ck8255_flag|=0x04;
            ShowTB(0x4);
            fires10=0;
        }
        else
        {
            fires10++;
        }
    }
    else
    {
        if(Ck8255_flag&0x04)
        {
            ShowTB(0x14);
            Ck8255_flag&=0xfb;
        }
        fires10=4;
    }
    if((!F[4]&&!Dis_flag)&&Voltage()<=1&&!ZeroFlag) /*短路*/
    {
        if(fires3>=5)
        {
            if(!K[3])
            {
                if(DN_K&&!K[8]||UP_K&&K[8])
                {
                    V=0;
                    Stop_z(0);
                    DN_K=0;
                    UP_K=0;
                }
                if(RT_K&&!K[8]||LF_K&&K[8])
                {
                    V=0;
                    Stop_x(0);
                    RT_K=0;
                    LF_K=0;
                }
                if(FR_K&&!K[8]||BK_K&&K[8])
                {
                    V=0;
                    Stop_y(0);
                    FR_K=0;
                    BK_K=0;
                }
                SoundTime|=0x4000;
                OpenSound();
            }
            if(lock_z_stop<10)
            {
                lock_z_stop=1;
            }
            if(ORgin&0x80)
            {
                Stop_z(0);
                ORgin=0;
                ShowMess(47);
            }
            V0ING=1;
            ShowFL(1);
            ShowTB(0x9);
        }
        else
        {
            fires3++;
        }
    }
    else
    {
        if(V0ING)
        {
            if(SoundTime>=0x4000)
            {
                SoundTime&=0x3fff;
            }
            else
            {
                SoundTime=0;
            }
            if(!SoundTime)
            {
                CloseSound();
            }
            ShowTB(0x19);
            ShowFL(0);
            V0ING=0;
        }
        lock_z_stop=0;
        fires3=5;
    }
    if(Dis_flag&&K[4]&&OilSwitch())
    {
        if(fires4>5)
        {
            ShowTB(0x8);
            Ck8255_flag|=0x40;
        }
        else
        {
            fires4++;
        }
    }
    else
    {
        if(Ck8255_flag&0x40)
        {
            ShowTB(0x18);
            Ck8255_flag&=0xbf;
        }
    }
}
/*9.9变压器*/
void Open_Elect(void) /*放电，开大变压器*/
{
    if(Table.LV[Dis_lines]>1)
    {
        outb(IO+4,IO4&=0xfb);                /* 低压 */
        outb(IO+4,IO4|=0x08);
    }
    else
    {
        outb(IO+4,IO4&=0xf7);
        outb(IO+4,IO4|=0x04);
    }
    if(((Table.Dianliu[Dis_lines]>7&&Table.LV[Dis_lines]<2)||
            (Table.Dianliu[Dis_lines]>3&&Table.LV[Dis_lines]>1))&&Dis_flag)
    {
        outb(IO+4,IO4|=0x80);/*>4A开风扇*/
        FS=0x1000;
    }
}
void Close_Elect(void)
{
    outb(IO+4,IO4&=0xf3);
    FS=1092;  /*关风扇*/
}
void Open_OCSF(void)
{
    int i;
    OCSF=1;
    outb(OSC,0x01);
    outb(OSC+2,OTPS1|=0x80);
}
void Close_OCSF(void)
{
    int i;
    OCSF=0;
    outb(OSC,0x01);
    outb(OSC+2,OTPS1&=0x7f);
}
void Dispcbar(int x,int y,int x1,int y1,int color,int bkcolor,int stx,char *str)
{
    setfillstyle(1,bkcolor);
    bar(x,y,x1,y1);
    List16x16M(x+stx,(y+y1)/2,color,str);
}
void Dispebar(int x,int y,int x1,int y1,int color,int bkcolor,int stx,char *str)
{
    setfillstyle(1,bkcolor);
    bar(x,y,x1,y1);
    Disp8x16(x1-stx,(y+y1)/2,2,color,str);
}
void Dispbar3d(int x,int y,int x1,int y1,int color,int flag,int stx,int w,char *str)
{
    int colors;
    int i,j=0;
    colors=color&0xf;
    if(str[0]==0)
    {
        setfillstyle(1,color>>8);
        bar(x,y,x1,y1);
        return;
    }
    if(stx>=0x100)
    {
        j=stx>>8;
        stx&=0xff;
    }
    j<<=1;
    j++;
    if(flag)
    {
        setcolor(0x8);
        for(i=0; i<w; i++)
        {
            line(x,y+i,x1-i,y+i);
            line(x+i,y,x+i,y1-i);
        }
        setcolor(0xf);
        for(i=0; i<w; i++)
        {
            line(x+i,y1-i,x1,y1-i);
            line(x1-i,y+i,x1-i,y1);
        }
        setfillstyle(1,colors);
        bar(x+w,y+w,x1-w,y1-w);
        List16x16M(x+3+stx,(y*j+y1)/(1+j),0xf,str);
    }
    else
    {
        setcolor(0xf);
        for(i=0; i<w; i++)
        {
            line(x,y+i,x1-i,y+i);
            line(x+i,y,x+i,y1-i);
        }
        setcolor(0x8);
        for(i=0; i<w; i++)
        {
            line(x+i,y1-i,x1,y1-i);
            line(x1-i,y+i,x1-i,y1);
        }
        setfillstyle(1,0x7);
        bar(x+w,y+w,x1-w,y1-w);
        List16x16M(x+3+stx,(y*j+y1)/(1+j),(color>>4)&0xf,str);
    }
}
char DispF6(int flag)
{
    int j;
    if(flag==0)
    {
        getimage(F9X-25,F9Y,F9X+259+25,F9Y+170+7,BMP);
        setcolor(0);
        rectangle(F9X+1-25,F9Y+1,F9X+257+25,F9Y+168+7);
        rectangle(F9X+2-25,F9Y+2,F9X+258+25,F9Y+169+7);
        setcolor(0xb);
        rectangle(F9X+0-25,F9Y,F9X+259+25,F9Y+170+7);
        rectangle(F9X+3-25,F9Y+3,F9X+257+25,F9Y+168+7);
        setfillstyle(1,0x7);
        bar(F9X+4-25,F9Y+4,F9X+256+25,F9Y+167+7);
        ShowF6(0);
    }
    if(flag==1)
    {
        Write_Nodes(0);
        setviewport(0,0,639,479,0);
        putimage(F9X-25,F9Y,BMP,0);
        ShowTable(0);
        ShowMess(0);
    }
    if(flag>=2)
    {
        Read_Nodes(0);
        setviewport(0,0,639,479,0);
        putimage(F9X-25,F9Y,BMP,0);
        ShowTable(0);
        ShowMess(0);
    }
    XX=0;
    YY=0;
    if(flag>2)
    {
        ShowTable(0);
    }
    return 0;
}

void ShowF6(int lines)
{
    char str[12],str1[6]=".000";
    int color=7;
    register j,k,i,z;
    long l=0;
    int k0=0;
    if(!lines)
    {
        Dispcbar(F7X+95,F7Y-12+20*0,F7X+185,F7Y+20*0+4,0,color,1,"位置设定");
        Dispcbar(F7X+5-25,F7Y+5+20*0,F7X+255+25,F7Y+5+20*0+5,0,0x1,1,"");
        Dispcbar(F7X+35-25,F7Y-6+20*1,F7X+130,F7Y+5+20*1+10,0,color,1,"X 位置");
        Dispcbar(F7X+130-25,F7Y-6+20*1,F7X+225,F7Y+5+20*1+10,0,color,1,"Y 位置");
        Dispcbar(F7X+225-25,F7Y-6+20*1,F7X+275,F7Y+5+20*1+10,0,color,1,"Z 升高");
        F6line=0;
    }
    if(!Muti_flag)
    {
        switch(XX)
        {
        case 0:
            MaxL=7;
            MinN=-9999999;
            MaxN=9999999;
            break;
        case 1:
            MaxL=7;
            MinN=-9999999;
            MaxN=9999999;
            break;
        case 2:
            MaxL=7;
            MinN=-9999999;
            MaxN=9999999;
            break;
        default:
            MaxL=-1;
            MaxN=0;
            MinN=0;
            break;
        }
    }
    else
    {
        MaxL=-1;
        MaxN=0;
        MinN=0;
    }
    str[0]=0;
    str[1]=0;
    if(!lines||lines<F6line+1||lines>F6line+4)
    {
        if(lines<F6line+1)
        {
            F6line=lines-1;
            if(F6line<0)
            {
                F6line=0;
            }
        }
        else if(lines>F6line+5)
        {
            F6line=lines-5;
            if(F6line>15)
            {
                F6line=15;
            }
        }
        k=0;
        z=5;
    }
    else
    {
        z=lines-F6line;
        k=lines-1-F6line;
    }
    if(lines>20)
    {
        return;
    }
    for(i=k; i<z; i++)
    {
        for(j=0; j<3; j++)
        {
            setfillstyle(1,8);
            color=0xf;
            if(j==XX&&i==YY-F6line)
            {
                setfillstyle(1,0xf);
                color=0x4;
            }
            if(1==Node.flag[i+F6line])
            {
                switch(j)
                {
                case    0:
                    l=Node.x_pos[i+F6line];
                    l*=DZC;
                    k0=l/1000;
                    if(!k0&&l<0)
                    {
                        str[0]='-';
                        itoa(l/1000,str+1,10);
                    }
                    else
                    {
                        itoa(l/1000,str,10);
                    }
                    str1[1]=48;
                    str1[2]=48;
                    str1[3]=48;
                    k0=l%1000;
                    k0=abs(k0);
                    str1[1]+=k0/100;
                    k0%=100;
                    str1[2]+=k0/10;
                    k0%=10;
                    str1[3]+=k0;
                    strcat(str,str1);
                    break;
                case    1:
                    l=Node.y_pos[i+F6line];
                    l*=DZC;
                    k0=l/1000;
                    if(!k0&&l<0)
                    {
                        str[0]='-';
                        itoa(l/1000,str+1,10);
                    }
                    else
                    {
                        itoa(l/1000,str,10);
                    }
                    str1[1]=48;
                    str1[2]=48;
                    str1[3]=48;
                    k0=l%1000;
                    k0=abs(k0);
                    str1[1]+=k0/100;
                    k0%=100;
                    str1[2]+=k0/10;
                    k0%=10;
                    str1[3]+=k0;
                    strcat(str,str1);
                    break;
                case    2:
                    l=Node.z_pos[i+F6line];
                    l*=DZC;
                    k0=l/1000;
                    if(!k0&&l<0)
                    {
                        str[0]='-';
                        itoa(l/1000,str+1,10);
                    }
                    else
                    {
                        itoa(l/1000,str,10);
                    }
                    str1[1]=48;
                    str1[2]=48;
                    str1[3]=48;
                    k0=l%1000;
                    k0=abs(k0);
                    str1[1]+=k0/100;
                    k0%=100;
                    str1[2]+=k0/10;
                    k0%=10;
                    str1[3]+=k0;
                    strcat(str,str1);
                    break;
                }
            }
            else
            {
                strcpy(str,KeyStr(0x0f,0x00));
            }
            bar(F7X+(j*88),F7Y+(i+2)*20,F7X+(j*88)+85,F7Y+(i+3)*20-3);
            Disp8x16(F7X+(j*88)+80,F7Y+(i+2)*20+8,2,color,str);
        }
        itoa(i+F6line,str,10);
        if(i==Dis_pos-F6line&&1==Node.flag[i+F6line]&&P[12]==1)
        {
            Dispcbar(F7X+7-25,F7Y+(i+2)*20,F7X+7+20-25,F7Y+(i+3)*20-3,1,7,1,str);
        }
        else
        {
            Dispcbar(F7X+7-25,F7Y+(i+2)*20,F7X+7+20-25,F7Y+(i+3)*20-3,0,7,1,str);
        }
    }
}
char DispF7(int flag)
{
    int j;
    if(flag==0)
    {
        getimage(F9X,F9Y,F9X+259,F9Y+170+7,BMP);
        setcolor(0);
        rectangle(F9X+1,F9Y+1,F9X+257,F9Y+168+7);
        rectangle(F9X+2,F9Y+2,F9X+258,F9Y+169+7);
        setcolor(0xb);
        rectangle(F9X+0,F9Y,F9X+259,F9Y+170+7);
        rectangle(F9X+3,F9Y+3,F9X+257,F9Y+168+7);
        setfillstyle(1,0x7);
        bar(F9X+4,F9Y+4,F9X+256,F9Y+167+7);
        ListStrDeep(12,-1,1);
        ListStr8(12,0,0);
        ListStr9(12,1,0);
        ListStr10(12,2,0);
        ListStr11(12,3,0);
        ListStrCur(12,4,0);
        ListStrAcr(12,5,0);
        ShowF7(0);
    }
    if(flag==1)
    {
        if(lDeep!=0x7fffffff&&iCurrent!=0x7fff&&iAcreage!=0x7fff&&
                cEffect!=0x7f&&cMaterial!=0x7f&&cShape!=0x7f)
        {
            ProcessTable(lDeep,cMaterial,iCurrent,iAcreage,cEffect,cShape,cProcess,-1);
            for(j=0; j<15&&Table.Index[j]==1; j++)
            {
                ;
            }
            j--;
            Dis_start=0;
            if(j>=1)
            {
                Dis_end=j;
            }
            else
            {
                Dis_end=0;
            }
            Dis_lines=Dis_start;
            F[6]=1;
            flag+=2;
            ShowMess(0);
        }
        else
        {
            Dispcbar(F7X+150,F7Y+5+18,F7X+249,F7Y+5+18+16,0,7,1,"无效操作");
            return 1;
        }
        ShowTable(0);
    }
    if(flag>=2)
    {
        setviewport(0,0,639,479,0);
        putimage(F9X,F9Y,BMP,0);
        ShowTable(0);
        ShowMess(0);
    }
    if(flag>2)
    {
        ShowTable(0);
    }
    return 0;
}
void ShowF7(int lines)
{
    int current[3][3]= {{95,63,31},{191,63,31},{31,31,31}};
    int ia;
    char str[12],str1[6]=".000";
    int color=7;
    register j;
    if(!lines)
    {
        Dispcbar(F7X+85,F7Y-12+20*0,F7X+185,F7Y+20*0+4,0,color,1,"自动生成表");
        Dispcbar(F7X+5,F7Y+5+20*0,F7X+255,F7Y+5+20*0+5,0,0x1,1,"");
        Dispcbar(F7X+5,F7Y-6+20*1,F7X+105,F7Y+5+20*1+10,0,color,1,"深度 Deep:   ");
        Dispcbar(F7X+5,F7Y-6+20*6,F7X+105,F7Y+5+20*6+10,0,color,1,"电流 Current:");
        Dispcbar(F7X+5,F7Y-6+20*7,F7X+105,F7Y+5+20*7+10,0,color,1,"面积 Acreage:");
        Dispcbar(F7X+5,F7Y-6+20*2,F7X+105,F7Y+5+20*2+10,0,color,1,"材料 Material:");
        Dispcbar(F7X+5,F7Y-6+20*3,F7X+105,F7Y+5+20*3+10,0,color,1,"效果 Effect: ");
        Dispcbar(F7X+5,F7Y-6+20*4,F7X+105,F7Y+5+20*4+10,0,color,1,"形状 Shape: ");
        Dispcbar(F7X+5,F7Y-6+20*5,F7X+105,F7Y+5+20*5+10,0,color,1,"制程 Process: ");
        lines++;
    }
    switch(lines)
    {
    case 1:
        MaxL=7;
        MinN=-9999999;
        MaxN=9999999;
        break;
    case 6:
        MaxL=3;
        MinN=1;
        MaxN=current[cMaterial-1][cShape-1];
        break;
    case 7:
        MaxL=2;
        MinN=1;
        MaxN=99;
        break;
    default:
        MaxL=-1;
        MaxN=0;
        MinN=0;
        break;
        /*case 3:MaxL=1;MinN=1;MaxN=3;break;*/
        /*case 4:MaxL=1;MinN=1;MaxN=4;break;*/
        /*case 5:MaxL=1;MinN=1;MaxN=3;break;*/
        /*case 6:MaxL=1;MinN=1;MaxN=2;break;*/
    }
    str[0]=0;
    str[1]=0;
    ListStrDeep(12,-1,0);
    ListStr8(12,0,0);
    ListStr9(12,1,0);
    ListStr10(12,2,0);
    ListStr11(12,3,0);
    ListStrCur(12,4,0);
    ListStrAcr(12,5,0);
    switch(lines)
    {
    case 1:
        ListStrDeep(12,-1,1);
        ShowAuto(130);
        break;
    case 2:
        ListStr8(12,0,1);
        ShowAuto(133);
        break;
    case 3:
        ListStr9(12,1,1);
        ShowAuto(134);
        break;
    case 4:
        ListStr10(12,2,1);
        ShowAuto(135);
        break;
    case 5:
        ListStr11(12,3,1);
        ShowAuto(136);
        break;
    case 6:
        ListStrCur(12,4,1);
        ShowAuto(131);
        break;
    case 7:
        ListStrAcr(12,5,1);
        ShowAuto(132);
        break;
    }
    /*Dispebar(F7X+115,F7Y+5+18*(lines+1),F7X+250,F7Y+5+18*(lines+1)+16,0,7,1,str);
    setcolor(color);
    for(j=1;j<=6;j++)line(F7X+115,F7Y+5+18*(j+1)+17,F7X+250,F7Y+5+18*(j+1)+17);
    setcolor(1);
    line(F7X+115,F7Y+5+18*(lines+1)+17,F7X+250,F7Y+5+18*(lines+1)+17);*/
}
void DispF8(int flag)
{
    if(flag==0)
    {
        getimage(F9X,F9Y,F9X+259,F9Y+159,BMP);
        setcolor(0);
        rectangle(F9X+1,F9Y+1,F9X+257,F9Y+157);
        rectangle(F9X+2,F9Y+2,F9X+258,F9Y+158);
        setcolor(0xb);
        rectangle(F9X+0,F9Y+0,F9X+259,F9Y+159);
        rectangle(F9X+3,F9Y+3,F9X+257,F9Y+157);
        setfillstyle(1,0x7);
        bar(F9X+4,F9Y+4,F9X+256,F9Y+156);
    }
    if(flag==1)
    {
    }
    if(flag>=2)
    {
        setviewport(0,0,639,479,0);
        putimage(F9X,F9Y,BMP,0);
    }
    if(flag>2)
    {
        ShowTable(0);
    }
}
/*修改u（单位）,计数方向*/
char DispF9(int flag)
{
    if(flag==0)
    {
        getimage(F9X-20,F9Y,F9X+259+20,F9Y+179,BMP);
        setcolor(0);
        rectangle(F9X+1-20,F9Y+1,F9X+257+20,F9Y+177);
        rectangle(F9X+2-20,F9Y+2,F9X+258+20,F9Y+178);
        setcolor(0xb);
        rectangle(F9X+0-20,F9Y+0,F9X+259+20,F9Y+179);
        rectangle(F9X+3-20,F9Y+3,F9X+257+20,F9Y+177);
        setfillstyle(1,0x7);
        bar(F9X+4-20,F9Y+4,F9X+256+20,F9Y+176);
        ListStr1(6,0,1);
        ListStr2(6,1,0);
        ListStr3(6,2,0);
        ListStr4(6,3,0);
        ListStr5(6,4,0);
        ListStr6(6,5,0);
        ListStrXP(17,-1,0);
        ListStrYP(17,0,0);
        ListStrZP(17,1,0);
        ShowMess(40);
        ShowF8(0);
        flag=1;
        setline=0;
    }
    if(flag==1)
    {
    }
    if(flag>=2)
    {
        setviewport(0,0,639,479,0);
        putimage(F9X-20,F9Y,BMP,0);
    }
    if(flag>2)
    {
        ShowTable(0);
    }
    return 0;
}
void ShowF8(int lines)
{
    int ia;
    char str[12];
    int color=7;
    register j;
    if(!lines)
    {
        Dispcbar(F9X+5+40,F9Y+5+20*0,F9X+100,F9Y+5+18*0+16,0,color,1,"    系统参数设置");
        Dispcbar(F9X+5-20,   F9Y+5+20*1,F9X+255+20,F9Y+5+18*1+5,0,0x1,1,"");
        Dispcbar(F9X+5-20,F9Y-10+21*2,F9X+40,F9Y+5+18*2+20,0,color,1,"  分辨率:");
        Dispcbar(F9X+5-20,F9Y-10+21*3,F9X+40,F9Y+5+18*3+20,0,color,1," X轴方向:");
        Dispcbar(F9X+5-20,F9Y-10+21*4,F9X+40,F9Y+5+18*4+20,0,color,1," Y轴方向:");
        Dispcbar(F9X+5-20,F9Y-10+21*5,F9X+40,F9Y+5+18*5+20,0,color,1," Z轴方向:");
        Dispcbar(F9X+5-20,F9Y-10+21*6,F9X+40,F9Y+5+18*6+20,0,color,1,"原点清零:");
        Dispcbar(F9X+5-20,F9Y-10+21*7,F9X+40,F9Y+5+18*7+20,0,color,1,"  电  流:");
        Dispcbar(F9X+5-20+150,F9Y-10+21*2,F9X+40+150,F9Y+5+18*2+20,0,color,1,"X增益:");
        Dispcbar(F9X+5-20+150,F9Y-10+21*3,F9X+40+150,F9Y+5+18*3+20,0,color,1,"Y增益:");
        Dispcbar(F9X+5-20+150,F9Y-10+21*4,F9X+40+150,F9Y+5+18*4+20,0,color,1,"Z增益:");
    }
}
/*修改运动顺序*/
char DispF10(int flag)
{
    if(flag==0)
    {
        getimage(F9X,F9Y,F9X+259,F9Y+159,BMP);
        setcolor(0);
        rectangle(F9X+1,F9Y+1,F9X+257,F9Y+157);
        rectangle(F9X+2,F9Y+2,F9X+258,F9Y+158);
        setcolor(0xb);
        rectangle(F9X+0,F9Y+0,F9X+259,F9Y+159);
        rectangle(F9X+3,F9Y+3,F9X+257,F9Y+157);
        setfillstyle(1,0x7);
        bar(F9X+4,F9Y+4,F9X+256,F9Y+156);
        ListStr7(6,0,1,1);
        ListStr7(6,1,0,2);
        ListStr7(6,2,0,3);
        ShowF9(0);
        flag=1;
        setline1=0;
    }
    if(flag==1)
    {
    }
    if(flag>=2)
    {
        setviewport(0,0,639,479,0);
        putimage(F9X,F9Y,BMP,0);
    }
    if(flag>2)
    {
        ShowTable(0);
    }
    return 0;
}
void ShowF9(int lines)
{
    int ia;
    char str[12];
    int color=7;
    register j;
    if(!lines)
    {
        Dispcbar(F9X+5+40,F9Y+5+20*0,F9X+100,F9Y+5+18*0+16,0,color,1,"    运动顺序设置   ");
        Dispcbar(F9X+5,F9Y+5+20*1,F9X+255,F9Y+5+18*1+10,0,0x1,1,"");
        Dispcbar(F9X+5+40,F9Y+15+20*2,F9X+100,F9Y+15+18*2+16,0,color,1,"第一运动顺序：");
        Dispcbar(F9X+5+40,F9Y+25+20*3,F9X+100,F9Y+25+18*3+16,0,color,1,"第  运动顺序：");
        Dispcbar(F9X+5+40,F9Y+35+20*4,F9X+100,F9Y+35+18*4+16,0,color,1,"第  运动顺序：");
    }
}
char *StrLN1(char flag)
{
    char *str[__LNSTR1__]= {"1","5"};
    return str[flag];
}
char *StrLN2(char flag)
{
    char *str[__LNSTR2__]= {"+","-"};
    return str[flag];
}
char *StrLN3(char flag)
{
    char *str[__LNSTR3__]= {"+","-"};
    return str[flag];
}
char *StrLN4(char flag)
{
    char *str[__LNSTR4__]= {"+","-"};
    return str[flag];
}
char *StrLN5(char flag)
{
    char *str[__LNSTR5__]= {"0","1"};
    return str[flag];
}
char *StrLN6(char flag)
{
    char *str[__LNSTR6__]= {"35","75"};
    return str[flag];
}
char *StrLN7(char flag)
{
    char *str[__LNSTR7__]= {"X+","X-","Y+","Y-","Z+","Z-"," "};
    return str[flag];
}
char *StrLN8(char flag)
{
    char *str[__LNSTR8__]= {"Cu-St","Gr-St","Cu-Wc"};
    return str[flag];
}
char *StrLN9(char flag)
{
    char *str[__LNSTR9__]= {"1","2","3","4"};
    return str[flag];
}
char *StrLN10(char flag)
{
    char *str[__LNSTR10__]= {"平面","深孔","清角"};
    return str[flag];
}
char *StrLN11(char flag)
{
    char *str[__LNSTR11__]= {"粗","正常"};
    return str[flag];
}
void ListStr1(int x,int y,char flag)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    strcpy(DispStr,StrLN1(sl0));
    strcat(DispStr,"  ");
    Listbar3d(F9X+100+x-1-80,F9Y+35+y,F9X+100+x+8*3+30-80,F9Y+35+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12-80,F9Y+35+y+1,F9X+100+x+8*3+30-80,F9Y+35+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr2(int x,int y,char flag)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    strcpy(DispStr,StrLN2(sl1));
    strcat(DispStr,"  ");
    Listbar3d(F9X+100+x-1-80,F9Y+35+y,F9X+100+x+8*3+30-80,F9Y+35+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12-80,F9Y+35+y+1,F9X+100+x+8*3+30-80,F9Y+35+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr3(int x,int y,char flag)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    strcpy(DispStr,StrLN3(sl2));
    strcat(DispStr,"  ");
    Listbar3d(F9X+100+x-1-80,F9Y+35+y,F9X+100+x+8*3+30-80,F9Y+35+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12-80,F9Y+35+y+1,F9X+100+x+8*3+30-80,F9Y+35+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr4(int x,int y,char flag)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    strcpy(DispStr,StrLN4(sl3));
    strcat(DispStr,"  ");
    Listbar3d(F9X+100+x-1-80,F9Y+35+y,F9X+100+x+8*3+30-80,F9Y+35+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12-80,F9Y+35+y+1,F9X+100+x+8*3+30-80,F9Y+35+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr5(int x,int y,char flag)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    strcpy(DispStr,StrLN5(sl4));
    strcat(DispStr,"  ");
    Listbar3d(F9X+100+x-1-80,F9Y+35+y,F9X+100+x+8*3+30-80,F9Y+35+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12-80,F9Y+35+y+1,F9X+100+x+8*3+30-80,F9Y+35+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr6(int x,int y,char flag)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    strcpy(DispStr,StrLN6(sl5));
    strcat(DispStr,"  ");
    Listbar3d(F9X+100+x-1-80,F9Y+35+y,F9X+100+x+8*3+30-80,F9Y+35+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12-80,F9Y+35+y+1,F9X+100+x+8*3+30-80,F9Y+35+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr7(int x,int y,char flag,char line)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=30;
    y+=10;
    if(line==1)
    {
        strcpy(DispStr,StrLN7(sl7));
    }
    else if(line==2)
    {
        strcpy(DispStr,StrLN7(sl8));
    }
    else
    {
        strcpy(DispStr,StrLN7(sl9));
    }
    strcat(DispStr,"  ");
    Listbar3d(F9X+100+x-1,F9Y+35+y,F9X+100+x+8*3+30,F9Y+35+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12,F9Y+35+y+1,F9X+100+x+8*3+30,F9Y+35+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr8(int x,int y,char flag)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    y+=8;
    if(cMaterial!=0x7f)
    {
        strcpy(DispStr,StrLN8(cMaterial-1));
    }
    else
    {
        strcpy(DispStr,StrLN8(0));
    }
    strcat(DispStr,"  ");
    Listbar3d(F9X+70+x-1,F9Y+43+y,F9X+100+x+8*3+30,F9Y+43+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12,F9Y+43+y+1,F9X+100+x+8*3+30,F9Y+43+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr9(int x,int y,char flag)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    y+=8;
    if(cEffect!=0x7f)
    {
        strcpy(DispStr,StrLN9(cEffect-1));
    }
    else
    {
        strcpy(DispStr,StrLN9(0));
    }
    strcat(DispStr,"  ");
    Listbar3d(F9X+70+x-1,F9Y+43+y,F9X+100+x+8*3+30,F9Y+43+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12,F9Y+43+y+1,F9X+100+x+8*3+30,F9Y+43+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr10(int x,int y,char flag)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    y+=8;
    if(cShape!=0x7f)
    {
        strcpy(DispStr,StrLN10(cShape-1));
    }
    else
    {
        strcpy(DispStr,StrLN10(0));
    }
    strcat(DispStr,"  ");
    Listbar3d(F9X+70+x-1,F9Y+43+y,F9X+100+x+8*3+30,F9Y+43+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12,F9Y+43+y+1,F9X+100+x+8*3+30,F9Y+43+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStr11(int x,int y,char flag)
{
    char DispStr[10];
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    y+=8;
    if(cProcess!=0x7f)
    {
        strcpy(DispStr,StrLN11(cProcess-1));
    }
    else
    {
        strcpy(DispStr,StrLN11(0));
    }
    strcat(DispStr,"  ");
    Listbar3d(F9X+70+x-1,F9Y+43+y,F9X+100+x+8*3+30,F9Y+43+y+18,(long)l,1,0,1,DispStr);
    Listbar3d(F9X+100+x+8*3+12,F9Y+43+y+1,F9X+100+x+8*3+30,F9Y+43+y+17,(long)(LNBC4<<8|LNCO4),0,0,1,"");
}
void ListStrDeep(int x,int y,char flag)
{
    char DispStr[10];
    char str[12],str1[6]=".000";
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    register j;
    str[0]=0;
    str[1]=0;
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    y+=8;
    if(lDeep!=0x7fffffff)
    {
        str[0]=' ';
        if(lDeep<0&&lDeep>-1000)
        {
            str[0]='-';
        }
        itoa(lDeep/1000,str+1,10);
        j=abs(lDeep%1000);
        str1[1]='0';
        str1[2]='0';
        str1[3]='0';
        str1[1]+=j/100;
        j%=100;
        str1[2]+=j/10;
        j%=10;
        str1[3]+=j;
        strcat(str,str1);
    }
    if(lDeep!=0x7fffffff)
    {
        strcpy(DispStr,str);
    }
    else
    {
        strcpy(DispStr,"");
    }
    strcat(DispStr,"  ");
    Listbar3d(F9X+70+x-1,F9Y+43+y,F9X+100+x+8*3+30,F9Y+43+y+18,(long)l,1,0,1,DispStr);
}
void ListStrCur(int x,int y,char flag)
{
    char DispStr[10];
    char str[12],str1[6]=".000";
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    register j;
    str[0]=0;
    str[1]=0;
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    y+=8;
    itoa(iCurrent,str,10);
    if(iCurrent!=0x7fff)
    {
        strcpy(DispStr,str);
    }
    else
    {
        strcpy(DispStr,"");
    }
    strcat(DispStr,"  ");
    Listbar3d(F9X+70+x-1,F9Y+43+y,F9X+100+x+8*3+30,F9Y+43+y+18,(long)l,1,0,1,DispStr);
}
void ListStrAcr(int x,int y,char flag)
{
    char DispStr[10];
    char str[12],str1[6]=".000";
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    register j;
    str[0]=0;
    str[1]=0;
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    y+=8;
    itoa(iAcreage,str,10);
    if(iAcreage!=0x7fff)
    {
        strcpy(DispStr,str);
    }
    else
    {
        strcpy(DispStr,"");
    }
    strcat(DispStr,"  ");
    Listbar3d(F9X+70+x-1,F9Y+43+y,F9X+100+x+8*3+30,F9Y+43+y+18,(long)l,1,0,1,DispStr);
}
void ListStrXP(int x,int y,char flag)
{
    char DispStr[10];
    char str[12],str1[6]=".000";
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    register j;
    str[0]=0;
    str[1]=0;
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    y+=8;
    itoa(x_Pid_kp,str,10);
    strcpy(DispStr,str);
    strcat(DispStr,"  ");
    Listbar3d(F9X+70+x-1,F9Y+47+y,F9X+100+x+8*3,F9Y+47+y+18,(long)l,1,0,1,DispStr);
}
void ListStrYP(int x,int y,char flag)
{
    char DispStr[10];
    char str[12],str1[6]=".000";
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    register j;
    str[0]=0;
    str[1]=0;
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    y+=8;
    itoa(y_Pid_kp,str,10);
    strcpy(DispStr,str);
    strcat(DispStr,"  ");
    Listbar3d(F9X+70+x-1,F9Y+47+y,F9X+100+x+8*3,F9Y+47+y+18,(long)l,1,0,1,DispStr);
}
void ListStrZP(int x,int y,char flag)
{
    char DispStr[10];
    char str[12],str1[6]=".000";
    long l=(long)(LNBC3<<24|LNCO3<<16|LNBC5<<8|LNCO5);
    register j;
    str[0]=0;
    str[1]=0;
    if(flag)
    {
        l<<=16;
    }
    x*=8;
    y*=LNYL;
    y+=8;
    itoa(z_Pid_kp,str,10);
    strcpy(DispStr,str);
    strcat(DispStr,"  ");
    Listbar3d(F9X+70+x-1,F9Y+47+y,F9X+100+x+8*3,F9Y+47+y+18,(long)l,1,0,1,DispStr);
}
void Listbar3d(int x,int y,int x1,int y1,long color,unsigned char flag,unsigned sty,unsigned char w,char *str1)
{
    int colors,bkcolors,color1;
    register i,j;
    signed char ya,yb;
    int xx,yy,ww,k;
    char str[0x80];
    strcpy(str,str1);
    if(flag&1)
    {
        color>>=0x10;
    }
    colors=color&0xff;
    bkcolors=color>>8;
    ya=sty&0xff;
    yb=sty>>8;
    ww=w&0xff;
    yy=(y1+y+1)/2;
    if(flag&1&&!(flag&4))
    {
        setcolor(0x8);
        for(i=0; i<ww; i++)
        {
            line(x,y+i,x1-i,y+i);
            line(x+i,y,x+i,y1-i);
        }
        setcolor(0xf);
        for(i=0; i<ww; i++)
        {
            line(x+i,y1-i,x1,y1-i);
            line(x1-i,y+i,x1-i,y1);
        }
        setfillstyle(1,bkcolors);
        bar(x+ww,y+ww,x1-ww,y1-ww);
    }
    else
    {
        setcolor(0xf);
        for(i=0; i<ww; i++)
        {
            line(x,y+i,x1-i,y+i);
            line(x+i,y,x+i,y1-i);
        }
        setcolor(0x8);
        for(i=0; i<ww; i++)
        {
            line(x+i,y1-i,x1,y1-i);
            line(x1-i,y+i,x1-i,y1);
        }
        setfillstyle(1,bkcolors);
        bar(x+ww,y+ww,x1-ww,y1-ww);
        if(flag&4)
        {
            return;
        }
    }
    if(flag&8)
    {
        for(j=0; str[j]&&str[j]!='\n'; j++)
        {
            ;
        }
        str[j]=0;
        yy-=8;
    }
    i=strlen(str)<<3;
    xx=(x1+x-i+1)/2;
    if(flag&0x80)
    {
        setcolor(0x17);
        line(x+ww+2,(y+y1)/2,x1-ww-2,(y+y1)/2);
        setcolor(0x1f);
        line(x+ww+2,1+(y+y1)/2,x1-ww-2,1+(y+y1)/2);
    }
    if(flag&0x2)
    {
        color1=0x1b;
    }
    else
    {
        color1=colors;
    }
    List16x16M(xx,yy+ya,color1,str);
    if(flag&8&&!(flag&0x40))
    {
        j++;
        i=strlen(str+j)<<3;
        xx=(x1+x-i)/2;
        yy+=16;
    }
    else
    {
        return;
    }
    if(flag&0x10)
    {
        if(flag&0x20)
        {
            color1=0x1b;
        }
        else
        {
            color1=colors;
        }
    }
    List16x16M(xx,yy+yb,color1,str+j);
}
