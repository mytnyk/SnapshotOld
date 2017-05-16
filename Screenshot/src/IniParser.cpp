#include "IniParser.hpp"
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <tchar.h>

IniParser::IniParser(const std::wstring& path)
{
	_path = path;
}

void IniParser::LoadFile(const std::wstring& path)
{
	_path = path;
}

std::wstring IniParser::GetValue(const std::wstring& key)
{
	wchar_t protocolChar[80];
	GetPrivateProfileStringW(L"Config", key.c_str(), L"", protocolChar, 80, _path.c_str());
	return protocolChar;
}
