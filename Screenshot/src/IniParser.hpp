#pragma once
#include <string>
#include <tchar.h>

using namespace std;
class IniParser
{
public:
	IniParser(string path);
	void LoadFile(string path);
	basic_string<TCHAR> GetValue(wstring key);
private:
	string path;
};

