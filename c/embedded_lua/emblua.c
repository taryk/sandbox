#include <stdlib.h>
#include <stdio.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
 
int main(int argc, char **argv)
{
  int s=0;
  lua_State *L = lua_open();

  // load the libs
  luaL_openlibs(L);

  //run a Lua scrip here
  luaL_dofile(L,"luascript.lua");

  lua_close(L);
  return 0;
}
