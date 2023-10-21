#pragma once

#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

class DebugMsg {
public:
	static wstring to_wstring(string str) {
		wstring ws(str.begin(), str.end());
		return ws;
	}

	static void Log(string msg) {
		wstring errmsg = L"Err: " + to_wstring(msg);
		//MessageBoxW(0, errmsg, L"Error", 0);
	}
};