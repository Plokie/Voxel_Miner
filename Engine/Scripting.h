#pragma once

// i tried very hard to get C# Mono to work, but I couldnt!
// i think it had something to do with how the executable was set up
// it messed with some low level operating system stuff that just completely
// disallowed mono to read from the assembly correctly

// so i gave up, and caved in... fineeee, ill do luaaaa !

extern "C" {
	#include "../lua/include/lua.hpp"
	#include "../lua/include/lualib.h"
	#include "../lua/include/lauxlib.h"
}

//#include <lua.h>
//#include <lua.h>
//#include <lualib.h>
////#include "../lua/include/lauxlib.h" //why wont <luaxlib.h> work??
////#include <luaxlib.h>
#include <string>
#include <array>
#include <assert.h>
//
//#pragma comment(lib, "lua54.lib")


//#define ERRCHECK(L, id) L!=LUA_OK

using namespace std;

typedef void* (*lua_Alloc) (void* ud,
	void* ptr,
	size_t osize,
	size_t nsize);

typedef int (*lua_CFunction)(lua_State* L);

bool LuaOK(lua_State* state, int id);

class Scripting {
private:
	static Scripting* Instance;
	lua_State* state = nullptr;


private:
	//Lua-called functions
	static int CreateObject3D(lua_State* state);
	static int SetObject3DPos(lua_State* state);
	static int MoveObject3DPos(lua_State* state);
	static int SetObject3DRot(lua_State* state);
	static int RotObject3DRot(lua_State* state);
	static int AddObject3DModel(lua_State* state);

public:
	static void CallOnSceneLoad(const string& sceneName);

	int GetInt(lua_State* state, const string& name);
	
	template<typename T>
	size_t GetTypeId() {
		return typeid(T).hash_code();
	}
	template<typename... Ts> //Variad template of types
	void CallProc(lua_State* state, const string& name, int argCount, ...) {
		auto arr = std::array<size_t, sizeof...(Ts)>{ GetTypeId<Ts>()...  };

		va_list ptr;
		va_start(ptr, argCount);


		lua_getglobal(state, name.c_str());
		if(!lua_isfunction(state, -1))
			assert(false);

		for(int i = 0; i < argCount; i++) {
			switch(arr[i]) {
			case 12638226781420530164: {
				float argF = va_arg(ptr, float); //float
				lua_pushnumber(state, argF); break;
			}
			case 12638232278978672507: { //int
				int arg = va_arg(ptr, int);
				lua_pushnumber(state, arg); break;
			}
			case 17648624087129316105: {
				const char* argCC = va_arg(ptr, const char*); //const char*
				lua_pushstring(state, argCC); break;
			}
			case 10283618467285603348: {//string
				const string& arg = va_arg(ptr, string);
				lua_pushstring(state, arg.c_str()); break;
			}
			}


		}

		va_end(ptr);

		if(!LuaOK(state, lua_pcall(state, argCount, 0, 0)))
			assert(false);
	}

	Scripting();
	~Scripting();
};