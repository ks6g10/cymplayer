#SRC=reader1.c
#OBJ=reader1.o
CC=gcc
CFLAGS=-O2 -fprofile-arcs
XFLAGS=`xml2-config --cflags`
GFLAGS= `pkg-config --cflags gtk+-2.0`
LIBS= `pkg-config --libs gtk+-3.0` `xml2-config --libs`

all:run

reader1.o : reader1.c
	$(CC) $(CFLAGS)  $(XFLAGS) $(GFLAGS) -g  -c reader1.c

test2.o: test2.c 
	$(CC) $(CFLAGS)   $(GFLAGS) -g -c test2.c

run: test2.o reader1.o
	$(CC) $(CFLAGS) $(LIBS) -DGTK_MULTIHEAD_SAFE=1 -g  -o run reader1.o test2.o
