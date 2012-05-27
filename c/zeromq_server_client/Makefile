.PHONY	: clean

CC=gcc
CFLAGS=`pkg-config --libs --cflags libczmq`
LIBS=

sources	= zmq_server.c zmq_client.c

zmq_server:
	$(CC) -g -Wall -std=gnu99 zmq_server.c -o zmq_server $(CFLAGS) $(LIBS)

zmq_client:
	$(CC) -g -Wall -std=gnu99 zmq_client.c -o zmq_client $(CFLAGS) $(LIBS)

all:	zmq_server zmq_client

clean:
	rm -rf *.o zmq_server zmq_client
