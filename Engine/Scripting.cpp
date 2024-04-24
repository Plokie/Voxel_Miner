#include "Scripting.h"

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

int Scripting::GetInt(lua_State* state, const string& name)
{
	lua_getglobal(state, name.c_str());
	//if(!lua_isinteger(state, -1)) assert(false);
		
	return (int)lua_tointeger(state, -1);
}

Scripting::Scripting() {
	if(Instance) return;
	Instance = this;

	

	//state = luaL_newstate();
	//state = lua_newstate(l_alloc, nullptr);
}