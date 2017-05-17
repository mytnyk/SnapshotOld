#pragma once
#include <string>
#include <tchar.h>

class IniParser
{
public:
  IniParser(const std::wstring& path);
  bool GetValue(std::wstring& value, const std::wstring& key);
private:
  std::wstring _path;
};

