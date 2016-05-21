#include <graphics.h>
#include <stdio.h>
#include "znc.h"
#include <alloc.h>
/*#include "svga16.h"
int huge DetectVGA16()
{
    return 3;
}
*/
InitScr()
{
    int  Gd = DETECT, Gm;
    int  Drv;
    char GrErr;
    int graphdriver,graphmode;
    /*  installuserdriver("Svga16",DetectVGA16);
        registerfarbgidriver(Svga16_fdriver);*/
    registerbgidriver(EGAVGA_driver);
    initgraph(&Gd,&Gm,"");
    /*  registerbgifont(triplex_font);*/
    registerbgifont(small_font);
    /*  registerbgifont(sansserif_font);*/
    /*  registerbgifont(gothic_font);*/
    GrErr = graphresult();
    if (GrErr != grOk)
    {
        printf("Graphics System Error: %s\n",grapherrormsg(GrErr));
        exit(1);
    }
    settextjustify(1,1);
    settextstyle(2,0,5);
    cleardevice();
    setviewport(0,0,getmaxx(),getmaxy(),0);
    /* buf1=hzk16f_A2(lb);  */
    return ;
}
