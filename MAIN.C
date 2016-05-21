#include "znc.h"
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
/*电源   油泵   声音   归零   中心   参数   高度   组别   坐标*/
/*睡眠   上升   校模   油位   速度   锁定   讯     反加工 关机*/
/*清时   XY保护 深孔   火警   积炭时 积炭高 积炭   钢蚀钢 口令 修底*/
void ClearAI(int flag);
void OpenPower(void);       /*打开副电源*/
void ClosePower(void);      /*关闭副电源*/
void Open_Pump(void);       /*开油泵*/
void Close_Pump(void);      /*关油泵*/
void OpenSound(void);       /*打开声音*/
void Opens_V(unsigned v);   /*声音大小*/
void CloseSound(void);      /*关闭声音*/
void StopF5();
void P2(void);  /*清时*/
void P3(void);  /*XY保护*/
void P4(void);  /*深孔*/
void P5(void);  /*火警*/
void P6(void);  /*积炭时*/
void P7(void);  /*积炭高*/
void P8(void);  /*积炭*/
void P9(void);  /*钢蚀钢*/
void P10(void); /*口令*/
void P16(void);  /*设置*/
void P11(void); /*修底*/
void F2(void);  /*电源*/
void F3(void);  /*泵浦*/
void F4(void);  /*声音*/
void F5(void);  /*归零*/
void F6(void);  /*中心*/
void F7(char flag); /*参数*/
void F8(char flag); /*高度*/
void F9(void);  /*组*/
void F10(void); /*坐标*/
void K2(void);  /*睡眠*/
void K3(void);  /*上升*/
void K4(void);  /*校模*/
void K5(void);  /*油位*/
void K6(void);  /*速度*/
void K7(void);  /*锁定*/
void K8(void);  /*讯息*/
void K9(void);  /*反加工*/
void K10(void); /*关机*/
void F11(void); /*停止放电*/
void F12(void); /*开始放电*/
void Disp_time(void);/*显示时间*/
void Show_hint(void);
void Init_IO()
{
    OTPS1=0x0a;
    OTPS0=0x00;
    IOZ0=0xe0;
    IO0=0x10;       /*开24V*/
    IO1=0x00;
    IO2=0x00;
    IO3=0x10;       /*开高压*/
    IO4=0x00;
    outb(Address_Z,0x01);
    outb(Address_Z+1,IOZ0);
    outb(OSC,0x01);
    outportb(OSC+1,OTPS0);
    outb(OSC,0x01);
    outportb(OSC+2,OTPS1);
    outb(OSC,0x02);
    outportb(OSC+3,0xff);
    outportb(IO+0,IO0);
    outportb(IO+1,IO1);
    outportb(IO+3,IO3);
    outportb(IO+4,IO4);
}
void main(int argc,char *argv[])
{
    register unsigned i;
    char far *pi;
    long ll;
    Argc=argc-1;
    if(argc)
    {
        Argv=argv[0];
    }
    Init_IO();
    Init_coordinate();
    Position_control_z(ZPosi(0));
    Position_control_x(XPosi(0));
    Position_control_y(YPosi(0));
    ResetXYZ();
    Read_Table(Group);
    CheckTab();
    InitScr();
    SetTable(Dis_lines);
    Disp();
    F2();
    ShowKey(1,1);
    Close_OCSF();
    outb(OSC,0x01);
    outb(OSC+1,OTPS0&0x07); /* 速度 */
    KEYL=0;
    for(;;)
    {
        if(ProTime())
        {
            Show_hint();
            ShowTime(1);
            V=Voltage();
            A=Current();
            Ck8255();
            Key();
            if(!Scr_flag &&ProtectTime>18*540)
            {
                CloseScr(0);
            }
            if(Scr_flag<2&&ProtectTime>18*600)
            {
                CloseScr(1);
            }
            if(DIS_OK!=1&&!P[9])
            {
 /*               ShowMess(33);*/
                if(Dis_flag)
                {
                    F11();
                }
            }
            ABS();
        }
        if(F[4])
        {
            Zero();
        }
        if(Dis_flag)
        {
            if(!K[8])
            {
                F12();
            }
            else
            {
                F13();
            }
        }
        SaveXYZ(1);
        ShowXYZ(0x10);
    }
}
void Write_spi(int number,unsigned long value)
{
    WriteSPI(number++,value);
    value>>=8;
    WriteSPI(number++,value);
    value>>=8;
    WriteSPI(number++,value);
    value>>=8;
    WriteSPI(number++,value);
}
unsigned long Read_spi(int number)
{
    unsigned long pi;
    pi =ReadSPI(number+3);
    pi<<=8;
    pi+=ReadSPI(number+2);
    pi<<=8;
    pi+=ReadSPI(number+1);
    pi<<=8;
    pi+=ReadSPI(number);
    return pi;
}
void WriteSPI(int number,unsigned char value)
{
    outportb(spi,0x06);
    delay(2);
    outportb(spi+1,number);
    outportb(spi+2,number>>8);
    outportb(spi+3,value);
    outportb(spi,0x02);
    delay(5);
}
unsigned char ReadSPI(int number)
{
    unsigned char pi;
    outportb(spi+1,number);
    outportb(spi+2,number>>8);
    outportb(spi,0x03);
    delay(5);
    pi=inportb(spi+3);
    return pi;
}
void P2(void)   /*清时*/
{
    if(P[1])
    {
        if(KeyN!=0x7FFFFFFF)
        {
            if(KeyN==3140)
            {
                ShowTimeD(0x80);
            }
            if(KeyN==321000)
            {
                position_t=0;
            }
            if(KeyN==8888000)
            {
                PassWordF=0;
            }
        }
        P[1]=0;
        ShowInput(0);
        ShowMess(0);
    }
    else
    {
        ClearKey(0);
        ShowMess(29);
        MaxL=7;
        MaxN=9999999;
        MinN=-9999999;
        P[1]=1;
    }
}
void P3(void)   /*XY保护*/
{
    ClearKey(0);
    if(P[2])
    {
        P[2]=0;
    }
    else
    {
        P[2]=1;
    }
}
void P4(void)   /*深孔*/
{
    ClearKey(0);
    if(P[3])
    {
        P[3]=0;
    }
    else
    {
        P[3]=1;
    }
}
void P5(void)   /*火警*/
{
    ClearKey(0);
    if(P[4])
    {
        P[4]=0;
    }
    else
    {
        P[4]=1;
    }
    ShowTB(0x17);
}
void P6(void)   /*积炭时*/
{
    AI_time=0;
    if(P[5]==2)
    {
        ClearKey(0);
        P[5]=0;
        return;
    }
    if(P[5]==1)
    {
        P[5]=2;
        if(KeyN!=0x7fffffff)
        {
            AI_time=KeyN;
        }
        if(!AI_time)
        {
            P[5]=0;
        }
        ClearKey(0);
    }
    else
    {
        ClearKey(0);
        ShowMess(38);
        P[5]=1;
        MaxL=3;
        MaxN=999;
        MinN=1;
    }
}
void P7(void)   /*积炭高*/
{
    AI_high=0;
    if(P[6]==2)
    {
        ClearKey(0);
        P[6]=0;
        return;
    }
    if(P[6]==1)
    {
        P[6]=2;
        if(KeyN!=0x7fffffff)
        {
            AI_high=KeyN;
        }
        if(!AI_high)
        {
            P[6]=0;
        }
        ClearKey(0);
    }
    else
    {
        ClearKey(0);
        ShowMess(39);
        P[6]=1;
        MaxL=3;
        MaxN=999;
        MinN=1;
    }
}
void P8(void)   /*积炭开关*/
{
    ClearKey(0);
    if(P[7]==2)
    {
        P[7]=0;
    }
    else
    {
        if(!P[6]&&!P[5])
        {
            P[6]=2;
            AI_high=250/DZC;
        }
        P[7]++;
    }
    if(Dis_flag)
    {
        if(AI)
        {
            ClearAI(1);
        }
    }
}
void P9(void)   /*钢蚀钢*/
{
    steel=0;
    Steel_flag=0;
    if(P[8]==2)
    {
        ClearKey(0);
        P[8]=0;
        if(Dis_flag)
        {
            JiXin(JX=Table.Jixin[Dis_lines]);
        }/*ShowSteel();*/return;
    }
    if(P[8]==1)
    {
        P[8]=2;
        if(KeyN!=0x7fffffff)
        {
            Steel_flag=KeyN;
        }
        if(!Steel_flag)
        {
            P[8]=0;
        }
        ClearKey(0);
    }
    else
    {
        ClearKey(0);
        ShowMess(21);
        P[8]=1;
        MaxL=2;
        MaxN=99;
        MinN=1;
    }
}
void P10(void)  /*口令*/
{
    unsigned char a;
    long l;
    l=KeyN;
    outb(OSC,0xdf);
    a=inportb(OSC+1);
    if(PassWordF==0)
    {
        a|=0x02;
    }
    else
    {
        a&=0x02;
    }
    if(a==0x00)
    {
        return;
    }
    if(PassWordF==0x6d)
    {
        return;
    }
    if(P[9])
    {
        P[9]=0;
        ShowInput(0);
        if(l!=0x7FFFFFFF)
        {
            if(l==(long)321000)
            {
                outb(OSC,0x01);
                OTPS0|=0x80;
                outportb(OSC+1,OTPS0);
                SetPass(0,0);
                KEYL=8;
                ShowMess(0);
                return;
            }
            else
            {
                ShowMess(30);
                if(!F[1])
                {
                    F2();
                }
            }
        }
        else
        {
            ShowMess(0);
            if(!F[1])
            {
                F2();
            }
        }
    }
    else
    {
        ClearKey(0);
        if(Dis_flag)
        {
            ShowMess(15);
            return;
        }
        else    if(F[1])
        {
            F2();
        }
        ShowMess(29);
        MaxL=7;
        MaxN=9999999;
        MinN=-9999999;
        P[9]=1;
    }
}
void P16(void)  /*设置*/
{
    long l;
    l=KeyN;
    if(P[14])
    {
        P[14]=0;
        ShowInput(0);
        if(l!=0x7FFFFFFF)
        {
            if(l==(long)1234000)
            {
                if(!Dis_flag)
                {
                    KEYL=10;
                    ShowKey(7,0);
                    DispF9(0);
                }
                else
                {
                    ShowMess(15);
                }
                return;
            }
            else
            {
                ShowMess(30);
            }
        }
        else
        {
            ShowMess(0);
        }
    }
    else
    {
        ClearKey(0);
        if(Dis_flag)
        {
            ShowMess(15);
            return;
        }
        else    if(F[1])
        {
            F2();
        }
        ShowMess(29);
        MaxL=7;
        MaxN=9999999;
        MinN=-9999999;
        P[14]=1;
    }
}
void P11(void)  /*修底*/
{
    ClearKey(0);
    if(P[10])
    {
        P[10]=0;
    }
    else
    {
        P[10]=1;
    }
}
void F2(void)
{
    ClearKey(0);
    if(F[1])
    {
        if(Dis_flag||F[4])
        {
            F11();
        }
        IO3&=0x7f;
        outb(IO+3,IO3);/* 锁住 */
        ClosePower();
        F[1]=0;
    }
    else if(!ExitTime)
    {
        IO3|=0x80;
        outb(IO+3,IO3);/* 开锁 */
        OpenPower();
        F[1]=1;
    }
    ShowFlag(1);
}
void OpenPower(void)        /*打开副电源*/
{
    IO3|=0x10;
    outb(IO+3,IO3);
}
void ClosePower(void)       /*关闭副电源*/
{
    IO3&=0xef;
    outb(IO+3,IO3);
}
void F3(void)
{
    ClearKey(0);
    if(F[2])
    {
        Close_Pump();
        F[2]=0;
    }
    else
    {
        Open_Pump();
        F[2]=1;
    }
    ShowFlag(2);
}
void Open_Pump(void)        /*开油泵*/
{
    if(!Pumn_flag)
    {
        IO4|=0x40;
        outb(IO+4,IO4);
        Pump_flag=1;
    }
    else
    {
        ShowTB(0xd);
        ShowMess(15);
    }
}
void Close_Pump(void)       /*关油泵*/
{
    IO4&=0xbf;
    outb(IO+4,IO4);
    Pump_flag=0;
}
void F4(void)
{
    ClearKey(0);
    if(F[3])
    {
        CloseSound();
        F[3]=0;
    }
    else
    {
        F[3]=1;
    }
}
void OpenSound(void)        /*打开声音*/
{
    if(F[3]){
        IO0|=0x08;
        outb(IO+0,IO0);
        Sound_flag=1;
    }

    ProtectTime=0;
    if(Scr_flag)
    {
        OpenScr();
    }
}
void Opens_V(unsigned v)        /*声音大小*/
{
    if(F[3]){
        IO0&=0xf0;
        v&=0x07;
        IO0|=v;
        outb(IO+0,IO0);
    }

    ProtectTime=0;
    if(Scr_flag)
    {
        OpenScr();
    }
}
void Opens_M(unsigned v)        /*亮度高低*/
{
    ProtectTime=0;
    if(Scr_flag)
    {
        OpenScr();
    }
}
void CloseSound(void)       /*关闭声音*/
{
    outb(IO+0,IO0&=0xf7);
    Sound_flag=0;
}
void OpenScr(void)
{
    disable();
    outportb(0x3C4,1);
    outportb(0x3C5,inportb(0x3C5)&0xd7);
    enable();
    Scr_flag=0;
    if(FireFlag&&!(Ck8255_flag&0x80))
    {
        FireFlag++;
    }
    if(Pum_flag&&!(Ck8256_flag&0x80))
    {
        Pum_flag++;
    }
    if(KeyPress())
    {
        KeyAss();
    }
    InitKey(1);
}
void CloseScr(char flag)
{
    disable();
    outportb(0x3C4,1);
    if(!flag)
    {
        outportb(0x3C5,inportb(0x3C5)|0x20);
    }
    else
    {
        outportb(0x3C5,inportb(0x3C5)|0x08);
    }
    enable();
    Scr_flag++;
}
void F5()
{
    ClearKey(0);
    if(F[4])
    {
        StopF5();
    }
    else
    {
        if(Ck8255_flag&0x3||Dis_flag||!F[1])
        {
            ShowMess(15);
            return;
        }
        {
            F[4]=2-K[8];
        }
    }
}
void Zero()
{
    static char byte_2159=0,byte_215A=0;
    if(!F[4])
    {
        byte_2159=0;
        byte_215A=0;
        return;
    }
    lock_z=0;
    offset_ds=0;
    ZeroFlag=0;
    if(byte_2159==0)
    {
        ShowTB(0x1b);
        if(!K[8])
        {
            if(Voltage()<10)
            {
                Velocity_control_z(0x100);
                byte_215A=1;
                delay(0xf00);
                Velocity_control_z(0xffe0);
            }
            else
            {
                Velocity_control_z(0xffd0);
            }
        }
        else
        {
            if(Voltage()<10)
            {
                Velocity_control_z(0xff00);
                byte_215A=1;
                delay(0xf00);
                Velocity_control_z(0x20);
            }
            else
            {
                Velocity_control_z(0x30);
            }
        }
        byte_2159=1;
    }
    else
    {
        if(byte_215A==0)
        {
            if(Voltage()<10)
            {
                if(!K[8])
                {
                    /*Velocity_control_z(0x100);*/
                    byte_215A=1;
                    delay(0xf00);
                    Velocity_control_z(0xffe0);
                }
                else
                {
                    /*Velocity_control_z(0xff00);*/
                    byte_215A=1;
                    delay(0xf00);
                    Velocity_control_z(0x20);
                }
            }
        }
        else if(Voltage()<=1)
        {
            byte_215A++;
            if(byte_215A>50)
            {
                ZeroFlag=1;
                StopF5();
                SoundTime=0x7f;
                byte_2159=0;
                byte_215A=0;
                ShowTB(0xb);
                XYPosition();
                position_d=position_z;
                offset_d=position_z-offset_z;
                ShowDP(0);
                ZeroFlag=1;
            }
        }
        else
        {
            byte_215A=1;
        }
    }
}
void StopF5(void)   /* 停止归零 */
{
    Stop_z(0);
    F[4]=0;
    Zero();
    ShortSound();
    if(KEYL==1)
    {
        ShowKey(2,1);
    }
}
void Position_control_z(long position)
{
    /* Position control mode */
    unsigned char far *pi;
    long l;
    if(K[6])
    {
        return;
    }
    if(DZC==5)
    {
        Pid_kp=0x20;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x40;
    }
    else
    {
        Pid_kp=0x14;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x0d;
    }
    set_PID();/*,输入kp*/
    pi=(char far *)&position;
    outb(Address_Z,0x10);
    outb((Address_Z+1),*pi);
    outb(Address_Z,0x10);
    outb((Address_Z+2),*(pi+1));
    outb(Address_Z,0x10);
    outb((Address_Z+3),*(pi+2));
    outb((Address_Z),0x01);
    outb((Address_Z+1),IOZ0&=0xf8);
}
void Velocity_control_z(unsigned v)
{
    /* proportional velocity control mode */
    if(K[6])
    {
        return;
    }
    if(DZC==5)
    {
        Pid_kp=0x1;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x40;
    }
    else
    {
        Pid_kp=0x1;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x0d;
    }
    set_PID();/*,输入kp*/
    outb(Address_Z,0x04);
    outb(Address_Z+1,(unsigned )v&0xff);
    outb(Address_Z,0x04);
    outb(Address_Z+2,(unsigned)v>>8);
    IOZ0&=0xf8;
    outb((Address_Z),0x01);
    outb((Address_Z+1),IOZ0|=0x01);
}
void Position_control_x(long position)
{
    /* Position control mode */
    unsigned char far *pi;
    long l;
    if(K[6])
    {
        return;
    }
    if(DZC==5)
    {
        Pid_kp=0x20;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x40;
    }
    else
    {
        Pid_kp=0x14;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x0d;
    }
    set_PIDX();/*,输入kp*/
    pi=(char far *)&position;
    outb(ICXaddr,0x10);
    outb((ICXaddr+1),*pi);
    outb(ICXaddr,0x10);
    outb((ICXaddr+2),*(pi+1));
    outb(ICXaddr,0x10);
    outb((ICXaddr+3),*(pi+2));
    outb((ICXaddr),0x01);
    outb((ICXaddr+1),IOX0&=0xf8);
}
void Velocity_control_x(unsigned v)
{
    /* proportional velocity control mode */
    if(K[6])
    {
        return;
    }
    if(DZC==5)
    {
        Pid_kp=0x1;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x40;
    }
    else
    {
        Pid_kp=0x1;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x0d;
    }
    set_PIDX();/*,输入kp*/
    outb(ICXaddr,0x04);
    outb(ICXaddr+1,(unsigned )v&0xff);
    outb(ICXaddr,0x04);
    outb(ICXaddr+2,(unsigned)v>>8);
    IOX0&=0xf8;
    outb((ICXaddr),0x01);
    outb((ICXaddr+1),IOX0|=0x01);
}
void Position_control_y(long position)
{
    /* Position control mode */
    unsigned char far *pi;
    long l;
    if(K[6])
    {
        return;
    }
    if(DZC==5)
    {
        Pid_kp=0x20;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x40;
    }
    else
    {
        Pid_kp=0x14;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x0d;
    }
    set_PIDY();/*,输入kp*/
    pi=(char far *)&position;
    outb(ICYaddr,0x10);
    outb((ICYaddr+1),*pi);
    outb(ICYaddr,0x10);
    outb((ICYaddr+2),*(pi+1));
    outb(ICYaddr,0x10);
    outb((ICYaddr+3),*(pi+2));
    outb((ICYaddr),0x01);
    outb((ICYaddr+1),IOY0&=0xf8);
}
void Velocity_control_y(unsigned v)
{
    /* proportional velocity control mode */
    if(K[6])
    {
        return;
    }
    if(DZC==5)
    {
        Pid_kp=0x1;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x40;
    }
    else
    {
        Pid_kp=0x1;
        Pid_ki=0x00;
        Pid_kd=0x01;
        Pid_st=0x0d;
    }
    set_PIDY();/*,输入kp*/
    outb(ICYaddr,0x04);
    outb(ICYaddr+1,(unsigned )v&0xff);
    outb(ICYaddr,0x04);
    outb(ICYaddr+2,(unsigned)v>>8);
    IOY0&=0xf8;
    outb((ICYaddr),0x01);
    outb((ICYaddr+1),IOY0|=0x01);
}
int KeyPress(void)
{
    struct REGPACK iintr;
    long lt;
    unsigned char ktime[6];
    OTPS0|=0x40;
    outb(OSC,0x01);
    outportb(OSC+1,OTPS0);
    outb(OSC,0x51);
    ktime[5]=inportb(OSC+2);
    ktime[4]=inportb(OSC+1);
    outb(OSC,0x41);
    ktime[3]=inportb(OSC+2);
    ktime[2]=inportb(OSC+1);
    outb(OSC,0x31);
    ktime[1]=inportb(OSC+2);
    ktime[0]=inportb(OSC+1);
    OTPS0&=0xbf;
    outb(OSC,0x01);
    outportb(OSC+1,OTPS0);
    lt = Get_password_time();
    WriteSPI(0x6,lt>>24);
    WriteSPI(0x7,lt>>16);
    WriteSPI(0x8,lt>>8);
    WriteSPI(0x9,lt);
    WriteSPI(0x5,ktime[5]);
    WriteSPI(0x4,ktime[4]);
    WriteSPI(0x3,ktime[3]);
    WriteSPI(0x2,ktime[2]);
    WriteSPI(0x1,ktime[1]);
    WriteSPI(0x0,ktime[0]);
    iintr.r_ax=0x1100;
    intr(0x16,&iintr);
    iintr.r_flags=~iintr.r_flags;
    return iintr.r_flags&0x40;
}
int KeyAss(void)
{
    int ak,ak1;
    ak=bioskey(0x10);
    Fx=0;
    ak1=ak&0xff;
    if(!ak1||ak1==0xe0)
    {
        ak>>=8;
        Fx=1;
    }
    return ak&0xff;
}
void Signal_Jianxi(char flag)
{
    if(!flag)
    {
        IOZ0|=0x06;
        IOZ0&=0xfe;
    }
    else
    {
        IOZ0|=0x04;
        IOZ0&=0xfc;
    }
    outb(Address_Z,0x01);
    outb(Address_Z+1,IOZ0);
}
void ShortSound(void)
{
    OpenSound();
    SoundTime=SoundTIME*2;
}
long ReadRegisterValue_Z(char cLowReg)
{
    unsigned char   i0,i1,i2,i3;
    long    ll;
    outb(Address_Z,cLowReg);
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
    return ll;
}
void F6(void)
{
    long l;
    F[5]=1;
    if(XYZ[0])
    {
        offset_x=position_x/2-XPosi(0);
        XYZ[0]=0;
        ShowXYZ(1);
    }
    if(XYZ[1])
    {
        offset_y=position_y/2-YPosi(0);
        XYZ[1]=0;
        ShowXYZ(2);
    }
    if(XYZ[2]&&!Dis_flag)
    {
        l=position_z/2-ZPosi(0);
        offset_d-=offset_z-l;
        offset_ds=0;
        position_d-=offset_z-l;
        offset_z=0x200;
        XYZ[2]=0;
        ShowDP(1);
        ShowXYZ(3);
    }
    SaveXYZ(0);
    ClearKey(0);
    F[5]=0;
}
void F7(char flag)
{
    int i,j;
    if(!flag)
    {
        ClearKey(0);
    }
    if(!F[6])
    {
        XX=0;
        YY=Dis_start;
        ShowMess(XX+1);
        ClearF7();
        F[6]=1;
        DelLine();
        /*ShowTable(YY+1);*/
        ShowTable(0);
        ShowTable(Dis_start+1);
        /*ShowKey(0x70,0);*/
    }
    else
    {
        if(!flag)
        {
            F[6]=0;
        }
        WriteSPI(Group+0x100,1);
        DelLine();
        i=F12_flag;
        j=Dis_lines;
        if(!Dis_flag)
        {
            i=0;
        }
        F12_flag=0;
        if(!Dis_flag||Table.Shenggao[Dis_lines]==0||K[6])
        {
            SetTable(j);
            if(!K[8])
            {
                F12();
            }
            else
            {
                F13();
            }
        }
        else
        {
            TF8=1;
        }
        F12_flag=i;
        Dis_lines=j;
        /*ShowTable(YY+1);*/
        ShowTable(0);
        ShowTable(Dis_start+1);
        ShowDP(1);
        Write_Table(Group);
        if(!flag) /*ShowKey(2,0);*/
        {
            ShowMess(0);
        }
    }
    /*  if(!flag)ShowKey(1,0);*/
}
void F8(char flag)
{
    if(!flag)
    {
        if(F[7])
        {
            F[7]=0;
            F8H=0;
            F8T=0;
            ShowHT();
            ShowMess(0);
        }
        else
        {
            ClearKey(0);
            ShowMess(16);
            F[7]=1;
            MaxL=2;
            MaxN=99;
            MinN=1;
            ShowHT();
        }
    }
    else
    {
        if(F[7]==1)
        {
            if(KeyN!=0x7fffffff)
            {
                F8H=KeyN;
            }
            else
            {
                F8(0);
                return;
            }
            ShowMess(0);
            ShowMess(17);
            MaxL=2;
            MaxN=99;
            MinN=1;
            F[7]=2;
            ShowHT();
        }
        else if(F[7]==2)
        {
            if(KeyN!=0x7fffffff)
            {
                F8T=KeyN;
            }
            else
            {
                F8(0);
                return;
            }
            F[7]=3;
            ShowHT();
        }
        if(F[7]==3)
        {
            ClearKey(0);
        }
    }
}
void F9(void)
{
    if(!F[8])
    {
        if(Dis_flag||K[3])
        {
            ShowMess(15);
            return;
        }
        ClearKey(0);
        ShowMess(18);
        MaxL=2;
        MaxN=50;
        MinN=1;
        F[8]=1;
    }
    else
    {
        if(KeyN!=0x7fffffff&&KeyN!=Group)
        {
            Group=(char)KeyN;
            Dis_start=Dis_end=0;
            Read_Table(Group);
            ShowTable(0);
            SetTable(0);
            WriteSPI(0x0d,Group);
        }
        F[8]=0;
        ClearKey(0);
    }
}
void CloseAllPower(void)    /*关闭所有电源*/
{
    if(F[1])
    {
        F2();
        delay(10000);
    }
    IO4|=0x02;
    outb(IO+4,IO4);
    Delay1(10000);
}
void K10(void)
{
    ClearKey(0);
    if(!K[9])
    {
        if(Dis_flag||F[4])
        {
            ShowMess(15);
            return;
        }
        if(F[1])
        {
            F2();
        }
        ExitTime=36;
        K[9]=1;
    }
    else
    {
        K[9]=0;
        ExitTime=0;
        IO4&=0xfc;
        outb(IO+4,IO4); /*OpenAllPower*/
    }
}
void K2(void)
{
    ClearKey(0);
    if(K[1])
    {
        K[1]=0;
    }
    else
    {
        K[1]=1;
    }
}
void K3(void)
{
    K_3=0;
    if(K[2]==2)
    {
        ClearKey(0);
        K[2]=0;/*ShowSS();*/
        return;
    }
    if(K[2]==1)
    {
        K[2]=2;
        if(KeyN!=0x7fffffff)
        {
            K_3=KeyN;
        }
        if(!K_3)
        {
            K[2]=0;
        }
        ClearKey(0);
    }
    else
    {
        ClearKey(0);
        ShowMess(19);
        K[2]=1;
        MaxL=2;
        MaxN=99;
        MinN=1;
    }
}
void K4(void)
{
    static zu;
    int i;
    ClearKey(0);
    if(Dis_flag)
    {
        ShowMess(15);
        return;
    }
    if(K[3])
    {
        K[3]=0;
        P[7]=2;
        KeyN=zu;
        F[8]=1;
        F9();
    }
    else
    {
        zu=Group;
        KeyN=0;
        Read_Table(0);
        if(Table.Index[0]!=1)
        {
            Table.Dianliu[0]=StrTable.Dianliu[4];
            Table.Maikuan[0]=StrTable.Maikuan[4];
            Table.Xiuzhi[0]=StrTable.Xiuzhi[4];
            Table.Jianxi[0]=StrTable.Jianxi[4];
            Table.Sudu[0]=StrTable.Sudu[4];
            Table.Shenggao[0]=StrTable.Shenggao[4];
            Table.Gongshi[0]=StrTable.Gongshi[4];
            Table.LV[0]=StrTable.LV[4];
            Table.Jixin[0]=StrTable.Jixin[4];
            Table.Gaoya[0]=StrTable.Gaoya[4];
        }
        for(i=1; i<10; i++)
        {
            Table.Index[i]=0;
        }
        Table.Index[0]=StrTable.Index[4];
        if(!K[8])
        {
            Table.Shendu[0]=StrTable.Shendu[4];
        }
        else
        {
            Table.Shendu[0]=-StrTable.Shendu[4];
        }
        Write_Table(0);
        F[8]=1;
        F9();
        K[3]=1;
        P[7]=0;
    }
}
char OilSwitch()
{
    outb(OSC,0x01);
    return(inb(OSC+3)&0x40);
}
void K5(void)
{
    ClearKey(0);
    if(K[4])
    {
        K[4]=0;
    }
    else
    {
        K[4]=1;
    }
    ShowTB(0x18);
}
void K6(void)
{
}
void K7(void)
{
    ClearKey(0);
    if(K[6])
    {
        K[6]=0;
        IO3|=0x80;
        outb(IO+3,IO3);/* 开锁 */
        IOZ0|=0x20;
        outb(Address_Z,0x01);
        outb(Address_Z+1,IOZ0);           /*ENZ*/
        if(Dis_flag)
        {
            F12_flag=1;
        }
    }
    else
    {
        K[6]=1;
        IO3&=0x7f;
        outb(IO+3,IO3);/* 锁住 */
        IOZ0&=0xdf;
        outb(Address_Z,0x01);
        outb(Address_Z+1,IOZ0);           /*DISZ*/
        if(Dis_flag)
        {
            F12_flag=1;
        }
    }
    ShowFlag(4);
}
void K8(void)
{
    ClearKey(0);
    if(K[7])
    {
        K[7]=0;
    }
    else
    {
        K[7]=1;
    }
}
void K9(void)
{
    ClearKey(0);
    if(Dis_flag)
    {
        ShowMess(15);
        return;
    }
    if(K[2])
    {
        K3();
    }
    if(K[8])
    {
        K[8]=0;
    }
    else
    {
        K[8]=1;
    }
    if(K[3])
    {
        if(!K[8])
        {
            Table.Shendu[0]=StrTable.Shendu[4];
        }
        else
        {
            Table.Shendu[0]=-StrTable.Shendu[4];
        }
        ShowTable(1);
        SetTable(0);
    }
    offset_ds=0;
    ShowFlag(3);
}
void K10b(void)
{
    ClearKey(0);
    if(Dis_flag)
    {
        ShowMess(15);
        return;
    }
    position_d=position_z/*1999999*/;
    if(!K[9])
    {
        K[9]=1;
        offset_xx[1]=offset_x;
        offset_yy[1]=offset_y;
        offset_zz[1]=offset_z;
        XYZGroup=2;
        WriteSPI(0x0e,XYZGroup);
        offset_x=offset_xx[2];
        offset_y=offset_yy[2];
        offset_z=offset_zz[2];
        ShowXYZ(0);
        offset_ds=0;
    }
    else
    {
        K[9]=0;
        offset_xx[2]=offset_x;
        offset_yy[2]=offset_y;
        offset_zz[2]=offset_z;
        XYZGroup=1;
        WriteSPI(0x0e,XYZGroup);
        offset_x=offset_xx[1];
        offset_y=offset_yy[1];
        offset_z=offset_zz[1];
        ShowXYZ(0);
    }
}
void F10(void)
{
    if(!F[9])
    {
        ClearKey(0);
        if(Dis_flag)
        {
            ShowMess(15);
            return;
        }
        ShowMess(26);
        F[9]=1;
        MaxL=1;
        MaxN=9;
        MinN=0;
    }
    else
    {
        F[9]=0;
        if(KeyN!=0x7fffffff)
        {
            offset_xx[XYZGroup]=offset_x;
            offset_yy[XYZGroup]=offset_y;
            offset_zz[XYZGroup]=offset_z;
            XYZGroup=(char)KeyN;
            WriteSPI(0x0e,XYZGroup);
            offset_x=offset_xx[XYZGroup];
            offset_y=offset_yy[XYZGroup];
            offset_z=offset_zz[XYZGroup];
            XYPosition();
            position_d=position_z/*1999999*/;
            ShowXYZ(0);
            ShowDP(0);
        }
        else
        {
            ShowMess(0);
        }
        ClearKey(0);
    }
    ShowFlag(6);
}
void K11()
{
    exitp(1);
}
#if TESTING
void K12()
{
    SaveBMP();
}
void SaveBMP()
{
    FILE *fp;
    int i,j;
    unsigned k,k0;
    unsigned char BMP0[]=
    {
        "\x42\x4D\xFE\x13\x00\x00\x00\x00\x00\x00\x76\x00\x00\x00\x28\x00"
        "\x00\x00\xC8\x00\x00\x00\x32\x00\x00\x00\x01\x00\x04\x00\x00\x00"
        "\x00\x00\x88\x13\x00\x00\x12\x0B\x00\x00\x12\x0B\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x00\x00\x80"
        "\x00\x00\x00\x80\x80\x00\x80\x00\x00\x00\x80\x00\x80\x00\x80\x80"
        "\x00\x00\x80\x80\x80\x00\xC0\xC0\xC0\x00\x00\x00\xFF\x00\x00\xFF"
        "\x00\x00\x00\xFF\xFF\x00\xFF\x00\x00\x00\xFF\x00\xFF\x00\xFF\xFF"
        "\x00\x00\xFF\xFF\xFF\x00\xFF"
    };
    fp=fopen("HUAFENG.BMP","wb+");
    for(i=0; i<0x76; i++)
    {
        BMP[i]=BMP0[i];
    }
    BMP[2]=0x76;
    BMP[3]=0x58;
    BMP[4]=0x2;
    BMP[0x12]=640&0xff;
    BMP[0x13]=0x2;
    BMP[0x16]=480&0xff;
    BMP[0x17]=0x1;
    fwrite(BMP,1,0x76,fp);
    for(i=480; i; i--)
    {
        for(j=0; j<640; j+=2)
        {
            k0=getpixel(j,i-1);
            k0&=0xf;
            if(k0==7)
            {
                k0=8;
            }
            else if(k0==8)
            {
                k0=7;
            }
            k=k0<<4;
            k0=getpixel(j+1,i-1);
            k0&=0xf;
            if(k0==7)
            {
                k0=8;
            }
            else if(k0==8)
            {
                k0=7;
            }
            k|=k0;
            BMP[j/2]=k;
        }
        fwrite(BMP,1,320,fp);
    }
    fclose(fp);
}
#endif
void exitp(char flag)
{
    if(F[1])
    {
        F2();
    }
    IO0=0x00;
    outportb(IO+0,IO0);
    outb(OSC,0x01);
    outportb(OSC+1,0x00);
    if(flag)
    {
        InitKey(0);
        closegraph();
        exit(0);
    }
}
long Velocity_GetTime(long Time)
{
    long ll;
    long far * pi;
    char la;
    pi=MK_FP(0x40,0x6c);
    ll=*pi;
    if(ll<Velocity_Time)
    {
        if(ll>=Velocity_Time+Time-0x1800B0)
        {
            Velocity_Time=ll;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(ll>=Velocity_Time+Time)
        {
            Velocity_Time=ll;
            return 1;
        }
        else
        {
            return 0;
        }
    }
}
void F11(void)
{
    char test[10];
    Dis_first=0;
    if(P[10])
    {
        P[10]=1;
    }
    if(F[4])    /* 归零 */
    {
        StopF5();
    }
    offset_rz=ZPosi(0);
    Position_control_z(offset_rz);
    F11_flag=1;
    if(Dis_flag)
    {
        F12_flag=0;
        Dis_flag=0;
        AI=0;/*AIHigh=1;AITime=0;*/
        ShowFL(0);
        Close_OCSF();
        Close_Elect();
        if(F[2])
        {
            Close_Pump();
            ShowFlag(2);
        }
        outb(OSC,0x01);
        outb(OSC+2,OTPS1&=0xfe);     /*DIS 指示灯*/
        outb(IO+0,IO0&=0xdf);
        outb(IO+4,IO4&=0xdf);
        outb(IO+4,IO4|=0x10);  /*极性恢复+ */
        /*ShowTable(0);*/
        ShowTable(Dis_lines+1);
    }
    lock_z=1;
    outb(OSC,0x01);
    outb(OSC+2,OTPS1&=0xfe);     /*DIS 指示灯*/
    outb(IO+0,IO0&=0xdf);
    outb(IO+4,IO4&=0xdf);
    outb(IO+4,IO4|=0x10);   /*极性恢复+ */
    F11_flag=1;
}
void F12(void)
{
    int i;
    static ia;
    static int ht,end;
    static long ld,lh,lt,positiond,hth,lhv,ldv,positiond1;
    static int Oil;
    static DY;
    static unsigned char vol;
    static XD=0;/*修底*/
    long ll;
    unsigned i1,i2,i3,i4,i5,ii,lik;
    unsigned long l1,l2;
    char str[0x10];
    if(F12_flag==0)
    {
        XD=0;
        if(Dis_flag)
        {
            if(Ck8255_flag&0x3||!F[1])
            {
                ShowMess(15);
                F11();
                return;
            }
            for(i=Dis_lines; i<=Dis_end; i++)
            {
                if(Table.Shendu[i]>position_z)
                {
                    Dis_lines++;
                    continue;
                }
                if(Table.Index[i]!=1)
                {
                    Dis_lines++;
                    continue;
                }
                break;
            }
            if(Dis_lines>Dis_end&&Dis_flag)
            {
                i=Dis_lines;
                Dis_lines=Dis_start;
                /*ShowTable(i+1);*/
                ShowTable(0);
                SoundTime=SDST;
                offset_d=9999999/DZC*2;
                offset_ds=0;
                if(!Table.Index[Dis_lines])
                {
                    ShowMess(32);
                    F11();
                    return;
                }
                else
                {
                    ShowMess(28);
                }
                if(!Dis_first)
                {
                    F11();
                    return;
                }
                F11();
                offset_rz=ZPosi(0);
                if(K[1])
                {
                    K[9]=1;
                    ExitTime=360;
                    if(KEYL==4)
                    {
                        ShowKey(5,4);
                    }
                }
                if(K[2])
                {
                    offset_rz+=((long)K_3*1000/DZC);
                }
                Position_control_z(offset_rz);
                return;
            }
            if(Dis_lines<=Dis_end)
            {
                ShowTable(Dis_lines+1);
                SetTable(Dis_lines);
                DY=Table.Jianxi[Dis_lines]/2+41+Table.Gaoya[Dis_lines]*2;
            }
            /*4.28 跳升不变化Bug*/
            lh=(Table.Shenggao[Dis_lines]%100)*100/DZC;
            ld=Table.Dianliu[Dis_lines]/8+MianJi;
            ld/=DZC;
            lt=10*MianJi/DZC;
            F12_flag++;
            AI=0;
            AITime=0;
            AIHigh=0;
            positiond1=9999999/DZC;
        }
        Dis_first=1;
        return;
    }
    if(DIS_OK!=1&&DIS_OK1!=1)
    {
        ShowMess(33);
        F11();
    }
    if(Ck8255_flag&0x3||!F[1])
    {
        F11();
        return;
    }
    if(!Dis_flag)
    {
        return;
    }
    switch(F12_flag)
    {
    case 0:
        break;
    case 1:
        ShowTable(0);
        ShowFL(0);
        if(Voltage()<10)
        {
            Velocity_control_z(0x40);
            Delay1(0x100);
        }
        Position_control_z(ZPosi(0));
        ShowTable(Dis_lines+1);
        end=0;
        ZeroFlag=0;
        ShowTB(0);
        SetTable(Dis_lines);
        Open_Elect();
        Delay(0x200);
        Open_OCSF();
        outb(OSC,0x01);
        outb(OSC+2,OTPS1|=0x01);   /*DIS LED*/
        GetTime((long)0);
        offset_t=Oldtime/18.2;
        if(F[2])
        {
            Open_Pump();
            ShowFlag(2);
        }
        if(K[6])
        {
            ShowFL(2);
            ShowFL(5);
            F12_flag=9;
            SetDianliu(Table.Dianliu[Dis_lines]);
            ShowDP(0);
            return;
        }
        Delay1(0x100);
        SetDianliu(1);
        Signal_Jianxi(K[8]);
        F12_flag++;
        break;
    case 2:
        if(offset_d<ZPosi(0)/*&&!K[3]*/&&offset_ds&&0)
        {
            if(P[3])
            {
                Close_OCSF();
            }
            positiond=offset_d+(545/DZC);
            ldv=positiond+ld;
            if(ldv<ZPosi(0))
            {
                Position_control_z(ldv);
            }
            if(lhv-ldv>1000/DZC)
                if(Elect_speed>30) {}
            F12_flag=4;
            ia=3;
            ShowDP(1);
            positiond=offset_d+45/DZC;
            SetDianliu(Table.Dianliu[Dis_lines]);
        }
        else
        {
            F12_flag++;
            offset_d=9999999/DZC*2;
        }
        break;
    case 3:
        if(offset_z+ZPosi(0)<Table.Shendu[Dis_lines])
        {
            Dis_lines++;
            if(Dis_lines>Dis_end)
            {
                F12_flag=0;
                return;
            }
            SetTable(Dis_lines);
            Open_Elect();
            DY=Table.Jianxi[Dis_lines]/2+41+Table.Gaoya[Dis_lines]*2;
            ShowTable(0);
            ShowTable(Dis_lines+1);
            /*4.28 跳升不变化Bug*/
            lh=(Table.Shenggao[Dis_lines]%100)*100/DZC;
            ld=Table.Dianliu[Dis_lines]/8+MianJi;
            ld/=DZC;
            lt=10*MianJi/DZC;
            return;
        }
        ShowFL(5);
        position_d=position_z;
        if(Voltage()<=Table.Jianxi[Dis_lines]) /*Delay1(50);*/
        {
            if(Voltage()<=Table.Jianxi[Dis_lines])
            {
                F12_flag+=2;
            }
            ShowFL(0);
        }
        ShowDP(1);
        positiond=position_z-offset_z+45/DZC;
        SetDianliu(Table.Dianliu[Dis_lines]);
        break;
    case 4:
        ShowFL(8);
        if(!ERR_XY)
        {
            if(ia>2) {}
            if(ia==2)
            {
                if(ZPosi(0)<(long)(ldv+2000/DZC))
                {
                    if(ia==2)
                    {
                        ia=0;
                    }
                    else
                    {
                        ia=1;
                    }
                }
            }
        }
        if(ERR_XY||ZPosi(0)<(long)(positiond+ld+10))
        {
            if(P[3])
            {
                Open_OCSF();
            }
            F12_flag++;
            if(P[10]<=1) /*NO 修底*/
            {
                Signal_Jianxi(K[8]);
                if(ia||ERR_XY)
                {
                    position_d=position_z;
                    ShowFL(0);
                    ShowFL(4);
                    ShowFL(8);
                    ia=0;
                    positiond1=positiond;
                }
            }
            else
            {
                F12_flag++;
            }
            ERR_XY=0;
            GetTime((long)0);
        }
        break;
    case 5: /*ShowFL(5);*/
        GetTime((long)0);
        if(position_d<=position_p||ZPosi(0)+offset_z<position_p)
        {
            end++;
            if(end>=1||ZPosi(0)+offset_z<position_p)
            {
                Dis_lines++;
                if(Dis_lines>Dis_end)
                {
                    if(!P[10])
                    {
                        F12_flag=0;
                        return;
                    }
                    else        /*修底*/
                    {
                        Dis_lines--;
                        P[10]++;
                        ShowMess(22);
                        vol=0xff;
                        positiond=position_p-offset_z;
                        ld=0;
                        F12_flag++;
                        return;
                    }
                }
                SetTable(Dis_lines);
                Open_Elect();
                DY=Table.Jianxi[Dis_lines]/2+41+Table.Gaoya[Dis_lines]*2;
                ShowTable(0);
                ShowTable(Dis_lines+1);
                /*4.28 跳升不变化Bug*/
                lh=(Table.Shenggao[Dis_lines]%100)*100/DZC;
                ld=Table.Dianliu[Dis_lines]/8+MianJi;
                ld/=DZC;
                lt=10*MianJi/DZC;
            }
        }
        else
        {
            end=0;
        }
        if(Voltage()>DY)
        {
            return;
        }
        if(Table.Shenggao[Dis_lines]>0)
        {
            F12_flag++;
        }
        break;
    case 6:
        ShowFL(5);
        ERR_XY=0;
        if(P[10]>1&&end>1) /*修底*/
        {
            position_d=position_p;
            if(Voltage()+10>vol&&ZPosi(0)+offset_z<=position_p)
            {
                delay(100);
                if(Voltage()+10>vol&&ZPosi(0)+offset_z<=position_p)
                {
                    XD++;
                }
                if(XD>5)
                {
                    F12_flag=0;
                    Dis_lines++;
                    return;
                }
            }
            if(GetTime(Work_time))
            {
                Close_OCSF();
                ShowFL(6);
                lhv=positiond+MianJi/DZC;
                F12_flag++;
            }
            return;
        }
        if(position_d<=position_p||ZPosi(0)+offset_z<position_p)
        {
            end++;
            if(end>=1||ZPosi(0)+offset_z<position_p)
            {
                Dis_lines++;
                if(Dis_lines>Dis_end)
                {
                    if(!P[10])
                    {
                        F12_flag=0;
                        return;
                    }
                    else        /*修底*/
                    {
                        Dis_lines--;
                        P[10]++;
                        ShowMess(22);
                        vol=0xff;
                        positiond=position_p-offset_z;
                        ld=0;
                    }
                }
                if(P[10]<=1)
                {
                    SetTable(Dis_lines);
                    Open_Elect();
                    DY=Table.Jianxi[Dis_lines]/2+41+Table.Gaoya[Dis_lines]*2;
                    ShowTable(0);
                    ShowTable(Dis_lines+1);
                }
                /*4.28 跳升不变化Bug*/
                lh=(Table.Shenggao[Dis_lines]%100)*100/DZC;
                ld=Table.Dianliu[Dis_lines]/8+MianJi;
                ld/=DZC;
                lt=10*MianJi/DZC;
            }
        }
        else
        {
            end=0;
        }
        if(P[10]<=1)
        {
            positiond=ZPosi(0);
        }
        if(offset_d>positiond)
        {
            offset_d=positiond;
        }
        if(positiond<positiond1)
        {
            positiond1=positiond;
        }
        if(positiond>positiond1+1)
        {
            if(!AI)
            {
                AI=1;
            }
            AIHigh=positiond-positiond1-1;
        }
        else if(AI)
        {
            if(AI==2)
            {
                ClearAI(1);
            }
            AI=0;
            AITime=0;
            AIHigh=0;
        }
        offset_ds=1;
        if(GetTime(Work_time)||P[10]>1)
        {
            Close_OCSF();
            ShowFL(6);
            if(P[10]<=1)
            {
                positiond+=45/DZC;
            }
            F12_flag++;
        }
        break;
    case 7:
        /*11.11 speed*/
        /*ShowLong(lt);*/
        if(Zposi(0)<=(positiond+lt))
        {
            if(Zposi(0)<positiond)
            {
                Velocity=100;
            }
            else
            {
                Velocity=2*(100-MianJi)+(Zposi(0)-positiond);
            }
            Velocity=abs(Velocity);
            Velocity_control_z(Velocity);
        }
        else
        {
            Position_control_z(positiond+lt+100/DZC);
            if(ZPosi(0)>positiond+lt)
            {
                if(F[7]==3)
                {
                    if(++ht>=F8T)
                    {
                        ht=0;
                        hth=(long)F8H*1000/DZC;
                    }
                    else
                    {
                        hth=0;
                    }
                }
                else
                {
                    ht=0;
                    hth=0;
                }
                lhv=positiond+lh+hth;
                Position_control_z(lhv+50/DZC);
                F12_flag++;
            }
        }
        break;
    case 8:
        ShowFL(7);
        if(ZPosi(0)>lhv-50/DZC)
        {
            /*ShowLong(ld);*/
            if(TF8)
            {
                SetTable(Dis_lines);
                Open_Elect();
                /*4.28 跳升不变化Bug*/
                DY=Table.Jianxi[Dis_lines]/2+41+Table.Gaoya[Dis_lines]*2;
                lh=(Table.Shenggao[Dis_lines]%100)*100/DZC;
                ld=Table.Dianliu[Dis_lines]/8+MianJi;
                ld/=DZC;
                lt=10*MianJi/DZC;
                /*ShowLong(lt);*/
                TF8=0;
            }
            if(K[4])
            {
                if(OilSwitch())
                {
                    ShowMess(27);
                    Oil=1;
                    return;
                }
                if(Oil)
                {
                    Oil=0;
                    ShowMess(0);
                }
            }
            if(Steel_flag)
            {
                if(++steel==Steel_flag)
                {
                    JiXin(JX);
                    steel=0;
                    JX^=1;
                }
            }
            ldv=positiond+ld;
            F12_flag=4;
            ia=2;
            if(!P[3]||P[10]>1)
            {
                Open_OCSF();
            }
            ii=Voltage();
            ii+=Voltage();
            ii+=Voltage();
            ii+=Voltage();
            vol=ii/4;
            if(vol>0xfe-10)
            {
                vol=0xfe-10;
            }
            if(!ERR_XY)
            {
                Position_control_z(ldv);
            }
        }
        break;
    case 9:
        break;
    }
}
void F13(void)
{
    int i;
    static ia;
    static int ht,end;
    static long ld,lh,positiond,hth,lhv,ldv,positiond1;
    static int Oil;
    static DY;
    static unsigned char vol;
    unsigned ii;
    long ll;
    if(F12_flag==0)
    {
        if(Dis_flag)
        {
            if(Ck8255_flag&0x3||!F[1])
            {
                ShowMess(15);
                F11();
                return;
            }
            for(i=Dis_lines; i<=Dis_end; i++)
            {
                if(Table.Shendu[i]<position_z)
                {
                    Dis_lines++;
                    continue;
                }
                if(Table.Index[i]!=1)
                {
                    Dis_lines++;
                    continue;
                }
                break;
            }
            if(Dis_lines>Dis_end&&Dis_flag)
            {
                i=Dis_lines;
                Dis_lines=Dis_start;
                ShowTable(i+1);
                SoundTime=SDST;
                offset_d=-9999999/DZC*2;
                offset_ds=0;
                if(!Table.Index[Dis_lines])
                {
                    ShowMess(32);
                    F11();
                    return;
                }
                else
                {
                    ShowMess(28);
                }
                if(!Dis_first)
                {
                    F11();
                    return;
                }
                F11();
                offset_rz=ZPosi(0);
                if(K[1])
                {
                    K[9]=1;
                    ExitTime=360;
                    if(KEYL==4)
                    {
                        ShowKey(5,4);
                    }
                }
                if(K[2])
                {
                    offset_rz-=((long)K_3*1000/DZC);
                }
                Position_control_z(offset_rz);
                return;
            }
            if(Dis_lines<=Dis_end)
            {
                ShowTable(Dis_lines+1);
                SetTable(Dis_lines);
                DY=Table.Jianxi[Dis_lines]/2+41+
                   Table.Gaoya[Dis_lines]*2;
            }
            /*4.28 跳升不变化Bug*/
            lh=(Table.Shenggao[Dis_lines]%100)*100/DZC;
            ld=10*MianJi/DZC;
            F12_flag++;
            AI=0;
            AITime=0;
            AIHigh=0;
            positiond1=-9999999/DZC;
        }
        Dis_first=1;
        return;
    }
    if(DIS_OK!=1&&DIS_OK1!=1)
    {
        ShowMess(33);
        F11();
    }
    if(Ck8255_flag&0x3||!F[1])
    {
        F11();
        return;
    }
    if(!Dis_flag)
    {
        return;
    }
    switch(F12_flag)
    {
    case 0:
    case 1:
        ShowTable(0);
        ShowFL(0);
        if(Voltage()<10)
        {
            Velocity_control_z(0xff80);
            Delay1(0x100);
        }
        Position_control_z(ZPosi(0));
        ShowTable(Dis_lines+1);
        end=0;
        ZeroFlag=0;
        ShowTB(0);
        SetTable(Dis_lines);
        Open_Elect();
        Delay(0x200);
        Open_OCSF();
        outb(OSC,0x01);
        outb(OSC+2,OTPS1|=0x01);    /*DIS LED*/
        GetTime((long)0);
        offset_t=Oldtime/18.2;
        if(F[2])
        {
            Open_Pump();
            ShowFlag(2);
        }
        if(K[6])
        {
            ShowFL(2);
            ShowFL(5);
            F12_flag=9;
            SetDianliu(Table.Dianliu[Dis_lines]);
            ShowDP(0);
            return;
        }
        Delay1(0x100);
        SetDianliu(1);
        Signal_Jianxi(K[8]);
        F12_flag++;
        break;
    case 2:
        if(offset_d>ZPosi(0)/*&&!K[3]*/&&offset_ds&&0)
        {
            if(P[3])
            {
                Close_OCSF();
            }
            positiond=offset_d-545/DZC;
            ldv=positiond-ld;
            if(ldv<ZPosi(0))
            {
                Position_control_z(ldv);
            }
            if(lhv-ldv<-1000/DZC)if(Elect_speed>30)
                {
                }
            F12_flag=4;
            ia=3;
            ShowDP(1);
            positiond=offset_d-45/DZC;
            SetDianliu(Table.Dianliu[Dis_lines]);
        }
        else
        {
            F12_flag++;
            offset_d=-9999999/DZC*2;
        }
        break;
    case 3:
        if(offset_z+ZPosi(0)>Table.Shendu[Dis_lines])
        {
            Dis_lines++;
            if(Dis_lines>Dis_end)
            {
                F12_flag=0;
                return;
            }
            SetTable(Dis_lines);
            Open_Elect();
            DY=Table.Jianxi[Dis_lines]/2+41+
               Table.Gaoya[Dis_lines]*2;
            ShowTable(0);
            ShowTable(Dis_lines+1);
            return;
        }
        ShowFL(5);
        position_d=position_z;
        if(Voltage()<=Table.Jianxi[Dis_lines]) /*Delay1(50);*/
        {
            if(Voltage()<=Table.Jianxi[Dis_lines])
            {
                F12_flag+=2;
            }
            ShowFL(0);
        }
        ShowDP(1);
        positiond=position_z-offset_z-45/DZC;
        SetDianliu(Table.Dianliu[Dis_lines]);
        break;
    case 4:
        ShowFL(8);
        if(!ERR_XY)
        {
            if(ia>2)
            {
            }
            if(ia==2)
            {
                if(ZPosi(0)<(long)(ldv+2000/DZC))
                {
                    if(ia==2)
                    {
                        ia=0;
                    }
                    else
                    {
                        ia=1;
                    }
                }
            }
        }
        if(ERR_XY||ZPosi(0)<(long)(positiond-ld/10+10))
        {
            if(P[3])
            {
                Open_OCSF();
            }
            F12_flag++;
            if(P[10]<2) /*NO 修底*/
            {
                Signal_Jianxi(K[8]);
                if(ia||ERR_XY)
                {
                    position_d=position_z;
                    ShowFL(0);
                    ShowFL(4);
                    ShowFL(8);
                    ia=0;
                    positiond1=positiond;
                }
            }
            else
            {
                F12_flag++;
            }
            ERR_XY=0;
            GetTime((long)0);
        }
        break;
    case 5:
        GetTime((long)0);
        if(position_d>=position_p||ZPosi(0)+offset_z>position_p)
        {
            end++;
            if(end>=1||ZPosi(0)+offset_z>position_p+1)
            {
                Dis_lines++;
                if(Dis_lines>Dis_end)
                {
                    if(!P[10])
                    {
                        F12_flag=0;
                        return;
                    }
                    else        /*修底*/
                    {
                        Dis_lines--;
                        P[10]++;
                        ShowMess(22);
                        vol=0xff;
                        positiond=position_p-offset_z;
                        ld=0;
                        F12_flag++;
                        return;
                    }
                }
                SetTable(Dis_lines);
                Open_Elect();
                DY=Table.Jianxi[Dis_lines]/2+41+
                   Table.Gaoya[Dis_lines]*2;
                ShowTable(0);
                ShowTable(Dis_lines+1);
                /*4.28 跳升不变化Bug*/
                lh=(Table.Shenggao[Dis_lines]%100)*100/DZC;
                ld=10*MianJi/DZC;
            }
        }
        else
        {
            end=0;
        }
        if(Voltage()>DY)
        {
            return;
        }
        if(Table.Shenggao[Dis_lines]>0)
        {
            F12_flag++;
        }
        break;
    case 6:
        ShowFL(5);
        ERR_XY=0;
        if(P[10]>1&&end>1) /*修底*/
        {
            position_d=position_p;
            if(Voltage()+10>vol&&ZPosi(0)+offset_z>=position_p)
            {
                delay(100);
                if(Voltage()+10>vol&&ZPosi(0)+offset_z>=position_p)
                {
                    F12_flag=0;
                    Dis_lines++;
                    return;
                }
            }
            if(GetTime(Work_time))
            {
                Close_OCSF();
                ShowFL(6);
                lhv=positiond-MianJi/DZC;
                F12_flag++;
            }
            return;
        }
        if(position_d>=position_p||ZPosi(0)+offset_z>position_p)
        {
            end++;
            if(end>=1||ZPosi(0)+offset_z>position_p)
            {
                Dis_lines++;
                if(Dis_lines>Dis_end)
                {
                    if(!P[10])
                    {
                        F12_flag=0;
                        return;
                    }
                    else        /*修底*/
                    {
                        Dis_lines--;
                        P[10]++;
                        ShowMess(22);
                        vol=0xff;
                        positiond=position_p-offset_z;
                        ld=0;
                    }
                }
                if(P[10]<=1)
                {
                    SetTable(Dis_lines);
                    Open_Elect();
                    DY=Table.Jianxi[Dis_lines]/2+41+
                       Table.Gaoya[Dis_lines]*2;
                    ShowTable(0);
                    ShowTable(Dis_lines+1);
                }
                /*4.28 跳升不变化Bug*/
                lh=(Table.Shenggao[Dis_lines]%100)*100/DZC;
                ld=10*MianJi/DZC;
            }
        }
        else
        {
            end=0;
        }
        if(P[10]<=1)
        {
            positiond=ZPosi(0);
        }
        if(offset_d<positiond)
        {
            offset_d=positiond;
        }
        if(positiond>positiond1)
        {
            positiond1=positiond;
        }
        if(positiond<positiond1-1)
        {
            if(!AI)
            {
                AI=1;
            }
            AIHigh=0-(positiond-positiond1-1);
        }
        else if(AI)
        {
            if(AI==2)
            {
                ClearAI(1);
            }
            AI=0;
            AITime=0;
            AIHigh=0;
        }
        offset_ds=1;
        if(GetTime(Work_time)||P[10]>1)
        {
            Close_OCSF();
            ShowFL(6);
            lhv=positiond-MianJi/DZC;
            if(P[10]<=1)
            {
                positiond-=45/DZC;
            }
            F12_flag++;
        }
        Velocity_GetTime((long)0);
        V_time=(long)MianJi;
        V_time=V_time*V_time;
        V_time/=100;
        Velocity=(long)MianJi;
        Velocity=(150-Velocity)*5;
        Velocity=abs(Velocity);
        Velocity=-Velocity;
        break;
    case 7:
        /*11.11 speed*/
        if(Zposi(0)>=(positiond-ld))
        {
            if(Zposi(0)>positiond)
            {
                Velocity=-100;
            }
            else
            {
                Velocity=2*(100-MianJi)+(Zposi(0)-positiond);
            }
            Velocity=abs(Velocity);
            Velocity_control_z(-Velocity);
        }
        else
        {
            Position_control_z(positiond-ld-100/DZC);
            if(ZPosi(0)<positiond-ld)
            {
                if(F[7]==3)
                {
                    if(++ht>=F8T)
                    {
                        ht=0;
                        hth=(long)F8H*1000/DZC;
                    }
                    else
                    {
                        hth=0;
                    }
                }
                else
                {
                    ht=0;
                    hth=0;
                }
                lhv=positiond-lh-hth;
                Position_control_z(lhv-50/DZC);
                F12_flag++;
            }
        }
        break;
    case 8:
        ShowFL(7);
        if(ZPosi(0)<lhv+50/DZC)
        {
            if(TF8)
            {
                SetTable(Dis_lines);
                Open_Elect();
                /*4.28 跳升不变化Bug*/
                lh=(Table.Shenggao[Dis_lines]%100)*100/DZC;
                ld=10*MianJi/DZC;
                TF8=0;
            }
            if(K[4])
            {
                if(OilSwitch())
                {
                    ShowMess(27);
                    Oil=1;
                    return;
                }
                if(Oil)
                {
                    Oil=0;
                    ShowMess(0);
                }
            }
            if(Steel_flag)
            {
                if(++steel==Steel_flag)
                {
                    JiXin(JX);
                    steel=0;
                    JX^=1;
                }
            }
            ldv=positiond-ld/10;
            F12_flag=4;
            ia=2;
            if(!P[3]||P[10]>1)
            {
                Open_OCSF();
            }
            ii=Voltage();
            ii+=Voltage();
            ii+=Voltage();
            ii+=Voltage();
            vol=ii/4;
            if(vol>0xfe-10)
            {
                vol=0xfe-10;
            }
            if(!ERR_XY)
            {
                Position_control_z(ldv);
            }
        }
        break;
    case 9:
        break;
    }
}
void outb(int portid,unsigned char valud)
{
    disable();
    outportb(portid,valud);
    enable();
    Delay1(DELAY);
}
unsigned char inb(int portid)
{
    unsigned char a;
    disable();
    a=inportb(portid);
    enable();
    Delay1(DELAY);
    return a;
}
void InitKey(char flag)
{
    struct REGPACK iintr;
    disable();
    outb(0x64,0xad);
    outb(0x60,0xff);
    delay(5);
    outb(0x64,0xae);
    enable();
    if(flag)
    {
        return;
    }
}
GetTime(long Time)
{
    long ll;
    long far * pi;
    char la;
    pi=MK_FP(0x40,0x6c);
    ll=*pi;
    if(ll<Oldtime)
    {
        if(ll>=Oldtime+Time-0x1800B0)
        {
            Oldtime=ll;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(ll>=Oldtime+Time)
        {
            Oldtime=ll;
            return 1;
        }
        else
        {
            return 0;
        }
    }
}
int ProTime(void)
{
    static long far *pi;
    static long OLDTIME,OLDST,px=0,py=0,pz=0,px1=0,py1=0,pz1=0;
    static long PROTIME;
    static int i=0,j=0;
    static int kx=0,ky=0,kz=0;
    static int T00=0;
    long ll,l1=0;
    int ii;
    unsigned int m,k;
    CheckDate();
    if(!i)
    {
        pi=MK_FP(0x40,0x6c);
        i=1;
        disable();
        OLDTIME=*pi;
        enable();
        if(OLDTIME>=0xE0066)
        {
            T00=1;
        }
        else
        {
            T00=0;
        }
        PROTIME=OLDTIME;
        l1=GetDate();
        ii=Cmp(OldDATE,l1);
        m=N0TN(OldDATE);
        k=m/372;
        k+=1980;
        if(ii<0||k>2099)
        {
            WriteSPI(0x2a,l1&0xff);
            WriteSPI(0x2b,l1>>8);
            OldDATE=l1;
            l1^=*pi;
            OldID+=l1;
            WriteSPI(0x28,OldID&0xff);
            WriteSPI(0x29,OldID>>8);
        }
        if(ii>0)
        {
            PassWordF&=0x20;
        }
        return 1;
    }
    else
    {
        outb(OSC,0xbf);
        outportb(OSC+3,0x3d);
        disable();
        ll=*pi;
        enable();
        if(!T00&&ll>=0xE0066)
        {
            i=0;
        }
        if(ll<OLDTIME)
        {
            OLDTIME=ll;
            i=0;
        }
        if(ll>OLDTIME)
        {
            l1=ll-OLDTIME;
        }
        else
        {
            l1=0;
        }
        if(l1>120)
        {
            l1=120;
        }
        if(l1)
        {
            if(ProtectTime<11000)
            {
                ProtectTime+=l1;
            }
            OLDTIME=ll;
        }
    }
    j+=l1;
    if(j>=1)
    {
        j=0;
        pz=position_z-offset_z;
        py=position_y-offset_y;
        px=position_x-offset_x;
        if(px!=px1)
        {
            ShowPZ(1);
            px1=px;
            kx=1;
        }
        else
        {
            ShowPZ(1);
        }
        if(py!=py1)
        {
            ShowPZ(2);
            py1=py;
            ky=1;
        }
        else
        {
            ShowPZ(2);
        }
        if(pz!=pz1)
        {
            ShowPZ(3);
            pz1=pz;
            kz=1;
        }
        else
        {
            ShowPZ(3);
            if(ExitTime>36)
            {
                ExitTime=36;
                if(F[1])
                {
                    F2();
                }
            }
        }
        if(FS&&FS<0x800)
        {
            if(FS<=l1)
            {
                FS=0;
                outb(IO+4,IO4&=0x7f);/*关风扇*/
            }
            else
            {
                FS-=l1;
            }
        }
        if(ExitTime)
        {
            ShowMess(31);
            if(ExitTime<=l1)
            {
                CloseAllPower();
            }
            else
            {
                ExitTime-=l1;
            }
        }
        if(SoundTime>0)
        {
            if(!(V0ING)||K[3]) /*no 短路*/
            {
                if(OLDST!=SoundTime)
                {
                    if(SoundTime<SoundTIME)
                    {
                        l1=0;
                    }
                    if(Sound_flag)
                    {
                        CloseSound();
                    }
                }
                if(!Sound_flag&&l1)
                {
                    OpenSound();
                }
                SoundTime-=l1;
                if(!SoundTime)
                {
                    SoundTime--;
                }
                OLDST=SoundTime;
            }
            else
            {
                OpenSound();
            }
        }
        else if(SoundTime<0)
        {
            SoundTime=0;
            CloseSound();
        }
        if(OLDTIME>PROTIME)
        {
            PROTIME=OLDTIME;
            if(SMessage>0)
            {
                SMessage-=l1;
                if(SMessage<=0)
                {
                    ShowMess(0xff);
                }
            }
            if(Dis_flag)
            {
                if(AI)
                {
                    if(AITime<30000)
                    {
                        AITime+=l1;
                    }else{
                        AITime=0;
                    }
                    if(AI==1&&((AITime>AI_time*18&&P[5]&&AIHigh*DZC>50)
                               ||(AIHigh*DZC>(AI_high+50)&&P[6]&&AITime>182)))
                    {
                        if(P[7]==1)
                        {
                            ClearAI(0);
                        }
                        if(P[7]==2)
                        {
                            F11();
                            ShowTB(0xa);
                        }
                        AITime=0;
                    }
                }

                if(AI==2)
                {
                    ClearAI(2);
/*
                    if(AITime>60*18)
                    {
                        F12_flag=0;
                    }
*/
                }

            }
        }
        if(PROTIME>OLDTIME)
        {
            PROTIME=OLDTIME;
            i=0;
        }
        return 1;
    }
    return 0;
}
void ClearAI(int flag)
{
    int i=0,j=0;
    if(!flag)
    {
        IO3&=0xfc;
        outb(IO+3,IO3);           /* 停高压V */
        if(GaoyaV==0){IO3|=0x00;}
        if(GaoyaV==1){IO3|=0x00;}
        if(GaoyaV==2){IO3|=0x00;}
        if(GaoyaV==3){IO3|=0x01;}
        outb(IO+3,IO3);          /* 高压V */

        outb(OSC,0x02);
        outb(OSC+2,(Table.Jianxi[Dis_lines])+10);   /* 间隙电压 */
        AI=2;
    }
    if(flag==1)
    {
        SetTable(Dis_lines);
        AI=0;
        AIHigh=0;
        AITime=0;
    }

    if(flag==2)
    {

        if(AIHigh*DZC>=50 && AIHigh*DZC<=(1*AI_high+50)){
            Work_time=(Table.Gongshi[Dis_lines]*4)/5;
            j=Table.Xiuzhi[Dis_lines]+(Table.Xiuzhi[Dis_lines]*1)/5;
        }
        else if(AIHigh*DZC>=(1*AI_high+50) && AIHigh*DZC<=(2*AI_high+50)){
            Work_time=(Table.Gongshi[Dis_lines]*3)/5;
            j=Table.Xiuzhi[Dis_lines]+(Table.Xiuzhi[Dis_lines]*2)/5;
        }
        else if(AIHigh*DZC>=(2*AI_high+50) && AIHigh*DZC<=(3*AI_high+50)){
            Work_time=(Table.Gongshi[Dis_lines]*2)/5;
            j=Table.Xiuzhi[Dis_lines]+(Table.Xiuzhi[Dis_lines]*3)/5;
        }
        else{
            Work_time=(Table.Gongshi[Dis_lines]*1)/5;
            j=Table.Xiuzhi[Dis_lines]+(Table.Xiuzhi[Dis_lines]*4)/5;
        }

        i=Table.Maikuan[Dis_lines];
        Duty_ratio(i,j,0);          /* 脉冲 */
        MianJi=(1); /* 面积 */
    }
}
unsigned NTN0(unsigned ii)
{
    register unsigned i,j;
    i=ii;
    j=i;
    j<<=13;
    i>>=3;/*i&=0x1fff;*/
    i|=j;
    i^=0x55aa;
    j=i;
    i>>=7;/*i&=0x1ff;*/
    j<<=9;
    i|=j;
    return i^0xaa55;
}
unsigned N0TN(unsigned ii)
{
    register unsigned i,j;
    i=ii;
    if(!i)
    {
        return i;
    }
    i^=0xaa55;
    j=i;
    i<<=7;
    j>>=9;
    i|=j;
    i^=0x55aa;
    j=i;
    i<<=3;
    j>>=13;
    i|=j;
    return i;
}
void SetDianliu(unsigned dianliu)
{
    unsigned ib;
    ib=dianliu;
    SDL=ib;
    if(sl5==0)
    {
        if(ib>63)
        {
            ib=63;
        }
    }
    else
    {
        if(ib>127)
        {
            ib=127;
        }
    }
    IO1=ib;
    outb(IO+1,IO1);     /* 电流 */
    if(((Table.Dianliu[Dis_lines]>7&&Table.LV[Dis_lines]<2)||
            (Table.Dianliu[Dis_lines]>3&&Table.LV[Dis_lines]>1))&&Dis_flag)
    {
        outb(IO+4,IO4|=0x80);/*>4A开风扇*/
        FS=0x1000;
    }
    else
    {
        FS=1092;
    }
}
int CheckDate()
{
    unsigned char a0;
    outb(OSC,0xdf);
    a0=inportb(OSC+1);
    if(!(a0&0x01) && dis_time >= 0)
    {
        DIS_OK=1;
        outb(OSC,0xdf);
        outportb(OSC+3,0x7c);
    }
    else
    {
        DIS_OK=0;
    }
}
int Cmp(unsigned i1,unsigned i2)
{
    static ii=0;
    register unsigned i,j;
    i=i1;
    j=i2;
    ii++;
    if(!i||!j)
    {
        return -1;
    }
    if(i1==i2)
    {
        return 0;
    }
    if(ii>5)
    {
        outb(OSC,0xbf);
        outportb(OSC+3,0x3d);
        i2^=0x6a57;
        j=i2;
        j>>=1;
        i2<<=15;
        j|=i2;
        j^=0x7a;
        i2=j;
        if(ii==8)
        {
            ii=0;
            i1^=0x6a57;
            i=i1;
            i>>=1;
            i1<<=15;
            i+=i1;
            i^=0x7a;
            i1=i;
            i1=N0TN(i1);
            i2=N0TN(i2);
            if(i1+1000==i2)
            {
                PassWordF|=0x40;
                return 0;
            }
            if(i1+1000<i2)
            {
                return -1;
            }
            if(i1+1234<i2||i1>i2)
            {
                return 2;
            }
            return 1;
        }
        i1=N0TN(i1);
        i2=N0TN(i2);
        if(i1>i2)
        {
            return 1;
        }
        if(i1==i2)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    i1=N0TN(i1);
    i2=N0TN(i2);
    if(ii==4)
    {
        if(i1+100<i2)
        {
            return -1;
        }
        if(i1>i2)
        {
            return -1;
        }
        return 1;
    }
    if(i1>i2)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
void SetTable(char lines)
{
    unsigned char ib;

    if(Table.Index[lines]!=1||lines>=15)
    {
        return;
    }

    AI=0;
    JX=Table.Jixin[lines];
    position_p=Table.Shendu[lines];
    if(position_p<-9999999/DZC)
    {
        position_p=-9999999/DZC;
    }
    if(position_p>9999999/DZC)
    {
        position_p=9999999/DZC;
    }
    position_pz=position_p;
    if(Table.Jixin[lines]==1&&Dis_flag)  /* 极性- */
    {
        outb(IO+0,IO0|=0x20);
        outb(IO+4,IO4&=0xef);
        outb(IO+4,IO4|=0x20);
    }
    else
    {
        outb(IO+0,IO0&=0xdf);
        outb(IO+4,IO4&=0xdf);
        outb(IO+4,IO4|=0x10);
    }
    SetDianliu(Table.Dianliu[lines]);   /* 电流 */
    ib=Table.Dianliu[lines];
    Duty_ratio(Table.Maikuan[lines],Table.Xiuzhi[lines],0);             /* 脉冲 */
    outb(OSC,0x02);
    outb(OSC+2,(Table.Jianxi[Dis_lines]));  /* 间隙电压 */
    MianJi=(Table.Shenggao[lines]/100)*10;        /* 面积 */
    outb(OSC,0x11);
    outb(OSC+1,MianJi);
    outb(OSC+2,MianJi>>8);
    Up_time=(Table.Shenggao[lines]%100)*100;   /* 升高 */
    outb(OSC,0x12);
    outb(OSC+1,Up_time);
    outb(OSC+2,Up_time>>8);
    Work_time=Table.Gongshi[lines];  /* 工时 */
    outb(OSC,0x10);
    outb(OSC+1,50);
    outb(OSC+2,Work_time);
    Elect_speed1=Table.Sudu[lines]-1;       /* 速度 */
    Elect_speed1&=0x07;
    OTPS0&=0xf8;
    OTPS0|=Elect_speed1;
    outb(OSC,0x01);
    outb(OSC+1,OTPS0);

    SGY=Table.Gaoya[lines]/10;
    GaoyaV=Table.Gaoya[lines]/10;
    IO3&=0xfc;
    outb(IO+3,IO3);           /* 停高压V */
    if(GaoyaV==0)
    {
        IO3|=0x00;
        Gaoya=160;
    }
    if(GaoyaV==1)
    {
        IO3|=0x00;
        Gaoya=160;
    }
    if(GaoyaV==2)
    {
        IO3|=0x01;
        Gaoya=200;
    }
    if(GaoyaV==3)
    {
        IO3|=0x03;
        Gaoya=260;
    }
    outb(IO+3,IO3);                     /* 高压V */

    GaoyaA=Table.Gaoya[lines]%10;
    IO3&=0xf3;
    outb(IO+3,IO3);           /* 停高压A */
    if(GaoyaA==0)
    {
        IO3&=0xf3;
    }
    if(GaoyaA==1)
    {
        IO3|=0x04;
    }
    if(GaoyaA==2)
    {
        IO3|=0x08;
    }
    if(GaoyaA==3)
    {
        IO3|=0x0c;
    }
    outb(IO+3,IO3);                     /* 高压A */

    Short=Table.PP[lines]%100/10;              /* 短路切断（十位）*/
    if (Short==1)
    {
        outb(OSC,0x01);
        outb(OSC+2,OTPS1|=0x20);
    }
    else
    {
        outb(OSC,0x01);
        outb(OSC+2,OTPS1&=0xdf);
    }
    Clip=Table.PP[lines]/100;            /* 削波（百位） */
    if (Clip==1)
    {
        outb(OSC,0x01);
        outb(OSC+2,OTPS1|=0x10);
    }
    else
    {
        outb(OSC,0x01);
        outb(OSC+2,OTPS1&=0xef);
    }
    Other=Table.PP[lines]%100%10;           /* 等脉冲(个位) */
    if (Other==1)
    {
        outb(OSC,0x01);
        outb(OSC+2,OTPS1|=0x40);
    }
    else
    {
        outb(OSC,0x01);
        outb(OSC+2,OTPS1&=0xbf);
    }
    WcL=Table.WcLc[lines]/10;               /* LC */
    IO2&=0x1f;
    outb(IO+2,IO2);
    if(WcL==0)
    {
        outb(IO+2,IO2&=0x1f);
    }
    if(WcL==1)
    {
        outb(IO+2,IO2|=0x80);
    }
    if(WcL==2)
    {
        outb(IO+2,IO2|=0x20);
    }
    if(WcL==3)
    {
        outb(IO+2,IO2|=0x40);
    }
    WcC=Table.WcLc[lines]%10;                               /* 硬质合金 */
    IO2&=0xe0;
    outb(IO+2,IO2);
    switch(WcC)
    {
    case 0:
        IO2&=0xe0;
        break;
    case 1:
        IO2|=0x01;
        break;
    case 2:
        IO2|=0x02;
        break;
    case 3:
        IO2|=0x04;
        break;
    case 4:
        IO2|=0x08;
        break;
    case 5:
        IO2|=0x10;
        break;
    case 6:
        IO2|=0x12;
        break;
    case 7:
        IO2|=0x14;
        break;
    case 8:
        IO2|=0x18;
        break;
    case 9:
        IO2|=0x1f;
        break;
    default:
        break;
    }
    outb(IO+2,IO2);
}
void JiXin(int JX1)
{
    if(!Dis_flag)
    {
        return ;
    }
    if(JX1==1)
    {
        outb(IO+0,IO0|=0x20);
        outb(IO+4,IO4&=0xef);
        outb(IO+4,IO4|=0x20);
    }
    else
    {
        outb(IO+0,IO0&=0xdf);
        outb(IO+4,IO4&=0xdf);
        outb(IO+4,IO4|=0x10);
    }
}
void Duty_ratio(unsigned maikuan,unsigned xiuzhi,unsigned clk0)
{
    if(!clk0)
    {
        switch(maikuan)
        {
        case 991:
            maikuan=1000;
            break;
        case 992:
            maikuan=1250;
            break;
        case 993:
            maikuan=1500;
            break;
        case 994:
            maikuan=1750;
            break;
        case 995:
            maikuan=2000;
            break;
        case 996:
            maikuan=2250;
            break;
        case 997:
            maikuan=2500;
            break;
        case 998:
            maikuan=2750;
            break;
        case 999:
            maikuan=3000;
        }
        maikuan*=4;
        xiuzhi*=4;
        outb(OSC,0x08);
        outb(OSC+1,xiuzhi&0xff);
        outb(OSC+2,xiuzhi>>8);
        outb(OSC,0x04);
        outb(OSC+1,maikuan&0xff);
        outb(OSC+2,maikuan>>8);
        MX=maikuan;
        MX/=(maikuan+xiuzhi);
    }
}
void Delay1(unsigned mill)
{
    delay(mill>>3);
}
void Init_coordinate(void)
{
    sl0=ReadSPI(0x10);
    sl1=ReadSPI(0x11);
    sl2=ReadSPI(0x12);
    sl3=ReadSPI(0x13);
    sl4=ReadSPI(0x14);
    sl5=ReadSPI(0x15);
    sl7=ReadSPI(0x17);
    sl8=ReadSPI(0x18);
    sl9=ReadSPI(0x19);
    if(sl0==0)
    {
        DZC=1;
    }
    else
    {
        DZC=5;
    }
    if(sl1==1)
    {
        IOX0&=0xef;
    }
    else
    {
        IOX0|=0x10;
    }
    outb((ICXaddr),0x01);
    outb((ICXaddr+1),IOX0);
    if(sl2==1)
    {
        IOY0&=0xef;
    }
    else
    {
        IOY0|=0x10;
    }
    outb((ICYaddr),0x01);
    outb((ICYaddr+1),IOY0);
    if(sl3==1)
    {
        IOZ0&=0xef;
    }
    else
    {
        IOZ0|=0x10;
    }
    outb((Address_Z),0x01);
    outb((Address_Z+1),IOZ0);
    soundV=ReadSPI(0x1c);
    if(soundV>7)
    {
        soundV=7;
    }
    Opens_V(soundV);
    lightM=ReadSPI(0x1d);
    if(lightM>7)
    {
        lightM=7;
    }
    Opens_M(lightM);
    full=ReadSPI(0x0f);
    outb(OSC,0x30);
    outb(OSC+3,full);
    if(sl0>1)
    {
        sl0=0;
    }
    if(sl1>1)
    {
        sl1=0;
    }
    if(sl2>1)
    {
        sl2=0;
    }
    if(sl3>1)
    {
        sl3=0;
    }
    if(sl4>1)
    {
        sl4=0;
    }
    if(sl5>1)
    {
        sl5=0;
    }
    if(sl7>5||sl7<4)
    {
        sl7=4;
    }
    if(sl8>3||sl8<2)
    {
        sl8=2;
    }
    if(sl9>1||sl9<0)
    {
        sl9=0;
    }
    StrTable.Shendu[0]/=DZC;
    StrTable.Shendu[1]/=DZC;
    StrTable.Shendu[4]/=DZC;
    position_x/=DZC;
    position_y/=DZC;
    position_p/=DZC;
    position_d/=DZC;
    KEYL=11;
}
void set_PID(void)
{
    outb(Address_Z,0x02);
    outb((Address_Z+1),Pid_kp);/*输入kp*/
    outb(Address_Z,0x02);
    outb((Address_Z+2),Pid_ki);/*输入ki*/
    outb(Address_Z,0x02);
    outb((Address_Z+3),Pid_kd);/*输入kd*/
    outb(Address_Z,0x03);
    outb((Address_Z+1),Pid_st);/*输入st*/
}
void set_PIDX(void)
{
    outb(ICXaddr ,0x02);
    outb((ICXaddr +1),Pid_kp);/*输入kp*/
    outb(ICXaddr ,0x02);
    outb((ICXaddr +2),Pid_ki);/*输入ki*/
    outb(ICXaddr ,0x02);
    outb((ICXaddr +3),Pid_kd);/*输入kd*/
    outb(ICXaddr ,0x03);
    outb((ICXaddr +1),Pid_st);/*输入st*/
}
void set_PIDY(void)
{
    outb(ICYaddr ,0x02);
    outb((ICYaddr +1),Pid_kp);/*输入kp*/
    outb(ICYaddr ,0x02);
    outb((ICYaddr +2),Pid_ki);/*输入ki*/
    outb(ICYaddr ,0x02);
    outb((ICYaddr +3),Pid_kd);/*输入kd*/
    outb(ICYaddr ,0x03);
    outb((ICYaddr +1),Pid_st);/*输入st*/
}
void SetParameter(void)/*设置系统参数*/
{
    WriteSPI(0x10,sl0);
    WriteSPI(0x11,sl1);
    WriteSPI(0x12,sl2);
    WriteSPI(0x13,sl3);
    WriteSPI(0x14,sl4);
    WriteSPI(0x15,sl5);
    if(sl4==1)
    {
        Write_spi(0x38,0);
    }
    Write_spi(0x3c,0);
}
void Return_origin(void)/* 归原点*/
{
    char temp[10],i;
    unsigned char b;
    long ll;
    long Z_Rvise=0,lrz;
    WriteSPI(0x17,sl7);
    WriteSPI(0x18,sl8);
    WriteSPI(0x19,sl9);
    strcpy(temp,StrLN7(sl7));
    ORgin=0x80;
    if(!SoundTime)
    {
        CloseSound();
    }
    lrz=ZPosi(0);
    if(temp[0]=='Z'&&temp[1]=='+')/*正向*/
    {
        Velocity_control_z(0x800);
        outb(Address_Z,0x01);
        b=inb(Address_Z+3);
        while(!(b&0x80))
        {
            if(KeyPress())
            {
                key();
            }
            else
            {
                CloseSound();
            }
            oz=ZPosi(0);
            position_z=ZPosi(0)+offset_z;
            ShowXYZ(0);
            ShowPZ(0);
            outb(Address_Z,0x01);
            b=inb(Address_Z+3);
            if(Voltage()<=10)
            {
                return;
            }
        }
    }
    if(temp[0]=='Z'&&temp[1]=='-')/*负向*/
    {
        Velocity_control_z(0xf800);
        outb(Address_Z,0x01);
        b=inb(Address_Z+3);
        while(!(b&0x40))
        {
            if(KeyPress())
            {
                key();
            }
            else
            {
                CloseSound();
            }
            oz=ZPosi(0);
            position_z=ZPosi(0)+offset_z;
            ShowXYZ(0);
            ShowPZ(0);
            outb(Address_Z,0x01);
            b=inb(Address_Z+3);
            if(Voltage()<=10)
            {
                return;
            }
        }
    }
    Stop_z(0);
    ORgin+=0x40;
    if(ZPosi_origin(0))
    {
        lpz=ZPosi_origin(0);
        Z_Rvise=s_OZh-s_OZl+lpz;
        Position_control_z(lpz);
        offset_rz=Z_Rvise;
        ORgin+=1;
    }
    else
    {
        offset_rz=lrz;
    }
}
void Show_hint(void)
{
    unsigned char b,k;
    static int i=0;
    outb(Address_Z,0x01);
    b=inb(Address_Z+3);
    outb(OSC,0xdf);
    k=inportb(OSC+1);
    if((k&0x2)&&(!(b&0x80))&&(!(b&0x40))&&(DIS_OK==1))
    {
        ShowMess(0x94);
        i=1;
    }
    if(((b&0x80)||(b&0x40)||(DIS_OK!=1)||!(k&0x2))&&i)
    {
        setfillstyle(1,0x8);
        bar(CX0+1,CY0+18,CX0+160,CY0+35);
        List16x16M(CX0+1,CY0+27,0xf,"");
        i=0;
    }
}
