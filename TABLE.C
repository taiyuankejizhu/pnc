#include "znc.h"
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <graphics.h>
#include <math.h>
#include <ctype.h>

FILE *FP;
char LN02A[90];
struct _TAB_ tabprocon[30];
char matile_arr[6] = {-1,-1,-1,-1,-1,-1};
char select[80];

double CNC_atof(const char *str){  /*字符串到浮点数的转换*/
  double l=0;
  char *temp,*temp1,*s;
  char *zz,*yy;
  int k=10;
  s=(char *) malloc(sizeof(strlen(str)+1));
  temp=(char *) malloc(sizeof(strlen(str)+1));
  zz=s;yy=temp;
  strcpy(s,str);
  while(!('+'==*s||'-'==*s||*s>='0'&&*s<='9'||'.'==*s))
    if(0==*s||'\0'==*s)
      return l;
    else
      *s++;
  if(('+'==*s||'-'==*s)&&!(*(s+1)>='0'&&*(s+1)<='9'))
    return l;
    temp1=temp;
  while('+'==*s||'-'==*s||*s>='0'&&*s<='9')
    {
       *temp++=*s++;
     }
     *temp='\0';
     l=(double)atol(temp1);
     if('.'==*s)
       {
          *s++;
          while(*s>='0'&&*s<='9')
            {
              if(l>=0)
                l+=(double)(*s++-'0')/k;
              else
                l-=(double)(*s++-'0')/k;
              k*=10;
            }
       }
       free(zz);
    	 free(yy);
       return l;
}

void MakeLN21P(char cMaterial,char cShape){  /*打开文件输入流*/
  register i,j;
  long l;
  long double d;
  char far *fn;
  char temp[5],Disp[100],temp1[15];
  switch(cMaterial){
    case 1:strcpy(temp1,"Cu-St");break;  /*Cu-St*/
    case 2:strcpy(temp1,"Gr-St");break;  /*Gr-St*/
    case 3:strcpy(temp1,"Cu-Wc");break;  /*Cu-WC*/
  }
  itoa(cShape,temp,10);
  strcat(temp1,temp);
  if(DZC==5)
    strcpy(Disp,"TAB5\\");
  else
    strcpy(Disp,"TAB1\\");
  strcat(Disp,temp1);strcat(Disp,".txt");
  FP=fopen(Disp,"rb");  /*打开文件*/
}

