.PHONY = clean

# lua5.0: lua-config --include --libs
LIBS = `pkg-config --libs --cflags lua5.1`

sources = emblua.c

emblua:
	gcc -Wall -g emblua.c -o emblua $(LIBS)

all:	emblua

clean:
	rm -rf *.o emblua
