#pragma once

// i tried very hard to get C# Mono to work, but I couldnt!
// i think it had something to do with how the executable was set up
// it messed with some low level operating system stuff that just completely
// disallowed mono to read from the assembly correctly

// so i gave up, and caved in... fineeee, ill do luaaaa !

#pragma warning( push )
#pragma warning( disable : 28020)

extern "C" {
	#include "../lua/include/lua.hpp"
	#include "../lua/include/lualib.h"
	#include "../lua/include/lauxlib.h"
}

#include <string>
#include <array>
#include <assert.h>
#include <vector>
#include <functional>
#include <map>

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

	string currentScene;

	/// <summary>
	/// Updates the Lua-side Engine variable containing information
	/// </summary>
	void UpdateLuaEngineInformation(lua_State* state);

private:
	//Lua-called functions
	static int CreateObject3D(lua_State* state);
	static int SetObject3DPos(lua_State* state);
	static int SetObject3DRot(lua_State* state);
	static int AddObject3DModel(lua_State* state);
	static int SetModelTexture(lua_State* state);
	static int LoadMesh(lua_State* state);
	static int LoadTexture(lua_State* state);
	static int GetObject3D(lua_State* state);

	static int DebugMessage(lua_State* state);


	map<string, function<int(lua_State*)>> funcs = {};
	static int CallGameFunction(lua_State* state);

public:
	static void CallOnSceneLoad(const string& sceneName);

	static void RegisterLuaFunc(const string& name, function<int(lua_State*)> func);

	void Update(float deltaTime);
	
	template<typename T>
	static size_t GetTypeId() {
		return typeid(T).hash_code();
	}
	template<typename... Ts> //Variad template of types
	void CallProc(lua_State* state, const string& name, int argCount, ...) {
		// turn type inputs into array of hash types
		auto arr = std::array<size_t, sizeof...(Ts)>{ GetTypeId<Ts>()...  };

		va_list ptr; // start variad iterator pointer
		va_start(ptr, argCount);


		lua_getglobal(state, name.c_str()); // push global function to stack
		if(!lua_isfunction(state, -1)) // make sure its a function
			assert(false);

		for(int i = 0; i < argCount; i++) {
			switch(arr[i]) { //check against valid hash codes
			case 12638226781420530164: { //float
				float arg = va_arg(ptr, float);
				lua_pushnumber(state, arg); 
			} break;
			case 12638230079955414429: { //double
				double arg = va_arg(ptr, double);
				lua_pushnumber(state, arg);
			} break;
			case 12638232278978672507: { //int
				int arg = va_arg(ptr, int);
				lua_pushnumber(state, arg); 
			} break;
			case 17648624087129316105: { //const char*
				const char* argCC = va_arg(ptr, const char*); 
				lua_pushstring(state, argCC); 
			} break;
			case 10283618467285603348: {//string
				const string& arg = va_arg(ptr, string);
				lua_pushstring(state, arg.c_str()); 
			} break;
			}


		}

		va_end(ptr);

		// call function
		if(!LuaOK(state, lua_pcall(state, argCount, 0, 0)))
			assert(false);

		//lua_pop(state, argCount); //pop parameters
	}

	template<typename... Ts>
	static void CallEvent(const string& eventName, int argCount, ...) {
		auto arr = std::array<size_t, sizeof...(Ts)>{ GetTypeId<Ts>()...  };

		va_list ptr; // start variad iterator pointer
		va_start(ptr, argCount);

		lua_getglobal(Instance->state, "Backend_EventCall"); // push global function to stack
		if(!lua_isfunction(Instance->state, -1)) // make sure its a function
			assert(false);

		lua_pushstring(Instance->state, eventName.c_str());
#
		for(int i = 0; i < argCount; i++) {
			switch(arr[i]) { //check against valid hash codes
			case 12638226781420530164: { //float
				float arg = va_arg(ptr, float);
				lua_pushnumber(Instance->state, arg);
			} break;
			case 12638230079955414429: { //double
				double arg = va_arg(ptr, double);
				lua_pushnumber(Instance->state, arg);
			} break;
			case 12638232278978672507: { //int
				int arg = va_arg(ptr, int);
				lua_pushnumber(Instance->state, arg);
			} break;
			case 17648624087129316105: { //const char*
				const char* argCC = va_arg(ptr, const char*);
				lua_pushstring(Instance->state, argCC);
			} break;
			case 10283618467285603348: {//string
				const string& arg = va_arg(ptr, string);
				lua_pushstring(Instance->state, arg.c_str());
			} break;
			}


		}

		va_end(ptr);

		if(!LuaOK(Instance->state, lua_pcall(Instance->state, argCount+1, 0, 0)))
			assert(false);
	}

	Scripting();
	~Scripting();
};

#pragma warning( pop )