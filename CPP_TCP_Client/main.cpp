
#include <iostream>
#include "Source/tcp_client.h"

int main()
{
	std::cout << "Hello CMake." << std::endl;
	Essentials::Communications::TCP_Client tcp;
	std::cout << Essentials::Communications::TcpClientVersion;
	tcp.Configure("127.0.0.1", 5000);
	tcp.Connect();
	std::cout << tcp.GetLastError();
	return 0;
}
