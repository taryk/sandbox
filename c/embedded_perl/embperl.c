#include <EXTERN.h>
#include <perl.h>
#include <pthread.h>

#define NUM_THREADS  4

char * perl_scripts[NUM_THREADS] = { "perlscript_one.pl"   ,
                                     "perlscript_two.pl"   ,
                                     "perlscript_three.pl" ,
                                     "perlscript_four.pl"  };

static void xs_init (pTHX);
 
EXTERN_C void boot_DynaLoader (pTHX_ CV* cv);
 
EXTERN_C void
xs_init(pTHX)
{
  char *file = __FILE__;
  /* DynaLoader is a special case */
  newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file);
}

void
perl_sub(PerlInterpreter *my_perl, char* str)
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
  printf ("original string '%s'\nprocessed string '%s'\n", str, POPp);
  PUTBACK;
  FREETMPS;                           /* free that return value        */
  LEAVE;                           /* ...and the XPUSHed "mortal" args.*/
}

void *
create_perl_context(void *threadid) 
{
  int tid;
  tid = (int)threadid;
  char *my_argv[] = { "", perl_scripts[tid] };
  PerlInterpreter *my_perl;
  my_perl = perl_alloc();
  PERL_SET_CONTEXT(my_perl);
  perl_construct(my_perl);
  perl_parse(my_perl, xs_init, 2, my_argv, (char **)NULL);
  PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
  perl_run(my_perl);
  perl_sub(my_perl, "hello world");
  perl_destruct(my_perl);
  perl_free(my_perl);
}

int main(int argc, char **argv, char **env)
{
  //if (argc < 2) {
  //  fprintf(stderr, "you must specify at least one argument\n");
  //  exit(0);
  //}
  pthread_t threads[NUM_THREADS];
  PERL_SYS_INIT3(&argc,&argv,&env);
  int t;
  for (t=0; t<NUM_THREADS; t++) {
    printf("In main: creating thread %d\n", t);
    (void)pthread_create(&threads[t], NULL, create_perl_context, (void *)t);
  }
  for (t=0;t<NUM_THREADS;t++) {
    (void)pthread_join(threads[t], NULL);
    printf("joined thread %d\n", t);
  }
  pthread_exit(NULL);
  PERL_SYS_TERM();
}
