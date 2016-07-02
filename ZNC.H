#include <graphics.h>
#include "malloc.h"
#include "alloc.h"
#include "time.h"
#define TESTING 0
#define SIONV   0
#if SIONV
#define VER "4.31"
#define VERP 30 /*版本标识*/
#else
#define VER "4.60"
#define VERP 60 /*版本标识*/
#endif
#define spi 0x260
#define IO  0x258
#define Address_Z 0x248
#define OSC 0x250
#define ICXaddr 0x240
#define ICYaddr 0x244
#define UDKN 2
#define IDS 0xd3b33C5A
#define CLK0 0
#if SIONV
#define BKCOLORA 0
#define BKCOLORB 0
#define BKCOLORC 8
#define BKCOLORD 0
#define BKCOLORE 7
#define BKCOLORG 19
#define BKCOLORE1 7 /*3D*/
#define BKCOLORE2 0 /*NUL*/
#define COLORA 10
#define COLORA1 11
#define COLORA2 12
#define COLORB 11
#define COLORC 12
#define COLORD 10
#define COLORD1 11
#define COLORE 11
#define COLORG 12
#define ren 412nc.bat autoexec.bat 7
#define COLORE1 0   /*3D*/
#define COLORE2 15  /*NUL*/
#define AX0 119 /*坐标框*/
#define AX1 548
#define AY0 0
#define AY1 186
#define BX0 119 /*参数表 */
#define BX1 638
#define BY0 187
#define BY1 387
#define CX0 119   /*讯息栏*/
#define CX1 638
#define CY0 388
#define CY1 423
#define DX0 549 /*机械坐标*/
#define DX1 638
#define DY0 0
#define DY1 186
#define EX0 0   /*状态栏*/
#define EX1 118
#define EY0 0
#define EY1 423
#define GX0 0   /* KEY  */
#define GX1 638
#define GY0 424
#define GY1 478
#define X0 0
#define X1 639
#define X2 550
#define X3 280
#define Y0 0
#define Y1 479
#define Y2 52
#define Y3 190
#define F7X 233
#define F7Y 200
#define F8X 233
#define F8Y 255
#define F9X 233
#define F9Y 190  /*227*/
#else
#define BKCOLORA 0
#define BKCOLORB 0
#define BKCOLORC 8
#define BKCOLORD 0
#define BKCOLORE 7
#define BKCOLORG 7
#define BKCOLORE1 7 /*3D*/
#define BKCOLORE2 0 /*NUL*/
#define COLORA 10
#define COLORA1 11
#define COLORA2 12
#define COLORB 11
#define COLORC 12
#define COLORD 10
#define COLORD1 11
#define COLORE 11
#define COLORG 9
#define COLORE1 0   /*3D*/
#define COLORE2 15  /*NUL*/
#define AX0 0
#define AX1 429
#define AY0 0
#define AY1 186
#define BX0 0
#define BX1 519
#define BY0 185
#define BY1 387
#define CX0 0
#define CX1 519
#define CY0 388
#define CY1 423
#define DX0 430
#define DX1 519
#define DY0 0
#define DY1 186
#define EX0 520
#define EX1 638
#define EY0 0
#define EY1 423
#define GX0 0
#define GX1 638
#define GY0 424
#define GY1 478
#define X0 0
#define X1 639
#define X2 550
#define X3 280
#define Y0 0
#define Y1 479
#define Y2 52
#define Y3 190
#define F7X 133
#define F7Y 225
#define F8X 133
#define F8Y 255
#define F9X 133
#define F9Y 208
#endif
#define ComputerType 2    /* 1 for 486,2 for 586*/
#define DELAY 10
void outb(int portid,unsigned char valud);
unsigned char inb(int portid);
void DeepProcess(void);
void InitKABT(void);
struct ThreeByte
{
    unsigned char cLowByte;
    unsigned char cMidByte;
    unsigned char cHighByte;
    unsigned char cNoUse;
    int cFlag;
};
struct Nodes
{
    long x_pos[20]; /*X偏移*/
    long y_pos[20]; /*Y偏移*/
    long z_pos[20]; /*Z升高*/
    char flag[21]; /*状态标识*/
};
extern struct Nodes Node;
struct _TAB_ /*len 83*/
{
    char no[5];
    char pl[2];
    char ip[6];
    char on[4];
    char off[4];
    char sv[4];
    char s[3];
    char up[4];
    char dn[3];
    char v[3];
    char hv[3];
    char lc[3];
    char pp[3];
    char lns[5];
    char step[6];
    char lnm[4];
    char b_ofs[6];
    char s_ofs[6];
    char ry[5];
    char timer[3];
};
extern struct Tab
{
    char    Index[16];
    long          Shendu[15]; /*深度*/
    unsigned char Jixin[15];    /*极性*/
    unsigned int Dianliu[15]; /*电流*/
    unsigned int  Maikuan[15]; /*脉宽*/
    unsigned int  Xiuzhi[15];  /*休止*/
    unsigned char Jianxi[15];  /*间隙*/
    unsigned char Sudu[15];    /*速度*/
    unsigned int  Shenggao[15];    /*升高*/
    unsigned char Gongshi[15];    /*工时*/
    unsigned char LV[15];       /*低压*/
    unsigned char Gaoya[15];    /*高压V*/
    unsigned char PP[15];       /*PP*/
    unsigned char WcLc[15];    /*LC*/
} Table;
extern struct Tab StrTable;
extern char Dis_flag;   /*放电标志。正在放电为1，停止放电为0。*/
extern char Dis_first;
extern long position_x; /*当前X轴的位置*/
extern long offset_x;   /*当前X轴的辅助位置,POSITION=OFFSET+COUNTER*/
extern long position_y; /*当前Y轴的位置*/
extern long offset_y;   /*当前Y轴的辅助位置,POSITION=OFFSET+COUNTER*/
extern long position_z,R_positionz; /*当前Z轴的位置*/
extern long offset_z;   /*当前Z轴的辅助位置,POSITION=OFFSET+COUNTER*/
extern long position_d; /*当前的最深深度*/
extern long position_p; /*当前的目标深度*/
extern long offset_rx;/*当前X轴寄存器坐标*/
extern long offset_ry;
extern long offset_rz;
extern long position_dx;
extern long position_dy;
extern long position_dz;
extern long position_px;
extern long position_py;
extern long position_pz;
extern char lock_x,lock_y,lock_z;
extern char lock_z_stop,lock_x_stop,lock_y_stop;
extern long position_t;
extern long position_tt;
extern long offset_xx[],offset_yy[],offset_zz[];
extern char Muti_flag;  /*放电状态机*/
extern char Dis_pos;        /*当前放电位置*/
extern long position_sx,position_sy;/*放电定位位置*/
extern char Dis_lines;   /*当前放电的行*/
extern char Dis_start;  /*定段的首行*/
extern char Dis_end;    /*定段的末行*/
extern char Profile_flag; /*运行Profile操作时置1，运行完毕后置0*/
extern char Group;   /*当前所处的组别*/
extern char XYZGroup;
extern char Elect_State;
extern int Elect_time;
extern char Elect_speed;
extern int soundV;  /*调节音量*/
extern char Elect_speed;   /*放电时机头上下的速度*/
extern char Elect_speed1;   /*放电时机头上下的速度*/
extern char AI_Flag;       /*人工智能标志*/
extern unsigned AI_time;
extern unsigned AI_high;
extern struct tm stTime;
extern char cClearFlag;
extern char RiseFlag;
extern char F11_flag;
extern long steel_time;
extern char SteelFlag;
extern char steel_flag;
extern char Sound_flag;
extern char Ep_Short;
extern int V;
extern int A;/*电流*/
extern int DIS_OK;
extern int DIS_OK1;
extern long Velocity;
extern char full;
extern unsigned int ProtectTime;
extern char Error1100_flag;
extern unsigned char IO0,IO1,IO2,IO3,IO4,IO5,OTPS0,OTPS1,IOPS1,IOZ0,OTPZ0,IOPZ1;
extern unsigned char IOX0,IOY0,OTPBX0,OTPBX1,IOPBX0,OTPBY0,OTPBY1,IOPBY0;
extern char OCSF;
extern float MX;
extern unsigned FS;/*风扇*/
extern unsigned SGY,SDL;
extern char TF8;/*4.14修改参数表生效时间*/
extern char Showline;/*当前显示行*/
extern char F6line;/*当前位置显示行*/
void OpenPower(void);      /*打开副电源*/
void ClosePower(void);   /*关闭副电源*/
void Signal_Jianxi(char flag);    /*发放间隙电压信号*/
void Open_OCSF(void);   /*开脉冲*/
void Close_OCSF(void);  /*关脉冲*/
void Open_Elect(void);  /*开变压器*/
void Close_Elect(void); /*关变压器*/
void Open_Pump(void);   /*开油泵*/
void Close_Pump(void);  /*关油泵*/
void CloseAllPower(void); /*关闭所有电源*/
unsigned char Current(void);   /*读取电流值*/
unsigned char Voltage(void);   /*读取电压值*/
void Position_control_z(long position); /**(struct ThreeByte position);**/ /* Entry position control mode */
void Velocity_control_z(unsigned v);           /* proportional velocity control mode */
/*9.1 Y轴添加*/
void Position_control_y(long position); /**(struct ThreeByte position);**/ /* Entry position control mode */
void Velocity_control_y(unsigned v);           /* proportional velocity control mode */
/*5.22 X轴添加*/
void Position_control_x(long position); /**(struct ThreeByte position);**/ /* Entry position control mode */
void Velocity_control_x(unsigned v);           /* proportional velocity control mode */
void Profile_z(char MaxVelocity,int Acceleration,long position);
struct ThreeByte input(char color,char back_color,int x,int y,char integer,char dot,int divalue);
struct ThreeByte LongToThreeByte(long lLongValue);
long int ThreeByteToLong(struct ThreeByte stThreeByte);
long ReadRegisterValue_Z(char cLowReg);
unsigned char ReadRAM(unsigned char page,unsigned int number);
void WriteRAM(unsigned char page,unsigned int number,unsigned char value);
void Read_Table(unsigned char group); /*把group所指定组别的数据从SRAM传递给全局变量Table*/
void Write_Table(unsigned char group); /*根据group所指定的组别，把全局变量Table的数据传递给SRAM*/
void Read_Nodes(unsigned char group); /*把group所指定组别的数据从SRAM传递给全局变量Node*/
void Write_Nodes(unsigned char group); /*根据group所指定的组别，把全局变量Node的数据传递给SRAM*/
void Init_IO(void);
void Check8255(void);
void Duty_ratio(unsigned int occupy,unsigned int empty,unsigned clk0);
void SetTable(char lines);    /*根据数据表格(全局结构Table)中的数据设置各放电参数*/
void PositionParameter(void);          /*更新全局变量position_z,position_d,position_p*/
long AbsLong(long Value);
char ProfileEnd(void);  /*Profile模式运行完毕返回1，否则返回0*/
void Electricity(void);
int KeyPress(void); /*没按键时返回0，有按键反回非0*/
int KeyAss(void);
void DefSeg(void);
void Key(void);
void char16x16(int iFrontColor,int iBackColor,int x,int y,int iChar);
void char16x24(int iFrontColor,int iBackColor,int x,int y,char cChar);
void char26x32(int iFrontColor,int iBackColor,int x,int y,int iChar);
void DisplayNumber(char cFontColor,char cBlackColor,int x,int y,char cDot,
                   long lNumberIn,char cChar);
