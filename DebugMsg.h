#pragma once

#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

//class DebugMsg {
//public:

//
//	static void Log(string msg) {
//		wstring errmsg = L"Err: " + to_wstring(msg);
//		//MessageBoxW(0, errmsg, L"Error", 0);
//	}
//};

class DebugMsg {
public:
    static wstring to_wstring(string str) {
        wstring ws(str.begin(), str.end());
        return ws;
    }

	static void Output(const wchar_t* msg, ...) {
        wchar_t szBuff[1024];
        va_list arg;
        va_start(arg, msg);
        _vsnwprintf_s(szBuff, sizeof(szBuff), msg, arg);
        va_end(arg);

        OutputDebugString(szBuff);
	}

    static void Output(const string& msg, ...) {
        //Output();
        wchar_t szBuff[1024];
        va_list arg;
        va_start(arg, to_wstring(msg).c_str());
        _vsnwprintf_s(szBuff, sizeof(szBuff), to_wstring(msg).c_str(), arg);
        va_end(arg);

        OutputDebugString(szBuff);
    }
};