CC=g++

OBJS=word_stock.o get_zimo.o
TARGET=zimo

$(TARGET):$(OBJS)
	$(CC) $(OBJS) -O3 -o $@

#%.o:%.cpp
#	$(CC) -c -O3 $@ $<

word_stock.o:word_stock.cpp word_stock.h
	$(CC) -c word_stock.cpp -o word_stock.o

get_zimo.o:get_zimo.cpp word_stock.h
	$(CC) -c get_zimo.cpp -o get_zimo.o

.PHONY:clean
clean:
	-rm -f *.o $(TARGET)