void Display(char cFontColor,char cFontBack_color,int x,int y,
             char cDot,long lValue);
void DispLong_Small(char cFontColor,char cFontBack_color,int x,int y,
                    char cDot,long lValue);
void DispLong(char cFontColor,char cFontBack_color,int x,int y,
              char cDot,long lValue);
void DrawLevin(int iPosX,int iPosY,int iLongX,int iLongY,int iColor,int iBackColor);
void DrawColu(int iDepth,int iFlag);
void DispLogo(void);
void ConvexButton(int x1,int y1,int x2,int y2,int col);
void ConcaveButton(int x1,int y1,int x2,int y2,int col);
void GraphicButton(int iPosX1,int iPosY1,int iPosX2,int iPosY2);
void InvertGraphic(int iPosX1,int iPosY1,int iPosX2,int iPosY2);
void DispFace(void);
void AllButton(char cChar,int iFlag);
void Zero(void); /*归零功能函数*/
void High(void);  /*高度功能函数*/
void AlterMessage(void);/****信息****/
void OpenSound(void);
void CloseSound(void);
void ShortSound(void);
void EntrySleep(void);
void RiseAxes(void);/*上升功能*/
void Steel(void);/*刚蚀刚功能*/
char OilSwitch(void);
void Stop_z(char flag);
/*5.22 X轴添加*/
void Stop_x(char flag);
/*9.1 Y轴添加*/
void Stop_y(char flag);
long XPosi(char flag);/*读取X轴计数器的数据*/
long YPosi(char flag);/*读取Y轴计数器的数据*/
long ZPosi(char flag);/*读取Y轴计数器的数据*/
long ZPosi_origin(char flag);/*读取原点的值*/
void XYPosition(void);/*设置X、Y轴的位置*/
void ResetXYZ(void);
void ProcessTable(long lDeep,char cMaterial,int iCurrent,int iAcreage,char cEffect,char cShape,char cProcess,int mCurrent);
void MakeLN21P(char cMaterial,char cShape);
int MakeLN22P(int iCurrent);
int MakeLN23P(int iCurrent,char cProcess,char cEffect);
void ScreenProtect(void);
char Test(void);
void SetPassword(void);
void Password(void);
void DisplayNumber2(int cFontColor,int cBlackColor,int x,int y,char cDot,long lNumberIn,char cChar);
void StopError();
void RiseError();
void F13(void);
void InitSYS(unsigned);
#define aF1 0x3B
#define aF2 0x3C
#define aF3 0x3D
#define aF4 0x3E
#define aF5 0x3F
#define aF6 0x40
#define aF7 0x41
#define aF8 0x42
#define aF9 0x43
#define aF10 0x44
#define aF11 0x85
#define aF12 0x86
#define LEFT  0x4b
#define RIGHT 0x4d
#define UP 0x48
#define DN 0x50
#define SoundTIME 3
#define SDST (18*30)
#define TBST 2
extern void KeyNumber(int k);
extern unsigned char IB[];
extern char Fx;     /*1:按下F?,0数字母*/
extern char * DispKey(int zu,int fkey);
extern unsigned char ReadRAM(unsigned char page,unsigned int number);
extern char CopyRight[];
/*9.1归零添加*/
extern char C[10];
extern char F[10];
extern char K[10];
extern char P[15];
extern char keysound;
extern long K_3;
extern char TabWidth[14];
extern char TabWidth1[14],TabWidth2[14];
extern char Gaoya;
extern char GaoyaV;
extern char GaoyaA;
extern int LV[3];
extern char Other;
extern char Clip;
extern char Short;
extern char WcL;
extern char WcC;
extern unsigned char PWS[];
extern char Inputstr[8],Inputstr1[8];
extern char XYZ[];
extern char *Argv;
extern int Argc;
extern int date,date1;
extern long Zpointer;
extern char Fx;     /*1:按下F?,0数字母*/
extern char *JixinS[2];
extern char Group;/*当前组*/
extern char MaxL;
extern long KeyN,MaxN,MinN;
extern char F11_flag,F12_flag;
extern char XX,YY,Y7Y;
extern char SWF_flag;
extern char SWF_K;
extern char UP_K;
extern char DN_K;
extern char DNUP;
/*5.22 X轴添加*/
extern char LF_K;
extern char RT_K;
extern char LFRT;
/*9.1 Y轴添加*/
extern char FR_K;
extern char BK_K;
extern char FRBK;
extern long UDL;
extern char Pump_flag;
extern long MianJi;
extern char Sound_flag;
extern char V0ING;
extern char Steel_flag;
extern int steel,JX;
extern int F8H;
extern char F8T;
extern char Scr_flag;
extern int A;
extern int V;
extern int KEYL;
extern int SMessage;
extern int Initsystem;
extern char oldpage;
extern unsigned char buf[];
extern int ExitTime;
extern long ox,oy,oz,origin_z,origin_x,origin_y,diff_oz,diff_ox,diff_oy,lpz,s_OZh,s_OZl,F_positionz;
extern char ORgin;
extern int SoundTime;
extern char Ck8255_flag,Ck8256_flag;
extern char ZeroFlag;
extern char FireFlag,Pum_flag,Pumn_flag;
extern char PassWordF;
extern long Oldtime,Velocity_Time;
extern long Work_time,Up_time,V_time,Velocity;
extern long offset_x,offset_y,offset_z,offset_t,position_p,position_d;
extern long position_xdis,position_ydis,offset_d;
extern char offset_ds;
extern long lDeep;
extern char cEffect,cMaterial,cShape,cProcess;
extern int iCurrent;
extern int iAcreage;
extern char ERR_XY;
extern char Position_flag;
extern unsigned char BMP[];
extern struct Tab Table;
extern unsigned OldID,OldDATE;
extern unsigned AI,AITime,AIHigh;
extern void windows(int left,int top,int right,int bottom,int newattr);
extern void ShowTable(int lines);
extern void ShowKey(int kn,int ke);
extern char *KeyStr(int y,int x);
extern void Key(void);
extern void ShowSWF(void);
/*5.22 X轴添加*/
extern void Left(void);
extern void Right(void);
/*9.1 Y轴添加*/
extern void Front(void);
extern void Back(void);
extern void Down(void);
extern void Up(void);
extern void ShowXYZ(int flag);
extern void ShowDP(char flag);
extern char SetTime(long Time);
extern unsigned GetDate();
extern void ShowFlag(char flags);
extern void ShowHT(void);
extern void ShowMess(int value);
extern void ShowInput(char value);
extern long GetInput(void);
extern void InitKey(char flag);
extern void exitp(char flag);
extern char DispF6(int flag);
extern void ShowF6(int lines);
extern char DispF7(int flag);
extern void ShowF7(int lines);
extern void ShowF8(int lines);
extern void ShowF9(int lines);
extern void ShowFL(int flag);
extern void ShowPZ(int);
extern int CheckBMP(void);
extern void JiXin(int);
extern void ClearKey(int key);
extern void F8(char flag);
extern void F7(char flag);
extern void OpenScr(void);
extern void CloseScr(char flag);
extern int ProTime(void);
extern int Cmp(unsigned,unsigned);
extern unsigned NTN0(unsigned);
extern unsigned N0TN(unsigned);
extern void ResetCop(void);
extern void SetDianliu(unsigned dianliu);
extern void SaveXYZ(char flag);
extern long GetSTime(void);
extern void ShowTime(char flag);
extern void ShowTimeD(int flag);
extern void ShowVer(char *ver);
extern void ShowLong(long value);
extern void SaveBMP(void);
extern void Delay1(unsigned mill);
extern void hzk16f(unsigned char value,char * buf);
void Listbar3d(int x,int y,int x1,int y1,long color,unsigned char flag,unsigned sty,unsigned char w,char *str1);
#define __LNSTR1__ 2    /*精度*/
#define __LNSTR2__ 2    /*X轴计数方向*/
#define __LNSTR3__ 2    /*Y轴计数方向*/
#define __LNSTR4__ 2    /*Z轴计数方向*/
#define __LNSTR5__ 2    /*原点清零*/
#define __LNSTR6__ 3    /*电流*/
#define __LNSTR7__ 7    /*坐标轴运动位置设定*/
#define __LNSTR8__ 3        /*材料*/
#define __LNSTR9__ 4        /*效果*/
#define __LNSTR10__ 3       /*形状*/
#define __LNSTR11__ 2       /*制程*/
extern unsigned sl0,sl1,sl2,sl3,sl4,sl5,sl6,sl7,sl8,sl9;
char *StrLN1(char flag);
char *StrLN2(char flag);
char *StrLN3(char flag);
char *StrLN4(char flag);
char *StrLN5(char flag);
char *StrLN6(char flag);
char *StrLN7(char flag);
void ListStr1(int x,int y,char flag);
void ListStr2(int x,int y,char flag);
void ListStr3(int x,int y,char flag);
void ListStr4(int x,int y,char flag);
void ListStr5(int x,int y,char flag);
void ListStr6(int x,int y,char flag);
void ListStr7(int x,int y,char flag,char line);
void ListStr8(int x,int y,char flag);
void ListStr9(int x,int y,char flag);
void ListStr10(int x,int y,char flag);
void ListStr11(int x,int y,char flag);
void ListStrDeep(int x,int y,char flag);
void ListStrCur(int x,int y,char flag);
void ListStrAcr(int x,int y,char flag);
void ListStrXP(int x,int y,char flag);
void ListStrYP(int x,int y,char flag);
void ListStrZP(int x,int y,char flag);
extern void SetParameter(void);
extern void Init_coordinate(void);
extern void set_PID(void);
extern void set_PIDY(void);
extern void set_PIDX(void);
extern unsigned setline;
extern unsigned last_setline;
extern unsigned setline1;
extern unsigned last_setline1;
extern unsigned DZC,TDZCC;
extern unsigned Pid_kp,Pid_ki,Pid_kd,Pid_st;
/*2015.9.19 Kp参数修改*/
extern unsigned x_Pid_kp,y_Pid_kp,z_Pid_kp;
extern void Opens_V(unsigned v);
extern void Show_origin(long l1);
extern long origin;
long Read_Velocity_x(char flag);
long Read_Velocity_y(char flag);
long Read_Velocity_z(char flag);
extern char Origin_Flag;
extern long loz_init;
extern long dis_time;/*解密时间差*/
extern long Get_password_time(void);
void Write_spi(int number,unsigned long value);
unsigned long Read_spi(int number);
unsigned char ReadSPI(int number);
void WriteSPI(int number,unsigned char value);
