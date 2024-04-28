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


int Scripting::CreateObject3D(lua_State* state)
{
	int argCount = lua_gettop(state);
	
	string name = lua_tostring(state, 1);

	Object3D* newObj = new Object3D();
	Engine::Get()->GetCurrentScene()->CreateObject3D(newObj, name);

	return 0;
}

int Scripting::SetObject3DPos(lua_State* state)
{
	int argCount = lua_gettop(state);

	string name = lua_tostring(state, 1);
	float x = (float)lua_tonumber(state, 2);
	float y = (float)lua_tonumber(state, 3);
	float z = (float)lua_tonumber(state, 4);
	//Vector3 pos = Vector3::FromLua()

	Object3D* obj = Engine::Get()->GetCurrentScene()->GetObject3D(name);
	if(obj) {
		obj->transform.position = { x,y,z };
	}
	else return 0;

	return 1;
}

int Scripting::SetObject3DRot(lua_State* state)
{
	string name = lua_tostring(state, 1);
	float x = (float)lua_tonumber(state, 2);
	float y = (float)lua_tonumber(state, 3);
	float z = (float)lua_tonumber(state, 4);

	Object3D* obj = Engine::Get()->GetCurrentScene()->GetObject3D(name);
	if(obj) {
		obj->transform.rotation = { x, y, z };
	}
	else return 0;

	return 1;
}

int Scripting::AddObject3DModel(lua_State* state)
{
	string name = lua_tostring(state, 1);
	string modelName = lua_tostring(state, 2);

	Model* model = Engine::Get()->GetCurrentScene()->GetObject3D(name)->AddModel(Graphics::Get()->GetDevice());
	model->SetMesh(modelName);

	return 0;
}

void Scripting::CallOnSceneLoad(const string& sceneName)
{
	Instance->CallProc<string>(Instance->state, "OnSceneLoad", 1, sceneName);
}

void Scripting::Update(float deltaTime)
{
	/*CallProc<float>(state, "Update", 1, deltaTime);*/
	//if(Input::IsKeyHeld('V')) 
		CallProc<double>(state, "Update", 1, deltaTime);
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

	//register functions
	lua_register(state, "Engine_CreateObject3D", CreateObject3D);
	lua_register(state, "Engine_SetObject3DPos", SetObject3DPos);
	lua_register(state, "Engine_SetObject3DRot", SetObject3DRot);

	lua_register(state, "Engine_AddObject3DModel", AddObject3DModel);

	if(!LuaOK(state, luaL_dofile(state, "Scripts\\Engine.lua")))
		assert(false);
}


Scripting::~Scripting() {
	lua_close(state);
}
