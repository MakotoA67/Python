#MAKEFILE

CC	= gcc
RM	= rm -f

#CFLAGS	= -g -Wall -O2
#CFLAGS	= -Wall -O2
#CFLAGS	= -Wall -O2 -DUSE_JPEGLIB
CFLAGS	= -Wall -O2 -DUSE_LIBTIFF

#LDFLAGS	= -lm -ltiff
#LDFLAGS	= -ltiff
#LDFLAGS	= -ljpeg -L. -lcolorbar
LDFLAGS	= -ltiff -L. -lcolorbar

#TARGET  = test
TARGET  = test libcolorbar.so
OBJS	= test.o

.c.o:
	${CC} -c ${CFLAGS} $<

all:	$(TARGET)

test: ${OBJS} libcolorbar.so
	${CC} -o $@ ${OBJS} ${LDFLAGS}

libcolorbar.so: colorbar.c
	${CC} -o $@ --shared $<

clean:
	${RM} ${TARGET} *.o *.so *~
