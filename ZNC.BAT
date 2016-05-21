@echo off
tcc  -C -1 -mh -c -f87 main.c
tcc  -C -1 -mh -c -f87 table.c
tcc  -C -1 -mh -c -f87 bmp.c
tcc  -C -1 -mh -c -f87 svga256.c
tcc  -C -1 -mh -c -f87 graphic.c
tcc  -C -1 -mh -c -f87 nc.h
tcc  -C -1 -mh -c -f87 disp.c
tcc  -C -1 -mh -c -f87 key.c
tlink  @nc.lnk
rem bmptoexe nc.exe nhf.bmp
