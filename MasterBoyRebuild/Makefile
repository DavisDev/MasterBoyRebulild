##  Escribimos un nombre cualquiera para nuestro proyecto
TARGET = MasterBoy

##  Aqui especificamos los archivos que deben compilarse y agregarse
OBJS = 	gbcore/apu.o gbcore/cheat.o gbcore/cpu.o gbcore/gb.o gbcore/lcd.o gbcore/mbc.o gbcore/rom.o gbcore/sgb.o \
		cpu/z80.o \
		memz80.o	\
		pio.o		\
		vdp.o		\
		render.o	\
		sms.o		\
		system.o	\
		sound/sound.o	\
		sound/sn76489.o	\
		sound/emu2413.o	\
		sound/ym2413.o	\
		sound/fmintf.o	\
		sound/stream.o \
		psp/gameboy_render.o \
		psp/VideoGu.o	\
		psp/Controls.o	\
		psp/PspSound.o	\
		psp/SMS.o \
		psp/main.o \
		psp/menuplus.o \
		psp/menuplus2.o \
		psp/config.o \
		psp/sound.o \
		fileio.o	\
		state.o		\
		loadrom.o \
		unzip/ioapi.o	\
		unzip/unzip.o \
		psp/gym/bzlib/blocksort.o psp/gym/bzlib/bzlib.o psp/gym/bzlib/compress.o psp/gym/bzlib/crctable.o \
		psp/gym/bzlib/decompress.o psp/gym/bzlib/huffman.o psp/gym/bzlib/randtable.o \
		psp/gz.o psp/VirtualGz.o psp/deflate.o psp/gzio.o \
		error.o
		


## exportador en el bat
## psp-build-exports -s exports.exp

PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin

PSP_FW_VERSION=371

CFLAGS = -O2 -G0 -Wall -g
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

##  Agregamos aqui librerias necesarias para que nuestro proyecto compile
##  Como vamos iniciando, hacen falta las de Lua
##  Básicamente, si el code no compila y avisa que alguna función no existe, es probable que nos haga falta una libreria aqui
LIBDIR = 

OSLIBS = -losl -lpng -lz \
         -lpsphprm -lpspsdk -lpspctrl -lpspumd -lpsprtc -lpsppower -lpspgu -lpspgum -lpspaudiocodec  -lpspaudiolib -lpspaudio -lpsphttp -lpspssl -lpspwlan \
         -lpspnet_adhocmatching -lpspnet_adhoc -lpspnet_adhocctl -lm -ljpeg -lpspusbstor
LIBS = $(OSLIBS) -lstdc++ -llua -lm 
#-lunziprar

LDFLAGS = -Wl,-Map -Wl,MasterBoy.map
## Extra ram use
PSP_LARGE_MEMORY = 1
## Exportar funciones del nucleo
BUILD_PRX = 1
#PRX_EXPORTS = exports.exp
##  Aqui especificamos que queremos un EBOOT.PBP, y le damos un titulo para mostrar en el XMB
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = MasterBoy Rebuild v2.02

PSP_EBOOT_ICON = icon0.png
PSP_EBOOT_PIC1 = pic1.png
PSP_EBOOT_SND0 = snd0.at3

##  Descomentando las ultimas 2 lineas podemos agregar icon0 y pic1 al EBOOT generado
## icon0: PNG, 144x80pix
##  pic1: PNG, 480x272pix
#PSP_EBOOT_ICON = 
#PSP_EBOOT_PIC1 = 

include $(PSPSDK)/lib/build.mak