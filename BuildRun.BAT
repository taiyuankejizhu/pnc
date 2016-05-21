del nc.exe
del main.obj
del nc.map
del nc.obj
del svga256.obj
del table.obj
del key.obj
del disp.obj
del bmp.obj
del graphic.obj
@echo off
tcc  -C -1 -mh -c -f87 main.c
tcc  -C -1 -mh -c -f87 key.c
tcc  -C -1 -mh -c -f87 table.c
tcc  -C -1 -mh -c -f87 bmp.c
tcc  -C -1 -mh -c -f87 svga16.c
tcc  -C -1 -mh -c -f87 graphic.c
tcc  -C -1 -mh -c -f87 nc.h
tcc  -C -1 -mh -c -f87 disp.c
tcc  -C -1 -mh -c -f87 key.c
tlink  @nc.lnk
rem bmptoexe nc.exe nhf.bmp
nc.exe
