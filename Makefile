TARGET = j17404.exe

SRCS = clock.c Shape.c

OBJS = ${SRCS:.c=.o}

HEADERS = Vec2.h Shape.h 

CC = i686-pc-cygwin-gcc
CCFLAGS = -Wall -I/usr/include/opengl
LD = i686-pc-cygwin-gcc
LDFLAGS = 
LIBS =  -lglut32 -lglu32 -lopengl32  -lm icon.o

#OBJSからTARGETをつくる
$(TARGET) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET) $(LIBS)

#.cから.oをつくる
.c.o :
	$(CC) $(CCFLAGS) -c $<

#.oはHEAERSとMakefileに依存
$(OBJS) : $(HEADERS) Makefile

#make cleanの定義
clean : 
	rm -f $(TARGET) $(OBJS) core *~
