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

#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;
using namespace sf;

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
  GetWindowRect(desktopHwnd, &desktopParams);
  DWORD width = desktopParams.right - desktopParams.left;
  DWORD height = desktopParams.bottom - desktopParams.top;
  /*
  DWORD FileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (sizeof(RGBTRIPLE) + 1 * (Width * Height * 4));
  char *BmpFileData = (char*)GlobalAlloc(0x0040, FileSize);
  PBITMAPFILEHEADER BFileHeader = (PBITMAPFILEHEADER)BmpFileData;
  PBITMAPINFOHEADER  BInfoHeader = (PBITMAPINFOHEADER)&BmpFileData[sizeof(BITMAPFILEHEADER)];
  BFileHeader->bfType = 0x4D42; // BM
  BFileHeader->bfSize = sizeof(BITMAPFILEHEADER);
  BFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  BInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
  BInfoHeader->biPlanes = 1;
  BInfoHeader->biBitCount = 24;
  BInfoHeader->biCompression = BI_RGB;
  BInfoHeader->biHeight = Height;
  BInfoHeader->biWidth = Width;
  RGBTRIPLE bmpFileData[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];*/
  HDC captureDC = CreateCompatibleDC(devC);
  HBITMAP captureBitmap = CreateCompatibleBitmap(devC, width, height);
  SelectObject(captureDC, captureBitmap);
  BitBlt(captureDC, 0, 0, width, height, devC, 0, 0, SRCCOPY | CAPTUREBLT);/*
  GetDIBits(CaptureDC, CaptureBitmap, 0, Height, bmpFileData, (LPBITMAPINFO)BInfoHeader, DIB_RGB_COLORS);
  DWORD Junk;
  HANDLE FH = CreateFileA(BmpName.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0);
  WriteFile(FH, bmpFileData, FileSize, &Junk, 0);
  CloseHandle(FH);
  GlobalFree(bmpFileData);*/

  Bitmap *p_bmp = Bitmap::FromHBITMAP(captureBitmap, NULL);

  CLSID encoderClsid;
  //Gdiplus::Image* image = new Gdiplus::Image(L"Image.bmp");
  GetEncoderClsid(L"image/png", &encoderClsid);
  p_bmp->Save(name.c_str(), &encoderClsid, NULL);
  delete p_bmp;

  }
/*
sf::Packet& operator << (sf::Packet& packet, sf::Image& texture)
{
  size_t sizeX, sizeY;
  sizeX = texture.getSize().x;
  sizeY = texture.getSize().y;
  packet << sizeX << sizeY;
  for (size_t i = 0; i < sizeX; ++i)
  {
    for (size_t j = 0; j < sizeY; j++)
    {
      packet << texture.getPixel(i, j).toInteger();
    }
  }
  return packet;
}*/

int main()
  {
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

  FreeConsole();
  std::wstring name = L"Image.png";

  while (true)
    {
    ScreenShot(name);
    TcpSocket socket;
    if (socket.connect("127.0.0.1", 2000) == Socket::Status::Done)
      {
      Packet packet;
      //sf::Image im;
      //im.loadFromFile(name);
      std::ifstream inputStream(name, std::ios::binary);
      std::string str((std::istreambuf_iterator<char>(inputStream)),
        std::istreambuf_iterator<char>());

      packet.append(&str[0], str.size());
      //packet << im;
      socket.send(packet);
      }
    Sleep(1000 * 2);
    }

  GdiplusShutdown(gdiplusToken);
  }