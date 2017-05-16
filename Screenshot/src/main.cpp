
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <gdiplus.h>
#include <tchar.h>
#include "IniParser.hpp"
#include <direct.h>


#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;
	UINT  size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure
	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}
	free(pImageCodecInfo);
	return -1;  // Failure
}
void ScreenShot(const std::wstring& name, std::wstring directory)
{
	HWND desktopHwnd = GetDesktopWindow();
	RECT desktopParams;
	HDC devC = GetDC(desktopHwnd);
	GetWindowRect(desktopHwnd, &desktopParams);
	DWORD width = desktopParams.right - desktopParams.left;
	DWORD height = desktopParams.bottom - desktopParams.top;
	HDC captureDC = CreateCompatibleDC(devC);
	HBITMAP captureBitmap = CreateCompatibleBitmap(devC, width, height);
	SelectObject(captureDC, captureBitmap);
	BitBlt(captureDC, 0, 0, width, height, devC, 0, 0, SRCCOPY | CAPTUREBLT);

	Bitmap *p_bmp = Bitmap::FromHBITMAP(captureBitmap, NULL);

	CLSID encoderClsid;
	//Gdiplus::Image* image = new Gdiplus::Image(L"Image.bmp");
	GetEncoderClsid(L"image/png", &encoderClsid);
	directory += L"\\";
	directory += name.c_str();
	p_bmp->Save(directory.c_str(), &encoderClsid, NULL);
	delete p_bmp;

}


int main()
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	FreeConsole();
	std::wstring name = L"Image.png";

	IniParser inip(L"config.ini");
	std::wstring serverip = inip.GetValue(L"serverip");
	std::wstring winterval = inip.GetValue(L"interval");
  int interval = _wtoi(winterval.c_str());
	std::wstring imgsavedir = inip.GetValue(L"imgsavedir");
	while (true)
	{
		ScreenShot(name, imgsavedir);
		sf::TcpSocket socket;
		if (socket.connect(sf::IpAddress(std::string(serverip.begin(), serverip.end())), 2000) == sf::Socket::Status::Done)
		{
      sf::Packet packet;
			std::ifstream inputStream(name, std::ios::binary);
			std::string str((std::istreambuf_iterator<char>(inputStream)),
				std::istreambuf_iterator<char>());
			packet.append(&str[0], str.size());
			socket.send(packet);
		}

		Sleep(interval * 1000);
	}

	GdiplusShutdown(gdiplusToken);
}