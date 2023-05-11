
#include <iostream>
#include "Source/tcp_client.h"

int main()
{
	std::cout << "Hello CMake." << std::endl;
	Essentials::Communications::TCP_Client tcp;
	std::cout << Essentials::Communications::TcpClientVersion;
	tcp.Configure("127.0.0.1", 8080);
	tcp.Connect();
	std::cout << tcp.GetLastError() << std::endl;

	std::string x = "Hello Server from Windows Client";
	tcp.Send(x.c_str(), x.length());

	char buffer[60] = {0};

	int i = sizeof(buffer);

	int i = tcp.Receive(&buffer, sizeof(buffer));

	if (i > 0)
	{
		std::cout << buffer << std::endl;
	}

	return 0;
}
