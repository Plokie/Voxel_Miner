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

int Scripting::CreateObject3D(lua_State* state)
{
	// Get object name parameter
	string name = lua_tostring(state, 1);

	//Creates the object in the currently active scene
	Object3D* newObj = new Object3D();
	Engine::Get()->GetCurrentScene()->CreateObject3D(newObj, name);

	return 0;
}

int Scripting::SetObject3DPos(lua_State* state)
{
	// Get lua parameters
	string name = lua_tostring(state, 1);
	float x = (float)lua_tonumber(state, 2);
	float y = (float)lua_tonumber(state, 3);
	float z = (float)lua_tonumber(state, 4);

	Object3D* obj = Engine::Get()->GetCurrentScene()->GetObject3D(name);
	if(obj) {
		obj->transform.position = { x,y,z };
	}
	else return 0;

	return 0;
}

int Scripting::SetObject3DRot(lua_State* state)
{
	// Gets lua parameters
	string name = lua_tostring(state, 1);
	float x = (float)lua_tonumber(state, 2);
	float y = (float)lua_tonumber(state, 3);
	float z = (float)lua_tonumber(state, 4);

	Object3D* obj = Engine::Get()->GetCurrentScene()->GetObject3D(name);
	if(obj) {
		obj->transform.rotation = { x, y, z };
	}

	return 0;
}

int Scripting::AddObject3DModel(lua_State* state)
{
	string name = lua_tostring(state, 1);
	string modelName = lua_tostring(state, 2);

	Object3D* obj = Engine::Get()->GetCurrentScene()->GetObject3D(name);
	if(obj) {
		Model* model = obj->AddModel(Graphics::Get()->GetDevice());
		model->SetMesh(modelName);
	}
	//Engine::Get()->GetCurrentScene()->GetObject3D(name)->AddModel(Graphics::Get()->GetDevice());
	//model->SetMesh(modelName);

	return 0;
}

// object name, idx, texture name
int Scripting::SetModelTexture(lua_State* state)
{
	string name = lua_tostring(state, 1);
	int idx = (int)lua_tonumber(state, 2);
	string texName = lua_tostring(state, 3);

	Object3D* obj = Engine::Get()->GetCurrentScene()->GetObject3D(name);
	ID3D11ShaderResourceView* tex = Resources::GetTexture(texName);

	if(obj && tex && obj->models.size() > idx) {
		obj->models[idx]->SetTexture(0, tex);
	}
	else return 0;

	return 1;
}

// indices table, vertices table, name
int Scripting::LoadMesh(lua_State* state)
{
	vector<DWORD> indices;
	vector<Vertex> vertices;

	// handle first parameter : indices table
	if(lua_istable(state, 1)) {
		// get length of indices table
		unsigned int size = (unsigned int)lua_rawlen(state, 1);
		indices.resize(size);

		// get each element and add to indices vector as DWORD
		for(unsigned int i = 0; i < size; i++) {
			lua_rawgeti(state, 1, i + 1);
			indices[i] = (DWORD)lua_tonumber(state, -1);
			lua_pop(state, 1);
		}
	}

	// handle second parameter : vertex table
	if(lua_istable(state, 2)) {
		// get length of vertex table
		unsigned int size = (unsigned int)lua_rawlen(state, 2);
		vertices.resize(size);

		// for each vertex element
		for(unsigned int i = 0; i < size; i++) {
			lua_rawgeti(state, 2, i + 1); // get vertex element in array
			int top = lua_gettop(state);

			Vertex v;
			// convert from table to vertex
			if(lua_istable(state, top)) {
				Vector3 pos;
				Vector2 uv;
				Vector3 norm;

				// position

				lua_rawgeti(state, -1, 1);
				pos.x = (float)lua_tonumber(state, -1);
				lua_pop(state, 1);

				lua_rawgeti(state, -1, 2);
				pos.y = (float)lua_tonumber(state, -1);
				lua_pop(state, 1);

				lua_rawgeti(state, -1, 3);
				pos.z = (float)lua_tonumber(state, -1);
				lua_pop(state, 1);

				// uv

				lua_rawgeti(state, -1, 4);
				uv.x = (float)lua_tonumber(state, -1);
				lua_pop(state, 1);

				lua_rawgeti(state, -1, 5);
				uv.y = (float)lua_tonumber(state, -1);
				lua_pop(state, 1);

				// normal

				lua_rawgeti(state, -1, 6);
				norm.x = (float)lua_tonumber(state, -1);
				lua_pop(state, 1);

				lua_rawgeti(state, -1, 7);
				norm.y = (float)lua_tonumber(state, -1);
				lua_pop(state, 1);

				lua_rawgeti(state, -1, 8);
				norm.z = (float)lua_tonumber(state, -1);
				lua_pop(state, 1);

				v = Vertex(pos.x, pos.y, pos.z, norm.x, norm.y, norm.z, uv.x, uv.y);
			}

			// add to vector
			vertices[i] = v;

			lua_pop(state, 1);
		}
	}

	// get name parameter
	string meshName = lua_tostring(state, 3);

	// add mesh to resources
	Resources::LoadMesh(&vertices[0], (int)vertices.size(), &indices[0], (int)indices.size(), meshName);

	return 0;
}

//path, name
int Scripting::LoadTexture(lua_State* state)
{
	// get parameters
	string path = lua_tostring(state, 1);
	string name = lua_tostring(state, 2);

	wstring wpath(path.begin(), path.end()); //todo: this is bad, and will mangle non-ASCII path / file names

	// add texture to engine resources
	Resources::LoadTexture(wpath.c_str(), name);
	return 1;
}

int Scripting::GetObject3D(lua_State* state)
{
	// get name parameter
	string name = lua_tostring(state, 1);

	// find objects
	Object3D* obj = Engine::Get()->GetCurrentScene()->GetObject3D(name);
	if(obj) { // push information
		lua_pushboolean(state, true); // first boolean represents a success or fail in finding the object
		lua_pushnumber(state, obj->transform.position.x);
		lua_pushnumber(state, obj->transform.position.y);
		lua_pushnumber(state, obj->transform.position.z);
		lua_pushnumber(state, obj->transform.rotation.x);
		lua_pushnumber(state, obj->transform.rotation.y);
		lua_pushnumber(state, obj->transform.rotation.z);
	}
	else { //push blank data
		lua_pushboolean(state, false);
		lua_pushnumber(state, 0);
		lua_pushnumber(state, 0);
		lua_pushnumber(state, 0);
		lua_pushnumber(state, 0);
		lua_pushnumber(state, 0);
		lua_pushnumber(state, 0);
	}

	return 7;
}

/// <summary>
/// Purely just triggers a breakpoint to make sure code is executing in the right order from lua
/// </summary>
/// <param name="state"></param>
/// <returns></returns>
int Scripting::DebugMessage(lua_State* state)
{
	string message = lua_tostring(state, 1);
	return 0;
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

	lua_register(state, "Engine_DebugMessage", DebugMessage);

	lua_register(state, "CallGameFunction", CallGameFunction);

	// initiate the file
	if(!LuaOK(state, luaL_dofile(state, "Scripts\\Main.lua")))
		assert(false);
}

Scripting::~Scripting() {
	lua_close(state);
}
