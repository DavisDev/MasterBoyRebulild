PATH=C:\pspsdk\bin\;C:\pspsdk\psp\bin\
make -f makefile > log.txt 2>&1
del *.o 
del cpu\*.o 
del gbcore\*.o 
del lib\*.o 
del psp\*.o 
del psp\vgm\*.o
del psp\gym\*.o 
del psp\gym\bzlib\*.o 
del psp\gym\zlib\*.o sound\*.o
del unzip\*.o
del *.elf *.sfo 
start log.txt
exit
