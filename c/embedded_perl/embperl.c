#include <EXTERN.h>
#include <perl.h>

PerlInterpreter *my_perl;

void
perl_sub(char* str)
{
  dSP;                                /* initialize stack pointer      */
  ENTER;                              /* everything created after here */
  SAVETMPS;                           /* ...is a temporary variable.   */
  PUSHMARK(SP);                       /* remember the stack pointer    */
  XPUSHs(sv_2mortal(newSVpv(str,0))); /* push the str onto the stack   */
  PUTBACK;                          /* make local stack pointer global */
  call_pv("perl_sub", G_SCALAR);      /* call the function             */
  SPAGAIN;                            /* refresh stack pointer         */
                                    /* pop the return value from stack */
  printf ("original string '%s'\nreversed string '%s'\n", str, POPp);
  PUTBACK;
  FREETMPS;                           /* free that return value        */
  LEAVE;                           /* ...and the XPUSHed "mortal" args.*/
}

int main (int argc, char **argv, char **env)
{
  if (argc < 2) {
    fprintf(stderr, "you must specify at least one argument\n");
    exit(0);
  }
  char *my_argv[] = { "", "perlscript.pl" };
  PERL_SYS_INIT3(&argc,&argv,&env);
  my_perl = perl_alloc();
  PERL_SET_CONTEXT(my_perl);
  perl_construct(my_perl);
  perl_parse(my_perl, NULL, 2, my_argv, (char **)NULL);
  PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
  perl_run(my_perl);
  perl_sub(argv[1]);
  perl_destruct(my_perl);
  perl_free(my_perl);
  PERL_SYS_TERM();
}
