#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <pthread.h> 

#define NUM_THREADS 3

char * lua_scripts[NUM_THREADS] = { "luascript_one.lua"   ,
                                    "luascript_two.lua"   ,
                                    "luascript_three.lua" };


void lua_func(lua_State *L, unsigned int tid, char *str)
{
  char *result;
    
  /* push functions and arguments */
  lua_getglobal(L, "lua_func");  /* function to be called */
  lua_pushstring(L, str);        /* push 1st argument */

  /* do the call (1 arguments, 1 result) */
  if (lua_pcall(L, 1, 1, 0) != 0)
    error(L, "error running function `lua_func': %s",
             lua_tostring(L, -1));

  /* retrieve result */
  if (!lua_isstring(L, -1))
    error(L, "function `lua_func' must return a number");
  result = lua_tostring(L, -1);
  lua_pop(L, 1);  /* pop returned value */
  printf ("original string  '%s'\nprocessed string '%s'\n", str, result);
  return result;
}

void *
create_lua_context(void *threadid)
{
  unsigned int tid;
  tid = (unsigned int)threadid;
  lua_State *L = lua_open();
  
  // load the libs
#ifdef LUA51
  // lua 5.1
  luaL_openlibs(L);
#else
  // lua 5.0
  luaopen_base(L);             /* opens the basic library */
  luaopen_table(L);            /* opens the table library */
  luaopen_io(L);               /* opens the I/O library */
  luaopen_string(L);           /* opens the string lib. */
  luaopen_math(L);             /* opens the math lib. */
#endif

  // run a Lua script
  if (luaL_loadfile(L, lua_scripts[tid]) || lua_pcall(L, 0, 0, 0))
    error(L, "cannot run the lua script: %s",
             lua_tostring(L, -1));

  // call the `lua_func`
  lua_func(L, tid, "hello world");
  lua_close(L);
}

int main(int argc, char **argv)
{
  pthread_t threads[NUM_THREADS];
  unsigned int t;
  for (t=0; t<NUM_THREADS; t++) {
    printf("In main: creating thread %d\n", t);
    (void)pthread_create(&threads[t], NULL, create_lua_context, (void *)t);
  }
  for (t=0;t<NUM_THREADS;t++) {
    (void)pthread_join(threads[t], NULL);
    printf("joined thread %d\n", t);
  }
  pthread_exit(NULL);
  return 0;
}
