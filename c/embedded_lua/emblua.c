#include <stdlib.h>
#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
 

void lua_func(lua_State *L, char *str)
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

int main(int argc, char **argv)
{
  int s=0;
  lua_State *L = lua_open();

  // load the libs (new in lua 5.1)
  luaL_openlibs(L);

  // run a Lua script
  luaL_dofile(L,"luascript.lua");
  // call the `lua_func`
  lua_func(L,"hello world");
  lua_close(L);
  return 0;
}
