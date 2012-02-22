#SRC=reader1.c
#OBJ=reader1.o
CC=gcc
CFLAGS=-O2
XFLAGS=`xml2-config --cflags`
GFLAGS= `pkg-config --cflags gtk+-3.0`
LIBS= `pkg-config --libs gtk+-3.0` `xml2-config --libs`
XLIBS= `xml2-config --libs`

all:run

reader1.o : reader1.c
	$(CC) $(CFLAGS)  $(XFLAGS) $(GFLAGS)  -c reader1.c

test2.o: test2.c 
	$(CC) $(CFLAGS)   $(GFLAGS) -c test2.c

run: test2.o reader1.o http.o streamfetch.o
	$(CC) $(CFLAGS) $(LIBS) -DGTK_MULTIHEAD_SAFE=1 -lcurl -o run reader1.o test2.o http.o streamfetch.o
streamfetch.o:streamfetch.c http.o
	$(CC) $(CFLAGS) $(GFLAGS) -c streamfetch.c

http.o:http.c reader1.o
	$(CC) $(CFLAGS) $(GFLAGS) -lcurl -c http.c