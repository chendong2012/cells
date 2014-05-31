#DEBUG  = -g -O0
#DEBUG   = -O3
CC      = g++
INCLUDEPATH = -I/usr/local/include -I.
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS = -L/usr/local/lib
SRC=comm.cpp rfm70.cpp rfthread.cpp rftimer.cpp user_activity.cpp
OBJS=comm.o rfm70.o rfthread.o rftimer.o user_activity.o

all:rf

rf:   $(OBJS) 
	@echo [link]
	ar rvs librf.a $^
	rm -f *.o
	cp librf.a ../raspberrypi-rfm70-library/
#.c.o:
%.o:%.c
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $< -o $@
clean:
	rm -f *~ core tags *.a *.o

tags:   $(SRC)
	@echo [ctags] ls
	@ctags $^

depend:
	makedepend -Y $(SRC)
