#pragma comment(lib, "Ws2_32.lib")
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <istream>
#include <memory>
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

void ScreenShot(const std::wstring& name)
{
  HWND desktopHwnd = GetDesktopWindow();
  RECT desktopParams;
  HDC devC = GetDC(desktopHwnd);
  if (GetWindowRect(desktopHwnd, &desktopParams) == FALSE)
  {
    printf("Error: cannot get window rect");
  }
  DWORD width = desktopParams.right - desktopParams.left;
  DWORD height = desktopParams.bottom - desktopParams.top;
  HDC captureDC = CreateCompatibleDC(devC);
  HBITMAP captureBitmap = CreateCompatibleBitmap(devC, width, height);
  SelectObject(captureDC, captureBitmap);
  BitBlt(captureDC, 0, 0, width, height, devC, 0, 0, SRCCOPY | CAPTUREBLT);
  std::unique_ptr<Bitmap> p_bmp(Bitmap::FromHBITMAP(captureBitmap, NULL));
  CLSID encoderClsid;
  if (-1 == GetEncoderClsid(L"image/png", &encoderClsid))
    {
    printf("Error: cannot get png encoder");
    }
  if (p_bmp->Save(name.c_str(), &encoderClsid, NULL) != Gdiplus::Status::Ok)
  {
    printf("Error: gdiplus save png image status != Ok");
  }
}


int main()
{
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  if (GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) != Gdiplus::Status::Ok)
  {
    printf("Error: gdiplus startup status != Ok");
  }
  //FreeConsole();
  IniParser inip(L".\\config.ini");
  std::wstring serverip;
  if (!inip.GetValue(serverip, L"serverip"))
  {
    printf("Cannot read serverip from .ini file");
    return -1;
  }
  std::wstring winterval;
  if (!inip.GetValue(winterval, L"interval"))
  {
    printf("Cannot read interval from .ini file");
    return -1;
  }
  std::wstring wport;
  if (!inip.GetValue(wport, L"port"))
  {
    printf("Cannot read port from .ini file");
    return -1;
  }
  int port = _wtoi(wport.c_str());
  int interval = _wtoi(winterval.c_str());
  wchar_t tempPath[80];
  DWORD retval = GetTempPathW(80, tempPath);
  if (retval == 0)
  {
    printf("Error at temp path: length = 0");
    return -1;
  }
  std::wstring name = std::wstring(tempPath) + L"image.png";
  std::string ip(serverip.begin(), serverip.end());
  
  while (true)
  {
    ScreenShot(name);

    std::ifstream inputStream(name, std::ios::binary);
    if (inputStream.is_open())
    {
      std::string str((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());

      sf::TcpSocket socket;
      sf::Socket::Status st = socket.connect(ip, port);
      if (st != sf::Socket::Status::Done)
        {
        printf("Error: cannot connect to the server");
        continue;
        }
      sf::Packet packet;
      packet.append(&str[0], str.size());
      sf::Socket::Status status = socket.send(packet);
      while (status == sf::Socket::Status::Partial)
        status = socket.send(packet);

      if (status != sf::Socket::Status::Done)
      {
        printf("Error: cannot send packet to the server");
      }
    }
    else
    {
      printf("Error: cannot read the .img file");
    }
    Sleep(interval * 1000);
  }
  Gdiplus::GdiplusShutdown(gdiplusToken);
  return 0;
}