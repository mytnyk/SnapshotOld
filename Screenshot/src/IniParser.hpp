#pragma once
#include <string>
#include <tchar.h>


class IniParser
{
public:
	IniParser(const std::wstring& path);
	void LoadFile(const std::wstring& path);
  std::wstring GetValue(const std::wstring& key);
private:
  std::wstring _path;
};

