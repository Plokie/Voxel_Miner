#include "MathUtil.h"

#include "Scripting.h"

Vector3 Vector3::FromLua(lua_State* state, const string& name)
{
	Vector3 ret;

	lua_getglobal(state, name.c_str());
	if(!lua_istable(state, -1))
		assert(false);

	lua_pushstring(state, "x");
	lua_gettable(state, -2);
	ret.x = static_cast<float>(lua_tonumber(state, -1));
	lua_pop(state, 1);

	lua_pushstring(state, "y");
	lua_gettable(state, -2);
	ret.y = static_cast<float>(lua_tonumber(state, -1));
	lua_pop(state, 1);

	lua_pushstring(state, "z");
	lua_gettable(state, -2);
	ret.z = static_cast<float>(lua_tonumber(state, -1));
	lua_pop(state, 1);

	return ret;
}
