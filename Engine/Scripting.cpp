#include "Scripting.h"

#include "MathUtil.h"

Scripting* Scripting::Instance = nullptr;

static void* l_alloc(void* ud, void* ptr, size_t osize,
    size_t nsize) {
    (void)ud;  (void)osize;  /* not used */
    if(nsize == 0) {
        free(ptr);
        return NULL;
    }
    else
        return realloc(ptr, nsize);
}


bool LuaOK(lua_State* state, int id)
{
	if(id != LUA_OK) {
		string test = lua_tostring(state, -1);
		return false;
	}
	return true;
}


int Scripting::GetInt(lua_State* state, const string& name)
{
	lua_getglobal(state, name.c_str());
	if(!lua_isinteger(state, -1)) assert(false);
		
	return (int)lua_tointeger(state, -1);
}

Scripting::Scripting() {
	if(Instance) return;
	Instance = this;

	

	state = luaL_newstate();
	luaL_openlibs(state);

	//if(ERRCHECK(state, luaL_dofile("Scripts\\Test.lua")))
	if(!LuaOK(state, luaL_dofile(state, "Scripts\\Test.lua")))
		assert(false);

	//int test_var = GetInt(state, "testVar");
	
	Vector3 testVar = Vector3::FromLua(state, "testVar");
	

	__noop();
	//state = lua_newstate(l_alloc, nullptr);
}


Scripting::~Scripting() {
	lua_close(state);
}
