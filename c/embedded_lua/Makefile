.PHONY = clean

## lua5.0
# LIBS50 = `lua-config --include --libs`
LIBS50 = `pkg-config --libs --cflags lua50 lualib50`

## lua5.1
LIBS51 = `pkg-config --libs --cflags lua5.1`

sources = emblua.c

lua51:
	gcc -Wall -g emblua.c -o emblua -DLUA51 $(LIBS51)

lua50:
	gcc -Wall -g emblua.c -o emblua -DLUA50 $(LIBS50)

all:	emblua

clean:
	rm -rf *.o emblua
