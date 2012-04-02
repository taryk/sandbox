.PHONY = clean

LIBS = `perl -MExtUtils::Embed -e ccopts -e ldopts`
CFLAGS =

sources = embperl.c embperl_single_context.c

embperl:
	/usr/bin/gcc -Wall -o embperl embperl.c $(LIBS) $(CFLAGS)

embperl_single_context:
	/usr/bin/gcc -Wall -o embperl_single_context embperl_single_context.c $(LIBS) $(CFLAGS)

all: embperl embperl_single_context

clean:
	rm -rf *.o embperl embperl_single_context
