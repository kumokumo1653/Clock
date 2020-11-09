TARGET = clock.exe

SRCS = clock.c Vec2.c Shape.c

OBJS = ${SRCS:.c=.o}

HEADERS = Vec2.h Shape.h 

CC = gcc
CCFLAGS = -Wall -I/usr/include/GL
LD = gcc
LDFLAGS = 
LIBS =  -lglpng -lglut -lGLU -lGL -lm

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