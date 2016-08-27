#include "znc.h"
#include <stdlib.h>
#include <graphics.h>
#include <dos.h>
void X();
void Y();
void Z();
void SWF();
void DelLine();
void SaveF7V();
void ClearF7();
void SetPass(char,int);
void ErrXY();
void Key(void)
{
    static char MaxL1;
    static long MaxN1,MinN1;
    static char KXX=-1;
    int i,j,k;
    long l,l1,l2,l3;
    int UDK,UDK1;
    char Temp_1[3],Temp_2[3],Temp_3[3];
    if((ORgin&0x80)&&KeyPress())
    {
        KeyAss();
        k=0;
        SoundTime=SoundTIME;
        OpenSound();
        Sound_flag=0;
        ShowMess(15);
        return ;
    }
    outb(OSC,0x01);
    UDK=inb(OSC+3);
    outb(Address_Z,0x01);
    UDK1=inb(Address_Z+3);
    if(!(UDK&0x04))/* SWF 三种状态转换 */
    {
        if(!SWF_K)
        {
            SoundTime=SoundTIME;
            SWF();
        }
        if(SWF_K<=40)
        {
            SWF_K++;
        }
        if(SWF_K==40)
        {
            OpenScr();
        }
    }
    else
    {
        SWF_K=0;
    }
    if(!DNUP)
    {
        if(!(UDK1&0x10))
        {
            DNUP=1;    /*DN*/
        }
        if(!(UDK1&0x20))
        {
            DNUP=2;    /*UP*/
        }
    }
    else
    {
        if(UDK1&0x20)
        {
            DNUP=0;
        }
    }
    if(DN_K)
        if(UDK1&0x10)
        {
            if(DN_K>=UDKN)
            {
                Stop_z(SWF_flag+1);
            }
            else
            {
                Stop_z(-1);
            }
            DN_K=0;
            UP_K=0;
        }
    if(UP_K)
        if(UDK1&0x20)
        {
            if(UP_K>=UDKN)
            {
                Stop_z(SWF_flag+1);
            }
            else
            {
                Stop_z(-1);
            }
            UP_K=0;
            DN_K=0;
        }
    if((F[4]<5||F[4]>6)&&!(UDK1&0x10)&&(UDK1&0x20||DNUP==2))
    {
        if(ORgin&0x80)
        {
            ShowMess(15);
            return ;
        }
        if(Dis_flag&&K[8])
        {
            F11();    /* DN 键  */
        }
        if(UDK1&0x20)
        {
            DNUP=1;
        }
        Down();
    }
    else if((F[4]<5||F[4]>6)&&!(UDK1&0x20))
    {
        if(ORgin&0x80)
        {
            ShowMess(15);
            return ;
        }
        if(UDK1&0x01)
        {
            DNUP=2;    /* UP 键  */
        }
        if(Dis_flag&&!K[8])
        {
            F11();
        }
        Up();
    }
    if(!Dis_flag)if(!(UDK&0x10))
        {
            SoundTime=SoundTIME;
            if(ORgin&0x80)
            {
                ShowMess(15);
                return ;
            }
            if(!F[1])
            {
                ShowMess(15);
            }
            else
            {
                ZeroFlag=0;
                F12_flag=0;
                Dis_flag=1;
                lock_z=0;
                DN_K=0;
                UP_K=0;
                ShowKey(0xff,0xff);
                ClearKey(0);
                position_xdis=position_x;
                position_ydis=position_y;
            }
        }/* DIS键  */
    if(!(UDK&0x20)&&!F11_flag)
    {
        SoundTime=SoundTIME;    /* 停止开关 */
        F11();
        if(!Dis_flag&&V<10)
        {
            lock_z_stop=20;
            ShowMess(54);
        }
        if(ORgin&0x80)
        {
            ShowMess(15);
            return ;
        }
    }
    if(UDK&0x20)
    {
        F11_flag=0;
    }
 		if(!(UDK&0x08) && PUMP_K>=0)
    {
        if(PUMP_K<5) PUMP_K++;
        else
        {
        	PUMP_K = -1;
					SoundTime=SoundTIME;
          F3();
          if(KEYL==0) ShowKey(1,2);
        }
    }
    else PUMP_K=0;
    /* 油泵开关 */
    if(KeyPress())
    {
        if(Ck8255_flag&0x80)
        {
            Ck8255_flag&=0x7f;
            ShowTB(0x17);
        }
        if(Ck8256_flag&0x80)
        {
            Ck8256_flag&=0x7f;
        }
        ShowTB(0x1d);
        if(Ck8255_flag&0x40)
        {
            Ck8255_flag&=0xbf;
            ShowTB(0x18);
        }
        if(Scr_flag)
        {
            SoundTime=SoundTIME;
            return;
        }
        k=KeyAss();
        while(KeyPress())
        {
            KeyAss();
        }
        SoundTime=SoundTIME;
        OpenSound();
        Sound_flag=0;
        if(Fx&&k==0x52)
        {
            K11();
            outb(OSC,0x01);
            OTPS0&=0x7f;
            outportb(OSC+1,OTPS0);
        }
#if TESTING
        if(k=='S' || k == 's')
        {
            K12();
        }
        if(Fx&&k==0x47)
        {
            SWF_K++;
            SWF();
            return;
        }
        if(Fx&&k==0x49)
        {
            Velocity_control_z(0x7fff);
        }
        if(Fx&&k==0x51)
        {
            Velocity_control_z(0x8000);
        }
        if(Fx&&k==0x4F)
        {
            Stop_z(0);
        }
#endif
        if((Fx&&k==aF11)||(!Fx&&(k=='P'||k=='p')))
        {
            F11();
            if(!Dis_flag&&V<10)
            {
                lock_z_stop=20;
                ShowMess(54);
            }
        }
        if(Fx&&k==aF12&&!Dis_flag)
        {
            if(!F[1]||F[6]>1||KEYL==6)
            {
                ShowMess(15);
                return;
            }
            else
            {
                ClearKey(0);
                F12_flag=0;
                Dis_flag=1;
                lock_z=0;
                DN_K=0;
                UP_K=0;
                ShowKey(0xff,0xff);
                position_xdis=position_x;
                position_ydis=position_y;
            }
        }
        if(!Fx&&k==0xd)
        {
            GetInput();
        }
        if(!Fx)
        {
            if(!XYZ[0]&&!XYZ[1]&&!XYZ[2])
            {
                switch(k)
                {
                case 'x':
                case 'X':
                {
                    MaxL1=MaxL;
                    MaxN1=MaxN;
                    MinN1=MinN;
                }
                ShowMess(0xfd);
                X();
                break;
                case 'y':
                case 'Y':
                {
                    MaxL1=MaxL;
                    MaxN1=MaxN;
                    MinN1=MinN;
                }
                ShowMess(0xfd);
                Y();
                break;
                case 'z':
                case 'Z':
                {
                    MaxL1=MaxL;
                    MaxN1=MaxN;
                    MinN1=MinN;
                }
                ShowMess(0xfd);
                Z();
                break;
                }
            }
            else
            {
                i=XYZ[0]|XYZ[1]|XYZ[2];
                switch(k)
                {
                case 'x':
                case 'X':
                    if(XYZ[1])
                    {
                        Y();
                    }
                    if(XYZ[2])
                    {
                        Z();
                    }
                    X();
                    break;
                case 'y':
                case 'Y':
                    if(XYZ[0])
                    {
                        X();
                    }
                    if(XYZ[2])
                    {
                        Z();
                    }
                    Y();
                    break;
                case 'z':
                case 'Z':
                    if(XYZ[0])
                    {
                        X();
                    }
                    if(XYZ[1])
                    {
                        Y();
                    }
                    Z();
                    break;
                case 0xd:
                    if(XYZ[0])
                    {
                        X();
                    }
                    if(XYZ[1])
                    {
                        Y();
                    }
                    if(XYZ[2])
                    {
                        Z();
                    }
                    break;
                default:
                    ShowInput(k);
                }
                if(i&&!XYZ[0]&&!XYZ[1]&&!XYZ[2])
                {
                    ClearKey(0);
                    MaxL=MaxL1;
                    MaxN=MaxN1;
                    MinN=MinN1;
                    ShowMess(0xfe);
                }
                return;
            }
        }
        else
        {
            if(KEYL==1&&k==aF4)
            {
                F6();
            }
            else  if(XYZ[0]||XYZ[1]||XYZ[2])
            {
                if(XYZ[0])
                {
                    X();
                }
                if(XYZ[1])
                {
                    Y();
                }
                if(XYZ[2])
                {
                    Z();
                }
                ClearKey(0);
                MaxL=MaxL1;
                MaxN=MaxN1;
                MinN=MinN1;
                ShowMess(0xfe);
            }
        }
        switch(KEYL)
        {
        case 0:
            if(Fx)    /*主菜单*/
            {
                switch(k)
                {
                case aF1:
                    F2();
                    ShowKey(1,1);
                    break;
                case aF2:
                    if(!Pumn_flag)
                    {
                        F3();
                        ShowKey(1,2);
                    }
                    else
                    {
                        F[2]=0;
                        ShowKey(1,2);
                        ShowMess(15);
                    }
                    break;
                case aF3:
                    KEYL=1;
                    ShowKey(2,0);
                    break;
                case aF4:
                    KEYL=2;
                    ShowKey(3,0);
                    break;
                case aF5:
                    KEYL=3;
                    ShowKey(4,0);
                    break;
                case aF6:
                    KEYL=4;
                    ShowKey(5,0);
                    break;
                case aF7:
                    K9();
                    ShowKey(1,7);
                    break;
                }
                return;
            }
            break;
        case 1: /*坐标*/
            if(Fx)
            {
                if(k!=aF2&&F[8])
                {
                    F9();
                    ShowKey(2,2);
                }
                if(k!=aF3&&F[9])
                {
                    F10();
                    ShowKey(2,3);
                }
                switch(k)
                {
                case aF1:
                    F5();
                    ShowKey(2,1);
                    break;
                case aF2:
                    F9();
                    ShowKey(2,2);
                    break;
                case aF3:
                    F10();
                    ShowKey(2,3);
                    break;
                case aF4:
                    F6();
                    ShowKey(2,4);
                    break;
                case aF5:
                    K4();
                    ShowKey(2,5);
                    ShowKey(2,2);
                    break;
                case aF6:
                    if(!Dis_flag&&!Origin_Flag)
                    {
                        KEYL=11;
                        DispF10(0);
                        ShowKey(7,0);
                    }
                    else
                    {
                        if(Dis_flag)
                        {
                            ShowMess(15);
                        }
                        else
                        {
                            if(Origin_Flag)
                            {
                                ShowMess(48);
                            }
                        }
                    }
                    break;
                case aF8:
                    KEYL=0;
                    ShowKey(1,0);
                    break;
                }
                return;
            }
            if(!Fx&&k==0xd)
            {
                if(F[8])
                {
                    F9();
                }
                if(F[9])
                {
                    F10();
                }
                ShowKey(2,0);
            }
            break;
        case 2: /*放电*/
            if(Fx)
            {
                if(k!=aF2&&F[8])
                {
                    F9();
                    ShowKey(3,2);
                }
                if(k!=aF3&&F[9])
                {
                    F10();
                    ShowKey(3,3);
                }
                if(k!=aF4&&K[5])
                {
                    K6();
                    ShowKey(3,4);
                }
                if(k!=aF5&&F[7]&&F[7]!=3)
                {
                    F8(0);
                    ShowKey(3,5);
                }
                if(k!=aF6&&P[8]==1)
                {
                    P9();
                    ShowKey(3,6);
                }
                switch(k)
                {
                case aF1:
                    KEYL=5;
                    KXX=-1;
                    ShowKey(6,0);
                    F7(0);
                    break;
                case aF2:
                    F9();
                    ShowKey(3,2);
                    break;
                case aF3:
                    F10();
                    ShowKey(3,3);
                    break;
                case aF4:
                    K6();
                    ShowKey(3,4);
                    break;
                case aF5:
                    F8(0);
                    ShowKey(3,5);
                    break;
                case aF6:
                    P9();
                    ShowKey(3,6);
                    break;
                case aF8:
                    KEYL=0;
                    ShowKey(1,0);
                    break;
                }
                return;
            }
            if(!Fx&&k==0xd)
            {
                if(F[8])
                {
                    F9();
                }
                if(F[9])
                {
                    F10();
                }
                if(F[7]<3&&F[7])
                {
                    F8(1);
                }
                if(K[5])
                {
                    K6();
                }
                if(P[8]==1)
                {
                    P9();
                }
                ShowKey(3,0);
            }
            break;
        case 3: /*安全*/
            if(Fx)
            {
                switch(k)
                {
                case aF1:
                    KEYL=9;
                    ShowKey(8,0);
                    break;
                case aF2:
                    P5();
                    ShowKey(4,2);
                    break;
                case aF3:
                    K5();
                    ShowKey(4,3);
                    break;
                case aF4:
                    P4();
                    ShowKey(4,4);
                    break;
                case aF5:
                    P3();
                    ShowKey(4,5);
                    break;
                case aF6:
                    K7();
                    ShowKey(4,6);
                    break;
                case aF8:
                    KEYL=0;
                    ShowKey(1,0);
                    break;
                }
                return;
            }
            break;
        case 4: /*系统*/
            if(Fx)
            {
                if(k!=aF3&&K[2]==1)
                {
                    K3();
                    ShowKey(5,3);
                }
                if(k!=aF5&&P[1])
                {
                    P2();
                    ShowKey(5,5);
                }
                if(k!=aF6&&P[9])
                {
                    P10();
                    ShowKey(5,6);
                }
                switch(k)
                {
                case aF1:
                    F4();
                    ShowKey(5,0);
                    break;
                case aF2:
                    K2();
                    ShowKey(5,2);
                    break;
                case aF3:
                    K3();
                    ShowKey(5,3);
                    break;
                case aF4:
                    K10();
                    ShowKey(5,4);
                    break;
                case aF5:
                    P2();
                    ShowKey(5,5);
                    break;
                case aF6:
                    P[14]=0;
                    P10();
                    ShowKey(5,6);
                    ShowKey(5,7);
                    break;
                case aF7:
                    P[9]=0;
                    P16();
                    ShowKey(5,7);
                    break;
                case aF8:
                    KEYL=0;
                    ShowKey(1,0);
                    break;
                }
                return;
            }
            if(!Fx&&k==0xd)
            {
                if(K[2]==1)
                {
                    K3();
                }
                if(P[1])
                {
                    P2();
                }
                if(P[9])
                {
                    P10();
                }
                if(P[14])
                {
                    P16();
                }
                if(KEYL==8||KEYL==10)
                {
                    ShowKey(7,0);
                }
                else
                {
                    ShowKey(5,0);
                }
            }
            break;
        case 5: /*参数*/
            if(Fx)
            {
                switch(k)
                {
                /*定段*/  case aF1  :
                    if(!Dis_flag)
                    {
                        Dis_lines=0;
                        Dis_start=0;
                        Dis_end=0;
                        ShowTable(0);
                        KEYL=7;
                        ShowKey(7,0);
                    }
                    else
                    {
                        ShowMess(15);
                    }
                    break;
                    /*自动*/
                case aF2  :
                    ShowInput(0);
                    if(!Dis_flag)
                    {
                        KEYL=6;
                        Y7Y=1;
                        ShowKey(7,0);
                        DispF7(0);
                        P[13]=1;
                    }
                    else
                    {
                        ShowMess(15);
                    }
                    return;
                    /*删除*/
                case aF3  :
                    if(!Dis_flag)
                    {
                        Table.Index[YY]=0;
                        DelLine();
                    }
                    else
                    {
                        ShowMess(15);
                    }
                    break;
                    /*修底*/
                case aF4  :
                    P11();
                    ShowKey(6,4);
                    break;
                case aF6  :
                    StrTable.Jianxi[0]=0;
                    break;
                case aF7  :
                    F7(1);
                    return;
                    /*返回*/
                case aF8  :
                    F7(0);
                    KEYL=2;
                    ShowKey(3,0);
                    StrTable.Jianxi[0]=20;
                    return;
                case LEFT :
                    if(!XX)
                    {
                        XX=13;
                    }
                    XX--;
                    if(XX==11||XX==12)
                    {
                        ShowMess(XX+1+43);
                    }
                    else
                    {
                        ShowMess(XX+1);
                    }
                    ClearF7();
                    break;
                case RIGHT:
                    XX++;
                    if(XX==13)
                    {
                        XX=0;
                    }
                    if(XX==11||XX==12)
                    {
                        ShowMess(XX+1+43);
                    }
                    else
                    {
                        ShowMess(XX+1);
                    }
                    ClearF7();
                    break;
                case UP   :
                    j=YY;
                    YY--;
                    if(YY==-1)
                    {
                        YY=0;/*while(Table.Index[YY]==1)YY++;*/
                    }
                    ShowTable(j+1);
                    ClearF7();
                    break;
                case DN   :
                    j=YY;
                    if(Table.Index[j]==1)
                    {
                        YY++;
                    }/*else YY=0;*/if(YY>14)
                    {
                        YY=14;
                    }
                    ShowTable(j+1);
                    ClearF7();
                    break;
                }
            }
            else
            {
                if(k==0xd) /*GetInput();*/
                {
                    SaveF7V();
                    XX++;
                    if(XX==13)
                    {
                        XX=0;
                    }
                    if(XX==11||XX==12)
                    {
                        ShowMess(XX+1+43);
                    }
                    else
                    {
                        ShowMess(XX+1);
                    }
                    ClearF7();
                }
                else
                {
                    if(XX==1)
                    {
                        Inputstr1[1]=0;
                    }
                    /*    ShowInput(k);*/
                }
            }
            ShowTable(YY+1);
            break;
        case 6: /*自动表格*/
            if(!Fx)
            {
                switch(k)
                {
                case 0xd:/*l=GetInput();*/
                    l=KeyN;
                    if(KeyN!=0x7fffffff)
                    {
                        switch(Y7Y)
                        {
                        case 1:
                            lDeep=l/DZC;
                            lDeep*=DZC;
                            break;
                        case 6:
                            iCurrent=l;
                            if(iCurrent<1)
                            {
                                iCurrent=1;
                            }
                            if(iCurrent>191)
                            {
                                iCurrent=191;
                            }
                            break;
                        case 7:
                            iAcreage=l;
                            if(iAcreage<1)
                            {
                                iAcreage=1;
                            }
                            if(iAcreage>99)
                            {
                                iAcreage=99;
                            }
                            break;
                        }
                    }
                    Fx=1;
                    k=DN;
                    /*default:ShowInput(k);*/
                }
            }
            if(Fx)
            {
                switch(k)
                {
                case DN:
                    l=GetInput();
                    ShowInput(0);
                    ShowF7(Y7Y);
                    if(l!=0x7fffffff)
                    {
                        switch(Y7Y)
                        {
                        case 1:
                            lDeep=l/DZC;
                            lDeep*=DZC;
                            break;
                        case 6:
                            iCurrent=l;
                            if(iCurrent<1)
                            {
                                iCurrent=1;
                            }
                            if(iCurrent>191)
                            {
                                iCurrent=191;
                            }
                            break;
                        case 7:
                            iAcreage=l;
                            if(iAcreage<1)
                            {
                                iAcreage=1;
                            }
                            if(iAcreage>99)
                            {
                                iAcreage=99;
                            }
                            break;
                        }
                    }
                    Y7Y++;
                    if(Y7Y==8)
                    {
                        Y7Y=1;
                    }
                    ShowF7(Y7Y);
                    break;
                case UP:
                    l=GetInput();
                    ShowInput(0);
                    ShowF7(Y7Y);
                    if(l!=0x7fffffff)
                    {
                        switch(Y7Y)
                        {
                        case 1:
                            lDeep=l/DZC;
                            lDeep*=DZC;
                            break;
                        case 6:
                            iCurrent=l;
                            if(iCurrent<1)
                            {
                                iCurrent=1;
                            }
                            if(iCurrent>191)
                            {
                                iCurrent=191;
                            }
                            break;
                        case 7:
                            iAcreage=l;
                            if(iAcreage<1)
                            {
                                iAcreage=1;
                            }
                            if(iAcreage>99)
                            {
                                iAcreage=99;
                            }
                            break;
                        }
                    }
                    if(Y7Y==1)
                    {
                        Y7Y=8;
                    }
                    Y7Y--;
                    ShowF7(Y7Y);
                    break;
                case RIGHT:
                    if(Y7Y==2)
                    {
                        cMaterial++;
                        if(cMaterial>3)
                        {
                            cMaterial=1;
                        }
                        ShowF7(2);
                    }
                    else if(Y7Y==3)
                    {
                        cEffect++;
                        if(cEffect>4)
                        {
                            cEffect=1;
                        }
                        ShowF7(3);
                    }
                    else if(Y7Y==4)
                    {
                        cShape++;
                        if(cShape>3)
                        {
                            cShape=1;
                        }
                        ShowF7(4);
                    }
                    else if(Y7Y==5)
                    {
                        cProcess++;
                        if(cProcess>2)
                        {
                            cProcess=1;
                        }
                        ShowF7(5);
                    }
                    break;
                case LEFT:
                    if(Y7Y==2)
                    {
                        cMaterial--;
                        if(cMaterial<1)
                        {
                            cMaterial=3;
                        }
                        ShowF7(2);
                    }
                    else if(Y7Y==3)
                    {
                        cEffect--;
                        if(cEffect<1)
                        {
                            cEffect=4;
                        }
                        ShowF7(3);
                    }
                    else if(Y7Y==4)
                    {
                        cShape--;
                        if(cShape<1)
                        {
                            cShape=3;
                        }
                        ShowF7(4);
                    }
                    else if(Y7Y==5)
                    {
                        cProcess--;
                        if(cProcess<1)
                        {
                            cProcess=2;
                        }
                        ShowF7(5);
                    }
                    break;
                case aF1:
                    ClearF7();
                    P[13]=0;
                    if(!DispF7(1))
                    {
                        KEYL=5;
                        ShowKey(6,0);
                    }
                    break;/*确定*/
                case aF2:
                case aF8:
                    ClearF7();
                    P[13]=0;
                    DispF7(2);
                    KEYL=5;
                    ShowKey(6,0);
                    break;/*取消*/
                }
                return;
            }
            break;
        case 7: /*定段*/
            if(Fx)
            {
                switch(k)
                {
                case UP   :
                    j=Dis_lines;
                    j++;
                    if(Dis_lines<0x10)
                    {
                        if(Dis_lines)
                        {
                            Dis_end=Dis_start=--Dis_lines;
                        }
                    }
                    else
                    {
                        if(Dis_lines>Dis_start+0x10)
                        {
                            Dis_end=(--Dis_lines)&0xf;
                        }
                    }
                    ShowTable(j&0xf);
                    break;
                case DN   :
                    j=Dis_lines;
                    j++;
                    if(Dis_lines<0x10)
                    {
                        if(Dis_lines<14)
                        {
                            Dis_end=Dis_start=++Dis_lines;
                        }
                    }
                    else
                    {
                        if(Dis_lines<14+0x10)
                        {
                            Dis_end=(++Dis_lines)&0xf;
                        }
                    }
                    ShowTable(j&0xf);
                    break;
                case aF1:
                    Fx=0;
                    k=0xd;
                    break;
                case aF2:
                case aF8:
                    Dis_start=0;
                    Dis_end=0;
                    Dis_lines=0;
                    KEYL=5;
                    ShowKey(6,0);
                    ShowTable(0);
                    return;
                    /*定段取消*/
                }
                ShowTable(1+Dis_lines&0xf);
            }
            if(!Fx)
            {
                switch(k)
                {
                case 0xd:
                    if(Dis_lines<0x10)
                    {
                        Dis_start=Dis_lines;
                        Dis_lines+=0x10;
                    }
                    else
                    {
                        Dis_lines&=0xf;
                        Dis_end=Dis_lines;
                        Dis_lines=Dis_start;
                        for(j=14; j; j--)if(Table.Index[j])
                            {
                                break;
                            }
                        if(Dis_end>j)
                        {
                            Dis_end=j;
                            ShowTable(0);
                        }
                        KEYL=5;
                        ShowKey(6,0);
                    }
                    break;
                }
                ShowTable(1+Dis_lines&0xf);
            }
            break;
        case 8: /*口令*/
            if(Fx)
            {
                switch(k)
                {
                case aF1:
                    SetPass(1,0xd);
                    break;
                case aF2:
                case aF8:
                    outb(OSC,0x01);
                    OTPS0&=0x7f;
                    outportb(OSC+1,OTPS0);
                    DispF8(3);
                    SetPass(2,0);
                    KEYL=4;
                    ShowKey(5,0);
                    if(!F[1])
                    {
                        F2();
                    }
                    break;
                default:
                    SetPass(1,0);
                }
            }
            else
            {
                SetPass(1,k);
            }
            break;
        case 9:
            if(Fx)
            {
                if(k!=aF1&&P[5]==1)
                {
                    P6();
                    ShowKey(8,1);
                }
                if(k!=aF2&&P[6]==1)
                {
                    P7();
                    ShowKey(8,2);
                }
                switch(k)
                {
                case aF1:
                    P6();
                    ShowKey(8,1);
                    break;
                case aF2:
                    P7();
                    ShowKey(8,2);
                    break;
                case aF3:
                    P8();
                    ShowKey(8,0);
                    break;
                case aF8:
                    KEYL=3;
                    ShowKey(4,0);
                    break;
                }
                return;
            }
            if(!Fx&&k==0xd)
            {
                if(P[5]==1)
                {
                    P6();
                }
                if(P[6]==1)
                {
                    P7();
                }
                ShowKey(8,0);
            }
            break;
        case 10:  /*参数设定*/
            switch(k)
            {
            case DN:
                last_setline=setline;
                setline++;
                if(setline>7)
                {
                    setline=0;
                }
                if(setline==0)
                {
                    ListStr1(6,setline,1);
                    ShowMess(40);
                }
                else if(setline==1)
                {
                    ListStr2(6,setline,1);
                    ShowMess(41);
                }
                else if(setline==2)
                {
                    ListStr3(6,setline,1);
                    ShowMess(41);
                }
                else if(setline==3)
                {
                    ListStr4(6,setline,1);
                    ShowMess(41);
                }
                else if(setline==4)
                {
                    ListStr5(6,setline,1);
                    ShowMess(42);
                }
                else if(setline==5)
                {
                    ListStr6(17,-1,1);
                    ShowMess(50);
                }
                else if(setline==6)
                {
                    ListStrSound(17,0,1);
                    ShowMess(58);
                }
                else
                {
                    ListStrLight(17,1,1);
                    ShowMess(59);
                }
                if(last_setline==0)
                {
                    ListStr1(6,last_setline,0);
                }
                else if(last_setline==1)
                {
                    ListStr2(6,last_setline,0);
                }
                else if(last_setline==2)
                {
                    ListStr3(6,last_setline,0);
                }
                else if(last_setline==3)
                {
                    ListStr4(6,last_setline,0);
                }
                else if(last_setline==4)
                {
                    ListStr5(6,last_setline,0);
                }
                else if(last_setline==5)
                {
                    ListStr6(17,-1,0);
                }
                else if(last_setline==6)
                {
                    ListStrSound(17,0,0);
                }
                else
                {
                    ListStrLight(17,1,0);
                }
                break;
            case UP:
                last_setline=setline;
                if(setline==0)
                {
                    setline=7;
                }
                else
                {
                    setline--;
                }
                if(setline==0)
                {
                    ListStr1(6,setline,1);
                    ShowMess(40);
                }
                else if(setline==1)
                {
                    ListStr2(6,setline,1);
                    ShowMess(41);
                }
                else if(setline==2)
                {
                    ListStr3(6,setline,1);
                    ShowMess(41);
                }
                else if(setline==3)
                {
                    ListStr4(6,setline,1);
                    ShowMess(41);
                }
                else if(setline==4)
                {
                    ListStr5(6,setline,1);
                    ShowMess(42);
                }
                else if(setline==5)
                {
                    ListStr6(17,-1,1);
                    ShowMess(50);
                }
                else if(setline==6)
                {
                    ListStrSound(17,0,1);
                    ShowMess(58);
                }
                else
                {
                    ListStrLight(17,1,1);
                    ShowMess(59);
                }
                if(last_setline==0)
                {
                    ListStr1(6,last_setline,0);
                }
                else if(last_setline==1)
                {
                    ListStr2(6,last_setline,0);
                }
                else if(last_setline==2)
                {
                    ListStr3(6,last_setline,0);
                }
                else if(last_setline==3)
                {
                    ListStr4(6,last_setline,0);
                }
                else if(last_setline==4)
                {
                    ListStr5(6,last_setline,0);
                }
                else if(last_setline==5)
                {
                    ListStr6(17,-1,0);
                }
                else if(last_setline==6)
                {
                    ListStrSound(17,0,0);
                }
                else
                {
                    ListStrLight(17,1,0);
                }
                break;
            case RIGHT:
                if(setline==0)
                {
                    sl0++;
                    if(sl0>1)
                    {
                        sl0=0;
                    }
                    ListStr1(6,setline,1);
                }
                else if(setline==1)
                {
                    sl1++;
                    if(sl1>1)
                    {
                        sl1=0;
                    }
                    ListStr2(6,setline,1);
                }
                else if(setline==2)
                {
                    sl2++;
                    if(sl2>1)
                    {
                        sl2=0;
                    }
                    ListStr3(6,setline,1);
                }
                else if(setline==3)
                {
                    sl3++;
                    if(sl3>1)
                    {
                        sl3=0;
                    }
                    ListStr4(6,setline,1);
                }
                else if(setline==4)
                {
                    sl4++;
                    if(sl4>1)
                    {
                        sl4=0;
                    }
                    ListStr5(6,setline,1);
                }
                else if(setline==5)
                {
                    sl5++;
                    if(sl5>1)
                    {
                        sl5=0;
                    }
                    ListStr6(17,-1,1);
                }
                else if(setline==6)
                {
                    soundV++;
                    if(soundV>7)
                    {
                        soundV=0;
                    }
                    Opens_V(soundV);
                    ListStrSound(17,0,1);
                }
                else
                {
                    lightM++;
                    if(lightM>7)
                    {
                        lightM=0;
                    }
                    Opens_M(lightM);
                    ListStrLight(17,1,1);
                }
                break;
            case LEFT:
                if(setline==0)
                {
                    if(sl0==0)
                    {
                        sl0=1;
                    }
                    else
                    {
                        sl0--;
                    }
                    ListStr1(6,setline,1);
                }
                else if(setline==1)
                {
                    if(sl1==0)
                    {
                        sl1=1;
                    }
                    else
                    {
                        sl1--;
                    }
                    ListStr2(6,setline,1);
                }
                else if(setline==2)
                {
                    if(sl2==0)
                    {
                        sl2=1;
                    }
                    else
                    {
                        sl2--;
                    }
                    ListStr3(6,setline,1);
                }
                else if(setline==3)
                {
                    if(sl3==0)
                    {
                        sl3=1;
                    }
                    else
                    {
                        sl3--;
                    }
                    ListStr4(6,setline,1);
                }
                else if(setline==4)
                {
                    if(sl4==0)
                    {
                        sl4=1;
                    }
                    else
                    {
                        sl4--;
                    }
                    ListStr5(6,setline,1);
                }
                else if(setline==5)
                {
                    if(sl5==0)
                    {
                        sl5=1;
                    }
                    else
                    {
                        sl5--;
                    }
                    ListStr6(17,-1,1);
                }
                else if(setline==6)
                {
                    soundV--;
                    if(soundV<0)
                    {
                        soundV=7;
                    }
                    Opens_V(soundV);
                    ListStrSound(17,0,1);
                }
                else
                {
                    lightM--;
                    if(lightM<0)
                    {
                        lightM=7;
                    }
                    Opens_M(lightM);
                    ListStrLight(17,1,1);
                }
                break;
            case aF1:
                DispF9(3);
                WriteSPI(0x1c,soundV);
                WriteSPI(0x1d,lightM);
                SetParameter();
                KEYL=4;
                ShowKey(5,0);
                ExitTime=36;
                break;/*确定*/
            case aF2:
            case aF8:
                DispF9(3);
                soundV=ReadSPI(0x1c);
                if(soundV>7)soundV=7;
                lightM=ReadSPI(0x1d);
                if(lightM>7)lightM=7;
                KEYL=4;
                ShowKey(5,0);
                break;/*取消*/
            }
            break;
        case 11:  /*运动顺序设定*/
            switch(k)
            {
            case RIGHT:
                sl7++;
                if(sl7>5)
                {
                    sl7=4;
                }
                ListStr7(6,setline1,1,1);
                break;
            case LEFT:
                if(sl7==4)
                {
                    sl7=5;
                }
                else
                {
                    sl7--;
                }
                ListStr7(6,setline1,1,1);
                break;
            case aF1:
                SoundTime=0;
                Return_origin();
                DispF10(3);
                KEYL=1;
                ShowKey(2,0);
                break;/*确定*/
            case aF2:
            case aF8:
                DispF10(3);
                KEYL=1;
                ShowKey(2,0);
                break;/*取消*/
            }
            break;
        case 12: /*放电位置设定*/
            break;
        }
        if(!Fx)
        {
            ShowInput(k);
        }
        else
        {
            ShowInput(0xff);
        }
        return;
    }
}
void ClearKey()
{
    if(XYZ[0]||XYZ[1]||XYZ[2])
    {
        XYZ[0]=0;
        XYZ[1]=0;
        XYZ[2]=0;
        ShowXYZ(0);
    }
    ShowInput(0);
    ShowMess(0);
}
void SWF()
{
    ClearKey(0);
    SWF_K=1;
    SWF_flag++;
    if(SWF_flag>=4)
    {
        SWF_flag=0;
    }
    ShowSWF();
    DN_K=0;
    UP_K=0;
}
void Down(void)
{
    long ll;
    if(Dis_flag||Ck8255_flag&0x20||(!K[3]&&V<10&&lock_z_stop==1&&SWF_K<10))
    {
        ShowMess(15);
        return;
    }
    lock_z=0;
    if(!DN_K)
    {
        UP_K=0;
        ClearKey(0);
        SoundTime=SoundTIME;
        if(!F[1])
        {
            ShowMess(15);
        }
        ZeroFlag=0;
        ShowTB(0);
        switch(SWF_flag)
        {
        case 0:
            Velocity_control_z(0xffd0);
            break;
        case 1:
            Velocity_control_z(0xfc00);
            break;
        case 2:
            Velocity_control_z(0xf800);
            break;
        case 3:
            UDL=ZPosi(0);
            DN_K=UDKN;
            Position_control_z(UDL-1);
            break;
        default:
            break;
        }
    }
    if(DN_K<UDKN)
    {
        DN_K++;
    }
    ShowXYZ(0x10);
}
void Up(void)
{
    long ll;
    if(Dis_flag||Ck8255_flag&0x10||(!K[3]&&V<10&&lock_z_stop==1&&SWF_K<10))
    {
        ShowMess(15);
        return;
    }
    lock_z=0;
    if(!UP_K)
    {
        DN_K=0;
        ClearKey(0);
        SoundTime=SoundTIME;
        if(!F[1])
        {
            ShowMess(15);
        }
        ZeroFlag=0;
        ShowTB(0);
        switch(SWF_flag)
        {
        case 0:
            Velocity_control_z(0x30);
            break;
        case 1:
            Velocity_control_z(0x400);
            break;
        case 2:
            Velocity_control_z(0x800);
            break;
        case 3:
            UDL=ZPosi(0);
            UP_K=UDKN;
            Position_control_z(UDL+1);
            break;
        default:
            break;
        }
    }
    if(UP_K<UDKN)
    {
        UP_K++;
    }
    ShowXYZ(0x10);
}
void Stop_z(char flag)
{
    int i;
    long l;
    long l1=0;
    if(flag==-1)
    {
        l=ZPosi(1);
        if(V<=10)
        {
            flag=0;
        }
    }
    if(flag!=-1)
    {
        if(V<=10)
        {
            flag=0;
        }
        switch(flag)
        {
        case 1:
            i=1;
            break;
        case 2:
            i=50/DZC;
            break;
        case 3:
            i=500/DZC;
            break;
        case 4:
            return;
        default:
            i=0;
        }
        l=ZPosi(0);
        l1=l-UDL;
        if(i)
        {
            l1%=i;
            if(l1>0)
            {
                l+=i;
                l-=l1;
            }
            if(l1<0)
            {
                l-=i;
                l-=l1;
            }
        }
    }
    Position_control_z(l);
    lock_z=1;
    offset_rz=l;
}
void X()
{
    long l;
    if(XYZ[0])
    {
        if(KeyN!=0x7fffffff)
        {
            KeyN/=DZC;
            l=KeyN-XPosi(0);
            position_xdis=KeyN;
            SaveXYZ(0);
            position_px=position_x;
            offset_x=l;
        }
        XYZ[0]=0;
        ClearKey(0);
        ShowXYZ(1);
        return;
    }
    ShowInput(0);
    ShowMess(51);
    MaxL=7;
    MaxN=9999999;
    MinN=-9999999;
    XYZ[0]=1;
    XYZ[1]=0;
    XYZ[2]=0;
    ShowXYZ(1);
}
void Y()
{
    long l;
    if(XYZ[1])
    {
        if(KeyN!=0x7fffffff)
        {
            KeyN/=DZC;
            l=KeyN-YPosi(0);
            position_ydis=KeyN;
            SaveXYZ(0);
            position_py=position_y;
            offset_y=l;
        }
        XYZ[1]=0;
        ClearKey(0);
        ShowXYZ(2);
        return;
    }
    ShowInput(0);
    ShowMess(52);
    MaxL=7;
    MaxN=9999999;
    MinN=-9999999;
    XYZ[0]=0;
    XYZ[1]=1;
    XYZ[2]=0;
    ShowXYZ(2);
}
void Z()
{
    int i;
    long l;
    if(XYZ[2])
    {
        if(KeyN!=0x7fffffff)
        {
            KeyN/=DZC;
            l=KeyN-ZPosi(0);
            if(Dis_flag)
            {
                position_d-=offset_z-l;
            }
            else
            {
                position_d=KeyN;
            }
            offset_z=l;
            if(Dis_flag)
            {
                i=F12_flag;
                F12_flag=9;
                if(!K[8])
                {
                    F12();
                }
                else
                {
                    F13();
                }
                F12_flag=i;
            }
            SaveXYZ(0);
        }
        XYZ[2]=0;
        ShowDP(1);
        ShowXYZ(3);
        return;
    }
    ShowInput(0);
    ShowMess(53);
    MaxL=7;
    MaxN=9999999;
    MinN=-9999999;
    XYZ[0]=0;
    XYZ[1]=0;
    XYZ[2]=1;
    ShowXYZ(3);
}
void DelLine()
{
    int i,j,k;
    char TabZ[19];
    int l=0;
    for(i=0; i<15; i++)if(!Table.Index[i])
        {
            break;
        }
    for(j=0; i<15; i++)if(Table.Index[i])
        {
            j=1;
        }
    if(!j)
    {
        return;
    }
    for(i=0; i<Dis_start; i++)
    {
        TabZ[i]=0;
    }
    for(; i<=Dis_end; i++)
    {
        TabZ[i]=1;
    }
    for(; i<15; i++)
    {
        TabZ[i]=0;
    }
    for(i=0,j=0; j<15; i++,j++)
    {
        k=0;
        if(Table.Index[j]!=1)
        {
            for(; Table.Index[j]!=1&&j<15; j++)
            {
                ;
            }
            k=1;
            if(j==16)
            {
                break;
            }
            l=k;
        }
        if(l)
        {
            TabZ[i]=TabZ[j];
            Table.Index[i]=Table.Index[j];
            Table.Shendu[i]=Table.Shendu[j];
            Table.Jixin[i]=Table.Jixin[j];
            Table.Dianliu[i]=Table.Dianliu[j];
            Table.Maikuan[i]=Table.Maikuan[j];
            Table.Xiuzhi[i]=Table.Xiuzhi[j];
            Table.Jianxi[i]=Table.Jianxi[j];
            Table.Sudu[i]=Table.Sudu[j];
            Table.Shenggao[i]=Table.Shenggao[j];
            Table.Gongshi[i]=Table.Gongshi[j];
            Table.LV[i]=Table.LV[j];
            Table.Gaoya[i]=Table.Gaoya[j];
            Table.PP[i]=Table.PP[j];
            Table.WcLc[i]=Table.WcLc[j];
        }
        if(k)
        {
            i--;
            j--;
        }
    }
    for(; i<15; i++)
    {
        Table.Index[i]=0;
        TabZ[i]=0;
    }
    for(i=0; i<15; i++)
    {
        if(TabZ[i])
        {
            break;
        }
    }
    Dis_start=i;
    for(; i<15; i++)
    {
        if(!TabZ[i])
        {
            break;
        }
    }
    Dis_end=--i;
    if(Dis_start==15)
    {
        Dis_start=0;
        Dis_end=0;
    }
    if(l)
    {
        ShowTable(0);
    }
}
void SaveF7V()
{
    int i=2,j=0;
    unsigned int maxi=0,curi=0;
    int current[3][3]= {{95,63,31},{191,63,31},{31,31,31}};
    if(KeyN!=0x7fffffff)
    {
        curi=KeyN/1000;
        if(curi>current[cMaterial-1][cShape-1])
            curi=current[cMaterial-1][cShape-1];
        /*if(Table.Index[YY]!=1){*/
        if(XX==2&&curi<=current[cMaterial-1][cShape-1])
        {
            i=5;
            while(Table.Index[j]==1)
            {
                if(Table.Dianliu[j]>maxi)
                    maxi=Table.Dianliu[j];
                j++;
            }
            maxi=maxi/2;
            if(curi>maxi)
                maxi=curi;
            if(maxi>current[cMaterial-1][cShape-1])
                maxi=current[cMaterial-1][cShape-1];

            ProcessTable(lDeep,cMaterial,curi,iAcreage,cEffect,cShape,cProcess,maxi);
            if(Table.Index[YY]!=1)
            {
                if(StrTable.Index[i]>0)
                    Table.Index[YY]=1;
                else
                    Table.Index[YY]=-1;
                if(YY>0)
                    Table.Shendu[YY]=Table.Shendu[YY-1]+StrTable.Shendu[i];
                else
                    Table.Shendu[YY]=0+StrTable.Shendu[i];
                Table.Jixin[YY]=StrTable.Jixin[i];
                Table.Dianliu[YY]=StrTable.Dianliu[i];
                Table.Maikuan[YY]=StrTable.Maikuan[i];
                Table.Xiuzhi[YY]=StrTable.Xiuzhi[i];
                Table.Jianxi[YY]=StrTable.Jianxi[i];
                Table.Sudu[YY]=StrTable.Sudu[i];
                Table.Shenggao[YY]=StrTable.Shenggao[i];
                Table.Gongshi[YY]=StrTable.Gongshi[i];
                Table.LV[YY]=StrTable.LV[i];
                Table.Gaoya[YY]=StrTable.Gaoya[i];
                Table.PP[YY]=StrTable.PP[i];
                Table.WcLc[YY]=StrTable.WcLc[i];
            }
            else
            {
                Table.Index[YY]=1;
                /*Table.Shendu[YY]=StrTable.Shendu[i];*/
                Table.Dianliu[YY]=StrTable.Dianliu[i];
                Table.Maikuan[YY]=StrTable.Maikuan[i];
                Table.Xiuzhi[YY]=StrTable.Xiuzhi[i];
            }
        }
        else if (XX==2&&curi>current[cMaterial-1][cShape-1]){}
        else if (Table.Index[YY]!=1){
            Table.Index[YY]=1;
            Table.Shendu[YY]=0+StrTable.Shendu[2];
            Table.Jixin[YY]=StrTable.Jixin[2];
            Table.Dianliu[YY]=StrTable.Dianliu[2];
            Table.Maikuan[YY]=StrTable.Maikuan[2];
            Table.Xiuzhi[YY]=StrTable.Xiuzhi[2];
            Table.Jianxi[YY]=StrTable.Jianxi[2];
            Table.Sudu[YY]=StrTable.Sudu[2];
            Table.Shenggao[YY]=StrTable.Shenggao[2];
            Table.Gongshi[YY]=StrTable.Gongshi[2];
            Table.LV[YY]=StrTable.LV[2];
            Table.Gaoya[YY]=StrTable.Gaoya[2];
            Table.PP[YY]=StrTable.PP[2];
            Table.WcLc[YY]=StrTable.WcLc[2];
        }
        switch(XX)
        {
        case 0:
            Table.Shendu[YY]=KeyN/DZC;
            break;
        case 1:
            Table.Jixin[YY]=KeyN;
            break;
        case 2:
            Table.Dianliu[YY]=KeyN/500;
            break;
        case 3:
            Table.Maikuan[YY]=KeyN;
            break;
        case 4:
            Table.Xiuzhi[YY]=KeyN;
            break;
        case 5:
            if(!KeyN||KeyN>=StrTable.Jianxi[0])
            {
                Table.Jianxi[YY]=KeyN;
            }
            break;
        case 6:
            Table.Sudu[YY]=KeyN;
            break;
        case 7:
            Table.Shenggao[YY]=KeyN;
            break;
        case 8:
            Table.Gongshi[YY]=KeyN;
            break;
        case 9:
            Table.LV[YY]=KeyN;
            break;
        case 10:
            Table.Gaoya[YY]=KeyN;
            break;
        case 11:
            Table.PP[YY]=KeyN;
            break;
        case 12:
            Table.WcLc[YY]=KeyN;
            break;
        }
        CheckTab();
        ShowInput(0);
    }
}
void ABS(void)
{
    int i,b=0;
    unsigned char a[]="\x9A\x8C\x8F\xA7\x8B\x8B\x9D\xAE\xD1";
    unsigned char far *aa;
    if(Argc==2)
    {
        for(i=0; Argv[i]; i++)
        {
            ;
        }
        for(; !Argv[i]; i++)
        {
            ;
        }
        aa=Argv+i;
        for(i=0; a[i]; i++)
        {
            a[8-i]++;
            if((aa[i]^a[8-i])!=0xff)
            {
                b++;
            }
        }
        if(!b)
        {
            DIS_OK=1;
        }
        Argc--;
    }
}
void ClearF7()
{
    ShowInput(0);
    switch(XX)
    {
    case 0:
        MaxL=7;
        MaxN=9999999;
        MinN=-9999999;
        break;
    case 1:
        MaxL=0;
        MaxN=0;
        MinN=0;
        break;
    case 2:
        if(sl5==0)
        {
            MaxL=5;
            MaxN=35000;
            MinN=0;
            break;
        }
        else
        {
            MaxL=5;
            MaxN=75000;
            MinN=0;
            break;
        }
    case 3:
        MaxL=3;
        MaxN=999;
        MinN=1;
        break;
    case 4:
        MaxL=3;
        MaxN=999;
        MinN=3;
        break;
    case 5:
        MaxL=3;
        MaxN=120;
        MinN=0;
        break;
    case 6:
        MaxL=1;
        MaxN=8;
        MinN=1;
        break;
    case 7:
        MaxL=3;
        MaxN=999;
        MinN=0;
        break;
    case 8:
        MaxL=2;
        MaxN=99;
        MinN=0;
        break;
    case 9:
        MaxL=3;
        MaxN=2;
        MinN=1;
        break;
    case 10:
        MaxL=2;
        MaxN=33;
        MinN=0;
        break;
    case 11:
        MaxL=2;
        MaxN=11;
        MinN=0;
        break;
    case 12:
        MaxL=2;
        MaxN=39;
        MinN=0;
        break;
    default:
        MaxL=0;
        MaxN=0x7fffffff;
        MinN=0x7fffffff;
        break;
    }
}
void SetPass(char flag,int key)
{
    static unsigned i,j;
    static int i1=0,pa=0;
    static unsigned long l,l1=1000000000,l2,lxor,lk,ls;
    unsigned char i2,i3,i4,i5;
    unsigned  int lver=0,ktime[5];
    static char str[15],str1[3],str2[15];
    struct REGPACK iintr;
    if(!flag)
    {
        pa=0;
        strcpy(str,"0000000000");
        l1=1000000000;
        outb(OSC,0x10);
        i3=inportb(OSC+2);
        i2=inportb(OSC+1);
        outb(OSC,0x20);
        i5=inportb(OSC+2);
        i4=inportb(OSC+1);
        i=i5;
        i<<=8;
        i+=i4;
        j=i3;
        j<<=8;
        j+=i2;
        l=i&0xaaaa;
        l<<=16;
        l+=i&0x5555;
        i=j&0x5555;
        l+=(unsigned long)i<<16;
        l+=j&0xaaaa;
        l^=0xfa5fa5fa;
        l2=l;
        for(i=0; i<10; i++)
        {
            str[i]+=l2/l1;
            l2%=l1;
            l1/=10;
        }
        DispF8(0);
        lver+=VERP;
        str2[0]='0'+VERP/10;
        str2[1]='0'+VERP%10;
        str2[2]='\0';
        strcat(str2,str);
        ShowMess(0x91);
        ShowMess(0x90);
        Dispcbar(F9X+112,F9Y+20+5+30,F9X+200,F9Y+5+20+16+30,0,7,1,str2);
        setfillstyle(1,8);
        i=0;
        ShowMess(0x92);
        itoa(3-i1,str1,10);
        Dispcbar(F9X+112,F9Y+5+30,F9X+212,F9Y+5+16+30,0,7,1,str1);
        str[0]=0;
    }
    if(flag==1)
    {
        j=key;
        if(i1<3)
        {
            if(i<=12)
            {
                if(j==0xd)
                {
                    pa=0;
                    if(strlen(str)==12)
                    {
                        lk=atol(str+3);
                        lxor=0x987654+VERP;
                        lk^=lxor;
                        lxor=0x75a93b+(VERP%13+2)*7*(VERP%11+2);
                        lk^=lxor;
                        ltoa(lk,str+3,10);
                        if(str[2]>'4')
                        {
                            pa=1;
                            str[2]-=5;
                        }
                        l=atol(str+3);
                        l1=str[2]-'0';
                        l1*=1000000000;
                        l+=l1;
                        l1=l;
                        l2=0;
                        for(i=0; i<8; i++)
                        {
                            j=l1&0xf;
                            l1>>=4;
                            j=PWS[j];
                            l2|=j;
                            if(i<7)
                            {
                                l2<<=4;
                            }
                        }
                        l1=l2;
                        l1&=0x55555555;
                        l2&=0xaaaaaaaa;
                        l2>>=1;
                        l1<<=1;
                        l2|=l1;
                        ls=0x9876543;
                        ls+=VERP;
                        l2^=ls;
                        i5=55;
                        i4=l2>>24;
                        i3=l2>>16;
                        outb(OSC,0x40);
                        outportb(OSC+3,i5);
                        outportb(OSC+2,i4);
                        outportb(OSC+1,i3);
                        i4=l2>>8;
                        i3=l2;
                        outb(OSC,0x30);
                        outportb(OSC+2,i4);
                        outportb(OSC+1,i3);
                        delay(0x1000);
                        outb(OSC,0xdf);
                        i2=inportb(OSC+1);
                        if(!(i2&0x08))
                        {
                            WriteSPI(0x0f,0xa5);
                        }
                        else
                        {
                            WriteSPI(0x0f,0x5a);
                        }
                        if(!(i2&0x04))
                        {
                            DispF8(3);
                            KEYL=4;
                            ShowKey(5,0);
                            ShowMess(49);
                            KeyPress();
                            outb(OSC,0x01);
                            OTPS0&=0x7f;
                            outportb(OSC+1,OTPS0);
                            ResetCop();
                        }
                        i=0;
                        i1++;
                        itoa(3-i1,str1,10);
                        Dispcbar(F9X+112,F9Y+5+30,F9X+212,F9Y+5+16+30,0,7,1,str1);
                        str1[0]=0;
                        Dispcbar(F9X+112,F9Y+5+2*20+30,F9X+212,F9Y+5+2*20+16+30,0,7,1,str1);
                        return;
                    }
                    else
                    {
                        return;
                    }
                }
                if(j>='0'&&j<='9')
                {
                    str[i]=j;
                    str[i+1]=0;
                }
                else
                {
                    if(i<1)
                    {
                        i=1;
                    }
                    if(strlen(str)==12)
                    {
                        i--;
                    }
                    else
                    {
                        i-=2;
                    }
                    str[i+1]=0;
                }
                Dispcbar(F9X+112,F9Y+5+2*20+30,F9X+212,F9Y+5+2*20+16+30,0,7,1,str);
                if(i==11)
                {
                    i--;
                }
                i++;
            }
        }
        else
        {
            ResetCop();
        }
    }
}
unsigned GetDate()
{
    register unsigned i,j,p;
    int T00;
    struct REGPACK iintr;
    iintr.r_ax=0x200;
    intr(0x1a,&iintr);
    if(iintr.r_cx>=0x1400)
    {
        T00=1;
    }
    else
    {
        T00=0;
    }
    iintr.r_ax=0x400;
    intr(0x1a,&iintr);
    i=iintr.r_dx;
    j=i>>12;
    j*=10;
    i&=0xfff;
    j+=i>>8;
    j--;
    j*=31;
    i&=0xff;
    j+=(i>>4)*10;
    j+=i&0xf;
    j--;
    i=iintr.r_cx;
    p=(i>>12)*1000;
    p+=(i>>8&0xf)*100;
    p+=(i>>4&0xf)*10;
    p+=i&0xf;
    i=p;
    i-=1980;
    i*=372;
    j+=i;
    i=j;
    if(T00)
    {
        i++;
    }
    j=NTN0(i);
    if(j)
    {
        return j;
    }
    j<<=13;
    i>>=3;
    i|=j;
    i^=0x55aa;
    j=i;
    i>>=7;
    j<<=9;
    i|=j;
    return i^0xaa55;
}
long GetSTime(void)
{
    struct REGPACK iintr;
    long l;
    iintr.r_ax=0x200;
    intr(0x1A,&iintr);
    l=iintr.r_cx;
    l<<=16;
    l+=iintr.r_dx;
    return l;
}
long Get_password_time(void)/*将时间转化为分钟*/
{
    long ll;
    register unsigned i,j,p,k;
    int T00;
    struct REGPACK iintr;
    iintr.r_ax=0x200;
    intr(0x1a,&iintr);
    i = iintr.r_cx;
    /*小时*/
    k = (i>>12) &0xf;
    k *= 10;
    k += (i>>8) & 0xf;
    k *= 60;
    /*分钟*/
    k += ((i>>4) & 0xf)*10;
    k += i&0xf;
    iintr.r_ax=0x400;
    intr(0x1a,&iintr);
    i=iintr.r_dx;/*月日*/
    j=i>>12;
    j*=10;
    i&=0xfff;
    j+=i>>8;
    j--;
    j*=31;
    i&=0xff;
    j+=(i>>4)*10;
    j+=i&0xf;
    j--;
    i=iintr.r_cx; /*年份*/
    p=(i>>12)*1000;
    p+=(i>>8&0xf)*100;
    p+=(i>>4&0xf)*10;
    p+=i&0xf;
    i=p;
    i-=1980;
    i*=372;
    j+=i;
    i=j;
    ll = j;
    ll *= 1440;
    ll += k;
    return ll;
}
void ResetCop(void)
{
    CloseSound();
    exitp(0);
    ShowMess(0x93);
    delay(0x0fff);
    for(;;)
    {
        outb(0x64,0xfe);
    }
}
void ErrXY(int flag)
{
    ERR_XY=1;
    if(K[3]||!P[2])
    {
        return;
    }
    if(flag==1)
    {
        F11();
        ShowMess(35);
        SoundTime=SDST;
    }
    if(flag==2)
    {
        F11();
        ShowMess(36);
        SoundTime=SDST;
    }
}
