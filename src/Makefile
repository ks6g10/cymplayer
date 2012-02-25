#SRC=reader1.c
#OBJ=reader1.o
CC=gcc
CFLAGS= -g
XFLAGS=`xml2-config --cflags`
GFLAGS= `pkg-config --cflags gtk+-3.0`
LIBS= `pkg-config --libs gtk+-3.0` `xml2-config --libs`
XLIBS= `xml2-config --libs`

all:cymplayer

reader1.o : reader1.c
	$(CC) $(CFLAGS)  $(XFLAGS) $(GFLAGS)  -c reader1.c

gtkmain.o: gtkmain.c 
	$(CC) $(CFLAGS)   $(GFLAGS) -c gtkmain.c

cymplayer: gtkmain.o reader1.o http.o streamfetch.o filehandler.o
	$(CC) $(CFLAGS) $(LIBS) -DGTK_MULTIHEAD_SAFE=1 -lcurl -o run reader1.o gtkmain.o http.o streamfetch.o filehandler.o

streamfetch.o:streamfetch.c http.o
	$(CC) $(CFLAGS) $(GFLAGS) -c streamfetch.c

filehandler.o:filehandler.c
	$(CC) $(CFLAGS) -c filehandler.c

http.o:http.c reader1.o
	$(CC) $(CFLAGS) $(GFLAGS) -lcurl -c http.c