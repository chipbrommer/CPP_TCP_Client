// CPP_TCP_Client.cpp : Defines the entry point for the application.
//
#include <iostream>
#include "Source/tcp_client.h"

int main()
{
	std::cout << "Hello CMake." << std::endl;
	Essentials::Communications::TCP_Client tcp;
	tcp.Configure("127.0.0.1", 5000);
	tcp.Connect();
	return 0;
}
