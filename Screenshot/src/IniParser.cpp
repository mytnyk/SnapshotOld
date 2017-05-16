#include "IniParser.hpp"
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <tchar.h>

IniParser::IniParser(string path)
{
	this->path = path;
}

void IniParser::LoadFile(string path)
{
	this->path = path;
}

basic_string<TCHAR> IniParser::GetValue(wstring key)
{
	LPCTSTR path = L"C:\\ProgramData\\Screenshot\\config.ini";
	TCHAR protocolChar[80];
	GetPrivateProfileString(_T("Config"), key.c_str(), _T(""), protocolChar, 80, path);
	std::basic_string<TCHAR> value(protocolChar);
	return value;
}
