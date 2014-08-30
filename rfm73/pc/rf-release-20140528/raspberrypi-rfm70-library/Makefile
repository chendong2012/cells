#DEBUG  = -g -O0
#DEBUG   = -O3
CC      = g++
INCLUDEPATH = -I/usr/local/include -I.
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS = -L/usr/local/lib
LIBS    = -lwiringPi

SRC     =       rfm70_test.c

OBJ     =       rfm70_test.o
OBJS	=	comm.o  rfm70.o  rfthread.o  rftimer.o  user_activity.o

all:            rfm70 rfm70_test

rfm70:          rfm70.o
	@echo [link]
#	$(CC) -c rfm70.cpp -o librfm70.o $(LDFLAGS) $(LIBS)
#	$(CC) -c user_activity.cpp comm.cpp rftimer.cpp rfthread.cpp -I. $(LDFLAGS) -L. $(LIBS) -lrt -lpthread
#	ar rvs librfm70.a $(OBJS)

rfm70_test:        rfm70_test
	@echo [link]
#	$(CC) u1.cpp u2.cpp user_activity.cpp rfm70_test.cpp comm.cpp rftimer.cpp rfthread.cpp -o $@ -I. $(LDFLAGS) -L. $(LIBS) -lrfm70  -lrt -lpthread
	$(CC) u1.cpp u11.cpp rfm70_test.cpp ISend.cpp IReceive.cpp time_server.cpp -o $@ -I. $(LDFLAGS) -L. $(LIBS) -lrf  -lrt -lpthread
.c.o:
	@echo [CC] $<
	@$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) *~ core tags rfm70_test

tags:   $(SRC)
	@echo [ctags]
	@ctags $(SRC)

depend:
	makedepend -Y $(SRC)
