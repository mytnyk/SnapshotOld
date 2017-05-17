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

bool IniParser::GetValue(std::wstring& value, const std::wstring& key)
{
	wchar_t protocolChar[80];
	DWORD retval = GetPrivateProfileStringW(L"Config", key.c_str(), L"", protocolChar, 80, _path.c_str());
  if (retval == 0)
    return false;
  value = protocolChar;
  return true;
}
