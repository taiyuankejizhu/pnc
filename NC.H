#include "znc.h"
char * DispKey(int zu,int fkey);
struct Tab Table;
struct Tab StrTable={/*[4]校模参数*/
{0,1,1,0,1,0,0,0,0,0},          /*索引*/
{-9999999,9999999,-1,0,-9999999,0,0,0,0,0}, /*深度*/
{0,1,0,0,0,0,0,0,0,0},          /*极性*/
{0,75*2,6,0,4,0,0,0,0,0},       /*电流*/
{1,999,200,0,20,0,0,0,0,0},     /*脉宽*/
{3,999,100,0,30,0,0,0,0,0},     /*休止*/
{30,120,55,0,60,0,0,0,0,0},     /*间隙*/
{1,8,5,0,5,0,0,0,0,0},          /*速度*/
{0,999,6,0,0,0,0,0,0,0},        /*升高*/
{1,99,5,0,99,0,0,0,0,0},        /*工时*/
{1,2,0,0,0,0,0,0,0,0},          /*低压*/
{0,33,0,0,0,0,0,0,0,0},         /*高压*/
{0,11,0,0,0,0,0,0,0,0},        /*PP*/
{0,39,0,0,0,0,0,0,0,0},     /*LC*/
};
char F[10]={4,0,0,4,0,0,0,0,0,0};
char K[10]={0,0,2,0,0,0,0,0,0,0};
char P[15]={0,0,0,0,1,0,2,2,0,0,0,0,0,0,0};
char TabWidth[14]={45,100,40,45,45,45,45,45,45,45,45,45,45,45};
char TabWidth1[14]={2,2,2,2,2,2,2,2,2,2,2,2,2,2};
char TabWidth2[14]={4,4,6,4,4,4,4,4,4,4,4,4,4,4};
char GaoyaV;    
char GaoyaA;
char Gaoya;
int LV[2]={90,120}; /* 90V 120V */
char Other;
char Clip;
char Short;
char WcL;
char WcC;
char full;
unsigned char PWS[17]={1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,0};
unsigned char IO0,IO1,IO2,IO3,IO4,IO5,OTPS0,OTPS1,IOPS0,IOPZ0,OTPZ0,IOZ0,OTPZ1,IOX0,IOY0;/*OSC0 输出标志位，OSC1*/
char OCSF=0;
char ORgin=0;
float MX=0;
unsigned FS=0;/*风扇*/
unsigned SGY=0,SDL=0;
char XYZ[]={0,0,0};
char F7auto[6];
char Pump_flag=0;
unsigned AI_time=0;
unsigned AI_high=999;
long MianJi=0;
char Sound_flag=0;
char V0ING=0;
char Steel_flag=0;
char G_Y=2;
char M_J=1;
char J_X=0;
long K_3=10;
char XX,YY,Y7Y;
char *Argv;
int Argc;
long Zpointer=0;
char Fx;        /*1:按下F?,0数字母*/
char *JixinS[2]={"+","-"};
char Inputstr[8]=" ",Inputstr1[8]=".";
char Group=0;/*当前组*/
char XYZGroup;
char Dis_flag=0,Dis_first=0,Dis_lines=0,Dis_start=0,Dis_end=0;
char MaxL=-1;
long KeyN=0x7fffffff,MinN,MaxN;
char F11_flag=1,F12_flag=0;
char Position_flag=0;
char Profile=0;
unsigned ProtectTime=0;
long Work_time,Up_time,V_time; /*升高,工时*/
long Velocity=0;
char Profile_flag=0; /*运行Profile操作时置1，运行完毕后置0*/
char Elect_speed1=8;   /*放电时机头上下的速度*/
int soundV=5;
unsigned char buf[0x200];
unsigned char currents[3][3]= {{95,63,31},{191,63,31},{31,31,31}};
char SWF_flag=1;
char SWF_K=0;
char UP_K=0;
char DN_K=0;
char DNUP=0;

long UDL;
int F8H=0;
char F8T=0;
char TF8=0;/*4.14修改参数表生效时间*/
char Showline=0;/*当前显示行*/
int steel,JX;
char Scr_flag=0;
int A;
int V;
int KEYL=0;
int SMessage=0;
int Initsystem=0;
int DIS_OK;
char oldpage=-1;
char Ck8255_flag=0,Ck8256_flag=0;
char ZeroFlag=0;
char FireFlag=0,Pum_flag=0,Pumn_flag=0;
char PassWordF=0x2d;
int SoundTime=0;
int ExitTime=0;
long Oldtime=0,Velocity_Time=0;
long ox,oy,oz,origin_z,origin_x,origin_y,diff_oz=0,diff_ox=0,diff_oy=0,lpz=0,s_OZh=0,s_OZl=0,F_positionz=0;
long position_x = -9999999,position_y = -9999999,position_z =0x8fffffff,R_positionz=0x8fffffff,position_t =0,position_tt;
long offset_x=0,offset_y=0,offset_z=0,offset_t=0,position_p=-9999999,position_d=9999999;
long position_dx,position_dy,position_dz,position_px,position_py,position_pz;
long offset_rx=0,offset_ry=0,offset_rz=0;
char lock_x=1,lock_y=1,lock_z=1;
long offset_xx[10],offset_yy[10],offset_zz[10];
long position_xdis,position_ydis,offset_d=1999999;
char offset_ds=0;
long lDeep=0;
char cEffect=1,cMaterial=1,cShape=1,cProcess=1;
int  iCurrent=1;
int  iAcreage=1;
char ERR_XY=0;
int DIS_OK1;
unsigned OldID,OldDATE;
unsigned AI=0,AITime=0,AIHigh=1;
long ReadRegisterValue_Z(char cLowReg);
void Open_Elect(void);
void Close_Elect(void);
void Open_OCSF(void);
void Close_OCSF(void);
int ProTime(void);
void Profile_z(char MaxVelocity,int Acceleration,long position);
void ShowTable(int liness);
void ShowXYZ(int flag);
void ShowDP(char flag);
void ShowFlag(char flags);
void ShowHT(void);
void ShowMess(int value);
void ShowInput(char value);
void ShowKey(int kn,int ke);
long GetInput(void);
void InitKey(char flag);
void exitp(char flag);
char DispF7(int flag);
void ShowF7(int lines);
void ShowF8(int lines);

void ShowFL(int flag);
void ShowPZ(int);
int CheckBMP(void);
unsigned GetDate();
void JiXin(int);/*极性,0=-,1=+*/
void ClearKey(int key);
void F8(char flag);
void F7(char flag);
void SetDianliu(unsigned dianliu);
int Cmp(unsigned,unsigned);
unsigned NTN0(unsigned);
unsigned N0TN(unsigned);
void ResetCop(void);
int SetWork(unsigned,unsigned);
int SetWork1(unsigned,unsigned);
void SaveXYZ(char flag);
long GetSTime(void);
void ShowTime(char flag);
void ShowTimeD(int flag);
void ShowVer(char *ver);
void Up(void);
void Down(void);
void Delay1(unsigned mill);
void SaveBMP(void);
void F13(void);

unsigned char sl0=1,sl1=0,sl2=0,sl3=0,sl4=0,sl5=1,sl6=0,sl7=4,sl8=4,sl9=4;
unsigned setline=0,last_setline=0;
unsigned setline1=0,last_setline1=0;
extern unsigned DZC=1;
unsigned Pid_kp,Pid_ki,Pid_kd,Pid_st;
unsigned xPid_kp,xPid_ki,xPid_kd,xPid_st;
long origin=0,loz_init=0;
char Origin_Flag=0;
long dis_time = 0;
