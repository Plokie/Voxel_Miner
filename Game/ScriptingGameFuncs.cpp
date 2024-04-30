#include "../Engine/Scripting.h"

//map<string, function<int(lua_State*)>> Scripting::funcs = {};

void Scripting::RegisterLuaFunc(const string& name, function<int(lua_State*)> func)
{
	Instance->funcs[name] = func;
}

int Scripting::CallGameFunction(lua_State* state) {
	//int argCount = lua_gettop(state);
	string funcName = lua_tostring(state, 1);

	auto findFunc = Instance->funcs.find(funcName);
	if(findFunc != Instance->funcs.end()) {
		return findFunc->second(state);
	}
	return 0;
}