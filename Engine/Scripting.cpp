#include "Scripting.h"

#include "MathUtil.h"
#include "Engine.h"
#include "Scene.h"

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


int Scripting::CreateObject3D(lua_State* state)
{
	int argCount = lua_gettop(state);
	
	string name = lua_tostring(state, 1);

	Object3D* newObj = new Object3D();
	Engine::Get()->GetCurrentScene()->CreateObject3D(newObj, name);

	return 0;
}

void Scripting::CallOnSceneLoad(const string& sceneName)
{
	Instance->CallProc<const char*>(Instance->state, "Root_OnSceneLoad", 1, sceneName.c_str());
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

	lua_register(state, "Engine_CreateObject3D", CreateObject3D);

	//if(ERRCHECK(state, luaL_dofile("Scripts\\Test.lua")))
	if(!LuaOK(state, luaL_dofile(state, "Scripts\\Engine.lua")))
		assert(false);

	//CallProc<const char*>(state, "Root_OnSceneLoad", 1, "game");
	
	Vector3 testVar = Vector3::FromLua(state, "testVar");
	

	__noop();
}


Scripting::~Scripting() {
	lua_close(state);
}
