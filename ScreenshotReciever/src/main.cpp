#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <SFML\Network.hpp>


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
    listener.listen(2000);
    listener.accept(socket);

    Packet packet;
    socket.receive(packet);
    Save(packet);
    }
  return 0;
  }