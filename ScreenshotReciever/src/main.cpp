#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <SFML\Network.hpp>
#include <stdio.h>

using namespace sf;

void Save(Packet& packet)
  {
  size_t size = packet.getDataSize();// >> tx;
  std::vector<char> data(size);

  time_t t = time(0);
  struct tm * now = localtime(&t);
  std::string name = "Image " + std::to_string(now->tm_mday) + "_" + std::to_string(now->tm_hour) + "_" + std::to_string(now->tm_min) + "_" + std::to_string(now->tm_sec) + ".png";

  std::cout << "Saved: " << name << std::endl;

  std::ofstream outputStream(name, std::ios::binary);
  outputStream.write((const char*)packet.getData(), size);
  }


int main()
  {
  while (true)
    {
    TcpSocket socket;
    TcpListener listener;
    sf::Socket::Status status = listener.listen(2001);
    if (status != sf::Socket::Status::Done)
      {
      printf("Error: cannot listen at 2001 port");
      }

    status = listener.accept(socket);
    if (status != sf::Socket::Status::Done)
      {
      printf("Error: cannot accept socket");
      }

    Packet packet;
    if (socket.receive(packet) == sf::Socket::Status::Done)
      {
      Save(packet);
      }
    else
      {
      printf("Error: cannot recieve packet");
      }
    }
  return 0;
  }