int MakeLN22P(int iCurrent){/*读取加工条件*/
  register s;
  fpos_t fi ,fj;
  int i,j;
  int ii,k,buttom,top,mianji;
  int flagl[6];/*0 - 1表示粗加工所取参数在文件中的范围，2 - 3 正常，4 - 5细*/
  char temp[10],ch,ch1,temp1[100],succed = 0;
  for(i = 0;i < 6;i++){flagl[i] = -1;matile_arr[i] = -1;}
  memset(LN02A ,0 ,sizeof(LN02A));
  for(i=0;i<30;i++){
    /*memset(tabprocon[i] ,0 ,sizeof(struct _TAB_));*/
    strncpy(tabprocon[i].no,"C103\0",5);
    strncpy(tabprocon[i].pl,"+\0",2);
    strncpy(tabprocon[i].ip,"003.0\0",6);
    strncpy(tabprocon[i].on,"095\0",4);
    strncpy(tabprocon[i].off,"040\0",4);
    strncpy(tabprocon[i].sv,"050\0",4);
    strncpy(tabprocon[i].s,"05\0",3);
    strncpy(tabprocon[i].up,"08\0",3);
    strncpy(tabprocon[i].dn,"08\0",3);
    strncpy(tabprocon[i].v,"02\0",3);
    strncpy(tabprocon[i].hv,"01\0",3);
    strncpy(tabprocon[i].lc,"00\0",3);
    strncpy(tabprocon[i].pp,"01\0",3);
    strncpy(tabprocon[i].lns,"0000\0",5);
    strncpy(tabprocon[i].step,"0.000\0",6);
    strncpy(tabprocon[i].lnm,"300\0",4);
    strncpy(tabprocon[i].b_ofs,"0.060\0",6);
    strncpy(tabprocon[i].s_ofs,"0.055\0",6);
    strncpy(tabprocon[i].ry,"025.0\0",6);
    strncpy(tabprocon[i].timer,"02\0",3);
  }

  if(FP==NULL)
    return -1;

  mianji = iCurrent;
  /*size_t fread( void* buffer, size_t size,size_t count, FILE* stream );
   返回实际读到的个数count*/

  ch=fgetc(FP);
  i = 0;
  while(ch!=EOF && !succed)
  {
       while(ch!=EOF && ch != '<')ch = fgetc(FP);
       /*int fseek(FILE *stream, long offset, int fromwhere); 参数3:0是头 1是当前，2是尾*/
       i = fread(temp1,1,100,FP);
       j = 0;
       if(j < i - 2 && temp1[j] == 'a'){/*判断面积*/
            j += 1;/*指向a后面一个字符*/
            while(!isdigit(temp1[j]))j++;
            s = 0;
            while(isdigit(temp1[j]))temp[s++] = temp1[j++];
            temp[s] = '\0';
            buttom = atoi(temp);
            while(!isdigit(temp1[j]))j++;
            s = 0;
            while(isdigit(temp1[j]))temp[s++] = temp1[j++];
            temp[s] = '\0';
            top = atoi(temp);
            if(mianji >= buttom && mianji <= top){
                fseek(FP,-98,1);
                fgetpos(FP ,&fj);
                /*i=fread(LN02A+8000,1,400,FP);*/
                /*List16x16M(CX0+40,CY0+9,0xf,LN02A+8000);*/
                /*List16x16M(CX0+40,CY0+9,0xf,itoa(fj ,LN02A+8000,10));*/
                i+=0;succed = 1;
            }
        }
        fseek(FP,-99,1);
        ch = fgetc(FP);
/*        fseek(FP,fj,0);
        i=fread(LN02A+8000,1,400,FP);
        List16x16M(CX0+40,CY0+9,0xf,itoa(fj ,LN02A+8000,10));*/
  }

  if(succed==0)
    return -1;

  fseek(FP,fj,0);
  while(fgetc(FP) != '<')fj++;
  fj--;
  fi=fj;
  fseek(FP,fj,0);

  while(flagl[1] == -1)/*粗<c> <ec*/
    {
        ch = fgetc(FP);
        if(ch!=EOF){
            if(ch == '<'){
                ch1 = fgetc(FP);
                if(ch1 == 'a'){/*下一精度范围的开始*/
                    fi = fj;
                }else if(ch1 == 'c'){/*开始*/
                    if(fgetc(FP) == '>'){
                        fgetpos(FP ,&fj);
                        flagl[0] = fj;
                    }
                }else if(ch1 == 'e'){/*结束*/
                    if(fgetc(FP) == 'c'){
                        fgetpos(FP ,&fj);
                        fj--;
                        flagl[1] = fj;
                    }
                }

            }else{
                fj++;
            }
        }
    }

  while(flagl[3] == -1)/*正常<n> <en*/
    {
        ch = fgetc(FP);
        if(ch!=EOF){
            if(ch == '<'){
                ch1 = fgetc(FP);
                if(ch1 == 'a'){/*下一精度范围的开始*/
                    fi = fj;
                }else if(ch1 == 'n'){/*开始*/
                    if(fgetc(FP) == '>'){
                        fgetpos(FP ,&fj);
                        flagl[2] = fj;
                    }
                }else if(ch1 == 'e'){/*结束*/
                    if(fgetc(FP) == 'n'){
                        fgetpos(FP ,&fj);
                        fj--;
                        flagl[3] = fj;
                    }
                }

            }else{
                fj++;
            }
        }
    }

  /*while(flagl[5] == -1)
    {
        ch = fgetc(FP);
        if(ch!=EOF){
            if(ch == '<'){
                ch1 = fgetc(FP);
                if(ch1 == 'a'){
                    fi = fj;
                }else if(ch1 == 'f'){
                    if(fgetc(FP) == '>'){
                        fgetpos(FP ,&fj);
                        flagl[4] = fj;
                    }
                }else if(ch1 == 'e'){
                    if(fgetc(FP) == 'f'){
                        fgetpos(FP ,&fj);
                        fj--;
                        flagl[5] = fj;
                    }
                }

            }else{
                fj++;
            }
        }
    }*/

  k = 0;
  s = 0;

  while(s < 5){
    if(flagl[s] != -1 && flagl[s+1] != -1){/*存在加工条件*/
        matile_arr[s] = k;
        for(fj=flagl[s];fj<flagl[s + 1]-82;){

            fseek(FP,fj,0);
            fi=fread(LN02A,1,82,FP);
            fj += fi;

            /*List16x16M(CX0+40,CY0+9,0xf,LN02A);*/
            /*List16x16M(CX0+40,CY0+9,0xf,tabprocon[k].no);*/
            /*List16x16M(CX0+80,CY0+9,0xf,tabprocon[k].ry);*/

            for(j = 0;j<82;){

                do{if(LN02A[j]==' '||LN02A[j]=='\t')j++;}
                while(LN02A[j]==' '||LN02A[j]=='\t');

                switch(LN02A[j]){
                case '(':
                    if(LN02A[j]=='(')while(LN02A[j++]!=';');
                    break;
                case 'C':
                case 'c':
                    ii=j;strncpy(tabprocon[k].no,LN02A+j,4);while(LN02A[j++]!=' ');tabprocon[k].no[j-ii-2]='\0';while(LN02A[j]==' ')j++;/*j++;*/while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].pl,LN02A+j,2);while(LN02A[j++]!=' ');tabprocon[k].pl[j-ii-1]='\0';while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].ip,LN02A+j,6);while(LN02A[j++]!=' ');tabprocon[k].ip[j-ii-1]='\0';while(LN02A[j]==' ')j++;
                    					
                    ii=j;strncpy(tabprocon[k].on,LN02A+j,4);while(LN02A[j++]!=' ');tabprocon[k].on[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].off,LN02A+j,4);while(LN02A[j++]!=' ');tabprocon[k].off[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].sv,LN02A+j,4);while(LN02A[j++]!=' ');tabprocon[k].sv[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].s,LN02A+j,3);while(LN02A[j++]!=' ');tabprocon[k].s[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].up,LN02A+j,4);while(LN02A[j++]!=' ');tabprocon[k].up[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].dn,LN02A+j,3);while(LN02A[j++]!=' ');tabprocon[k].dn[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].v,LN02A+j,3);while(LN02A[j++]!=' ');tabprocon[k].v[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].hv,LN02A+j,3);while(LN02A[j++]!=' ');tabprocon[k].v[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].lc,LN02A+j,3);while(LN02A[j++]!=' ');tabprocon[k].lc[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].pp,LN02A+j,3);while(LN02A[j++]!=' ');tabprocon[k].pp[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].lns,LN02A+j,1);while(LN02A[j++]!=' ');tabprocon[k].lns[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].step,LN02A+j,6);while(LN02A[j++]!=' ');tabprocon[k].step[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].lnm,LN02A+j,4);while(LN02A[j++]!=' ');tabprocon[k].lnm[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].b_ofs,LN02A+j,6);while(LN02A[j++]!=' ');tabprocon[k].b_ofs[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].s_ofs,LN02A+j,6);while(LN02A[j++]!=' ');tabprocon[k].s_ofs[j-ii-1]=0;while(LN02A[j]==' ')j++;

                    ii=j;strncpy(tabprocon[k].ry,LN02A+j,5);while(LN02A[j]!=' '&&LN02A[j]!=';')j++;tabprocon[k].ry[j-ii]=0;while(LN02A[j]==' ')j++;
                    					
                    while(LN02A[j++]!=';');k++;
                    break;
                default:j++;
                }

                do{if(LN02A[j]=='\r'||LN02A[j]=='\n')j++;}
                while(LN02A[j]=='\r'||LN02A[j]=='\n');
            }

        }
        matile_arr[s+1] = k - 1;
    }
    s += 2;
  }
  /*procon.fileline=k;*/
  fclose(FP);
  return k;
}

