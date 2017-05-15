#pragma comment(lib, "sfml-network-d.lib")
#pragma comment(lib, "sfml-graphics-d.lib")
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>

using namespace std;
using namespace sf;

Image& operator >> (Packet& packet, Image texture)
{
	size_t sizeX, sizeY;
	packet >> sizeX >> sizeY;
	texture.create(sizeX, sizeY, Color::Blue);
	for (size_t i = 0; i < sizeX; i++)
	{
		for (size_t j = 0; j < sizeY; j++)
		{
			Uint32 c;
			packet >> c;
			texture.setPixel(i, j, Color(c));
		}
	}
	time_t t = time(0);
	struct tm * now = localtime(&t);
	string name = "Image " + to_string(now->tm_mday) + "_" + to_string(now->tm_hour) + "_" + to_string(now->tm_min) + "_" + to_string(now->tm_sec) + ".png"; 
	texture.saveToFile(name);
	std::cout << "Saved: " << name << endl;
	return texture;
}
int main()
{
	while (true)
	{
		TcpSocket socket;
		TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);
		Image tx;
		Packet packet;
		socket.receive(packet);
		packet >> tx;
	}
	return 0;
}