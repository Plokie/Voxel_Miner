#pragma once

// i tried very hard to get C# Mono to work, but I couldnt!
// i think it had something to do with how the executable was set up
// it messed with some low level operating system stuff that just completely
// disallowed mono to read from the assembly correctly

// so i gave up, and caved in... fineeee, ill do luaaaa !

#include <lua.h>
//#include <lua.h>
//#include <lualib.h>
////#include "../lua/include/lauxlib.h" //why wont <luaxlib.h> work??
////#include <luaxlib.h>
#include <string>
//#include <assert.h>
//
#pragma comment(lib, "lua/lua54.lib")


//#define ERRCHECK(L, id) L!=LUA_OK

using namespace std;

typedef void* (*lua_Alloc) (void* ud,
	void* ptr,
	size_t osize,
	size_t nsize);

class Scripting {
private:
	static Scripting* Instance;
	lua_State* state;

public:


	int GetInt(lua_State* state, const string& name);
	

	Scripting();
};