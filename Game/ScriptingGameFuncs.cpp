#include "../Engine/Scripting.h"


void Scripting::RegisterLuaFunc(const string& name, function<int(lua_State*)> func)
{
	Instance->funcs[name] = func;
}

int Scripting::CallGameFunction(lua_State* state) {
	// get the function name parameter passed
	string funcName = lua_tostring(state, 1);

	// look for the function in the map register
	auto findFunc = Instance->funcs.find(funcName);
	if(findFunc != Instance->funcs.end()) {
		return findFunc->second(state); // call the function if its found, passing the state too
	}
	return 0;
}