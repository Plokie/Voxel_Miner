#include "Scripting.h"

void Scripting::Init() {
	mono_set_assemblies_path("./");

	MonoDomain* _rootDomain = mono_jit_init("MyScriptRuntime");
	if (!_rootDomain) {
		exit(304);
	}

	rootDomain = _rootDomain;

	appDomain = mono_domain_create_appdomain((char*)"MyAppDomain", nullptr);
	mono_domain_set(appDomain, true);
}

void Scripting::Close() {
	if(rootDomain) {
		delete rootDomain;
	}
}