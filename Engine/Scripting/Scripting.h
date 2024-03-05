#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>


namespace Scripting {
	static MonoDomain* rootDomain;
	static MonoDomain* appDomain;

	extern void Init();
	extern void Close();
}