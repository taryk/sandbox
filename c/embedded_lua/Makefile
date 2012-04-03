.PHONY = clean

## lua5.0
# LIBS50 = `lua-config --include --libs`
LIBS50 = `pkg-config --libs --cflags lua50 lualib50`

## lua5.1
LIBS51 = `pkg-config --libs --cflags lua5.1`

sources = emblua.c emblua_single_context.c

lua51:
	gcc -std=c99 -Wall -g emblua.c -o emblua -DLUA51 $(LIBS51) -lpthread
	gcc -std=c99 -Wall -g emblua_single_context.c -o emblua_single_context -DLUA51 $(LIBS51) -lpthread

lua50:
	gcc -std=c99 -Wall -g emblua.c -o emblua -DLUA50 $(LIBS50) -lpthread
	gcc -std=c99 -Wall -g emblua_single_context.c -o emblua_single_context -DLUA50 $(LIBS50) -lpthread

all:	emblua

clean:
	rm -rf *.o emblua emblua_single_context
