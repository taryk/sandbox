#include <EXTERN.h>
#include <perl.h>
#include <pthread.h>

#define NUM_THREADS  3

#define PERL_SCRIPT "perlscript.pl"

char * perl_subs[NUM_THREADS] = { "perl_sub_one"   ,
                                  "perl_sub_two"   ,
                                  "perl_sub_three" };

PerlInterpreter *my_perl;

pthread_mutex_t mutex_perl;

void
perl_sub(int tid, char* str)
{
  dSP;                                /* initialize stack pointer      */
  ENTER;                              /* everything created after here */
  SAVETMPS;                           /* ...is a temporary variable.   */
  PUSHMARK(SP);                       /* remember the stack pointer    */
  XPUSHs(sv_2mortal(newSVpv(str,0))); /* push the str onto the stack   */
  PUTBACK;                          /* make local stack pointer global */
  call_pv(perl_subs[tid], G_SCALAR);      /* call the function         */
  SPAGAIN;                            /* refresh stack pointer         */
                                    /* pop the return value from stack */
  printf ("original string  '%s'\nprocessed string '%s'\n", str, POPp);
  PUTBACK;
  FREETMPS;                           /* free that return value        */
  LEAVE;                           /* ...and the XPUSHed "mortal" args.*/
}

void *
thread_context(void *threadid) 
{
  int tid;
  tid = (int)threadid;
  pthread_mutex_lock(&mutex_perl);
  perl_sub(tid, "hello world");
  pthread_mutex_unlock(&mutex_perl);
}

int main(int argc, char **argv, char **env)
{
  //if (argc < 2) {
  //  fprintf(stderr, "you must specify at least one argument\n");
  //  exit(0);
  //}
  pthread_t threads[NUM_THREADS];
  pthread_mutex_init(&mutex_perl, NULL);
  PERL_SYS_INIT3(&argc,&argv,&env);
  char *my_argv[] = { "", PERL_SCRIPT };
  my_perl = perl_alloc();
  PERL_SET_CONTEXT(my_perl);
  perl_construct(my_perl);
  perl_parse(my_perl, NULL, 2, my_argv, (char **)NULL);
  PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
  perl_run(my_perl);
  int t;
  for (t=0; t<NUM_THREADS; t++) {
    printf("creating thread %d\n", t);
    (void)pthread_create(&threads[t], NULL, thread_context, (void *)t);
  }
  for (t=0;t<NUM_THREADS;t++) {
    (void)pthread_join(threads[t], NULL);
    printf("joined thread %d\n", t);
  }
  perl_destruct(my_perl);
  perl_free(my_perl);
  pthread_exit(NULL);
  pthread_mutex_destroy(&mutex_perl);
  PERL_SYS_TERM();
}