int MakeLN23P(int iCurrent,char cProcess,char cEffect){
  register i,j,k;
  int top,but,topline;
  int line0=-1,line1=-1;
  char temp[80];
  double d;

  if(cProcess==1){
    but = matile_arr[0];
    top = matile_arr[1];
  }else{
    but = matile_arr[2];
    top = matile_arr[3];
  }

  for(j=but;j<top&&j>=0;j++){
    d=CNC_atof(tabprocon[j].ip);
    if(d<iCurrent){
            line0=j-1;
            break;
    }else if(d==iCurrent){
        line0=j;
        break;
    }
  }

  if(line0==-1)return 0;
  else{ 
    line1=top;
    line1-=(5-cEffect);
    line1+=1;
    if(line1==-1)return 0;
  }

  if(line1!=-1)
  {
    for(i=0,j=line0;j<line1+1;j++,i++){
        select[i]=j;
    }
    topline=i;
  }
  else{
    memset(LN02A ,0 ,sizeof(LN02A));
    return -1;
  }
  return topline;
}

long CNC_atol(const char *str) /*字符串到长整的转换*/
{
    long l;
    long i;
    char *zheng;
    char *xiao;
    char *cur;
    char *zz,*yy;
    zheng=(char *) malloc(sizeof(strlen(str)+1));
    xiao=(char *) malloc(sizeof(strlen(str)+1));
    zz=zheng;yy=xiao;
    cur=zheng;
    while(*str!='\0')
    {
        if(*str==46)
        {
            *cur='\0';
            cur=xiao;
            str++;
            continue;
        }
        *cur=*str;
        cur++;
        str++;
    }
    *cur='\0';
    l=atoi(zheng);
    l*=1000;
    i=atoi(xiao);
    l+=i;
    free(zz);
    free(yy);
    return l;
}
void ProcessTable(long lDeep,char cMaterial,int iCurrent,int iAcreage,char cEffect,char cShape,char cProcess,int mCurrent)
{
    long l=0,d=0;
    register int i,j,k;              /*循环索引*/
    char cLineNumber;                /*确定要显示的行数*/
    struct Tab stTemp;
    int iBai=0;
    MakeLN21P(cMaterial,cShape);
    if(mCurrent<0)
    {
        if(iCurrent<1)
        {
            iCurrent=1;
        }
        if(iCurrent>191)
        {
            iCurrent=191;
        }
        if(MakeLN22P(iCurrent)>0)
        {
            cLineNumber=MakeLN23P(iCurrent,cProcess,cEffect);
        }
        else
        {
            /*ShowMess(57);*/
            cLineNumber=0;
        }
        if(cLineNumber>15)
        {
            cLineNumber=15;
        }
    }
    else
    {
        if(mCurrent<1)
        {
            mCurrent=1;
        }
        if(mCurrent>191)
        {
            mCurrent=191;
        }
        if(MakeLN22P(mCurrent)>0)
        {
            cLineNumber=MakeLN23P(mCurrent,cProcess,4);
        }
        else
        {
            /*ShowMess(57);*/
            cLineNumber=0;
        }
        if(cLineNumber>15)
        {
            cLineNumber=15;
        }
    }
    for(i=0,k=0; i<cLineNumber; i++,k++)
    {
        stTemp.Index[i]=1;
        l=0;
        d=0;
        for(j=i; j<cLineNumber-1; j++)
        {
            /*d=1000*CNC_atof(tabprocon[select[j]].b_ofs);*/
            d=CNC_atol(tabprocon[select[j]].b_ofs);
            l+=d;
        }
        if(!K[8])
        {
            stTemp.Shendu[i]=(lDeep+l)/DZC;
        }
        else
        {
            stTemp.Shendu[i]=(lDeep-l)/DZC;
        }
        if(tabprocon[select[k]].pl[0]=='+')
        {
            stTemp.Jixin[i]=0;
        }
        else
        {
            stTemp.Jixin[i]=1;
        }
        stTemp.Dianliu[i]=2*CNC_atof(tabprocon[select[k]].ip);
        stTemp.Maikuan[i]=CNC_atof(tabprocon[select[k]].on);
        stTemp.Xiuzhi[i]=CNC_atof(tabprocon[select[k]].off);
        stTemp.Jianxi[i]=CNC_atof(tabprocon[select[k]].sv);
        stTemp.Sudu[i]=CNC_atof(tabprocon[select[k]].s);
        iBai=(iAcreage/10-(cLineNumber-i-1))*100;
        if(iBai<0)
        {
            iBai=0;
        }
        stTemp.Shenggao[i]=iBai+CNC_atof(tabprocon[select[k]].up);
        /*stTemp.Shenggao[i]=iAcreage*(cLineNumber-i)/cLineNumber;*/
        stTemp.Gongshi[i]=CNC_atof(tabprocon[select[k]].dn);
        stTemp.LV[i]=CNC_atof(tabprocon[select[k]].v);
        stTemp.Gaoya[i]=CNC_atof(tabprocon[select[k]].hv);;
        stTemp.PP[i]=CNC_atof(tabprocon[select[k]].pp);
        stTemp.WcLc[i]=CNC_atof(tabprocon[select[k]].lc);
    }
    for(i=cLineNumber; i<15; i++)
    {
        stTemp.Index[i]=0;
        stTemp.Shendu[i]='?';
        stTemp.Dianliu[i]='?';
        stTemp.Maikuan[i]='?';
        stTemp.Xiuzhi[i]='?';
        stTemp.Jianxi[i]='?';
        stTemp.Sudu[i]='?';
        stTemp.Shenggao[i]='?';
        stTemp.Gongshi[i]='?';
        stTemp.Gaoya[i]='?';
        stTemp.LV[i]='?';
        stTemp.Jixin[i]='?';
        stTemp.PP[i]='?';
        stTemp.WcLc[i]='?';
    }
    if(mCurrent<0)
    {
        stTemp.Dianliu[0]=2*iCurrent;
        Table=stTemp;
    }
    else
    {
        i=0;
        while(stTemp.Index[i]==1)
        {
            if(stTemp.Dianliu[i]<=iCurrent*2)
            {
                break;
            }
            i++;
        }
        if(stTemp.Index[i]!=1)
            i=i-1;
        if(K[8])
        {
            StrTable.Shendu[5]=CNC_atol(tabprocon[select[0]].b_ofs)/DZC;
        }
        else
        {
            StrTable.Shendu[5]=CNC_atol(tabprocon[select[0]].b_ofs)/DZC;
            StrTable.Shendu[5]=0-StrTable.Shendu[5];
        }
        StrTable.Index[5]=1;
        StrTable.Jixin[5]=stTemp.Jixin[i];
        StrTable.Dianliu[5]=stTemp.Dianliu[i];
        StrTable.Maikuan[5]=stTemp.Maikuan[i];
        StrTable.Xiuzhi[5]=stTemp.Xiuzhi[i];
        StrTable.Jianxi[5]=stTemp.Jianxi[i];
        StrTable.Sudu[5]=stTemp.Sudu[i];
        StrTable.Shenggao[5]=stTemp.Shenggao[i];
        StrTable.Gongshi[5]=stTemp.Gongshi[i];
        StrTable.Gaoya[5]=stTemp.Gaoya[i];
        StrTable.LV[5]=stTemp.LV[i];
        StrTable.PP[5]=stTemp.PP[i];
        StrTable.WcLc[5]=stTemp.WcLc[i];
        StrTable.Dianliu[5]=2*iCurrent;
    }
    ShowTable(0);
}
