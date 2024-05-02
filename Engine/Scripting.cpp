#include "Scripting.h"

#include "MathUtil.h"
#include "Engine.h"
#include "Scene.h"

Scripting* Scripting::Instance = nullptr;

bool LuaOK(lua_State* state, int id)
{
	if(id != LUA_OK) {
		string test = lua_tostring(state, -1);
		return false;
	}
	return true;
}

/// <summary>
/// Updates the Engine table information
/// </summary>
/// <param name="state">Lua state</param>
void Scripting::UpdateLuaEngineInformation(lua_State* state)
{
	lua_getglobal(state, "Engine");
	if(!lua_istable(state, -1)) //make sure it hasnt been redefined
		return;

	// push key : value parameters
	lua_pushstring(state, "elapsedTime");
	lua_pushnumber(state, Engine::Get()->GetTotalElapsedTime());
	lua_settable(state, -3); // set table information

	// push key:value parameters
	lua_pushstring(state, "currentScene");
	lua_pushstring(state, currentScene.c_str());
	lua_settable(state, -3); // set table information

	lua_pop(state, 1); // pop Engine table
}

// Called from the Engine, when the scene is changed
void Scripting::CallOnSceneLoad(const string& sceneName)
{
	Instance->currentScene = sceneName;
	Instance->CallProc<string>(Instance->state, "OnSceneLoad", 1, sceneName);
	Instance->UpdateLuaEngineInformation(Instance->state);
}

void Scripting::Update(float deltaTime)
{
	UpdateLuaEngineInformation(state);

	// Calls the Update function in Lua
	// needs to be passed as double because i think the variad converts it to a double as a default
	CallProc<double>(state, "Update", 1, deltaTime);

	if(Input::IsKeyPressed('G')) CallEvent("TestEvent", 0);
}

Scripting::Scripting() {
	if(Instance) return;
	Instance = this;

	// initialise state and open lua libraries
	state = luaL_newstate();
	luaL_openlibs(state);

	//register functions
	lua_register(state, "Engine_CreateObject3D", CreateObject3D);
	lua_register(state, "Engine_SetObject3DPos", SetObject3DPos);
	lua_register(state, "Engine_SetObject3DRot", SetObject3DRot);

	lua_register(state, "Engine_AddObject3DModel", AddObject3DModel);
	lua_register(state, "Engine_SetModelTexture", SetModelTexture);
	lua_register(state, "Engine_LoadMesh", LoadMesh);
	lua_register(state, "Engine_LoadTexture", LoadTexture);
	lua_register(state, "Engine_GetObject3D", GetObject3D);

	lua_register(state, "Engine_GetObjectForward", GetObjectForward);
	lua_register(state, "Engine_GetObjectLeft", GetObjectLeft);
	lua_register(state, "Engine_GetObjectUp", GetObjectUp);

	lua_register(state, "Engine_DebugMessage", DebugMessage);

	lua_register(state, "CallGameFunction", CallGameFunction);

	// initiate the file
	if(!LuaOK(state, luaL_dofile(state, "Scripts\\Main.lua")))
		assert(false);
}

Scripting::~Scripting() {
	lua_close(state);
}
