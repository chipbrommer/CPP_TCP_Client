///////////////////////////////////////////////////////////////////////////////
//!
//! @file		tcp_client.cpp
//! 
//! @brief		Implementation of the tcp client class
//! 
//! @author		Chip Brommer
//! 
//! @date		< 04 / 30 / 2023 > Initial Start Date
//!
/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
//
//  Includes:
//          name                        reason included
//          --------------------        ---------------------------------------
#include	"tcp_client.h"				// TCP Client Class
#include <iostream>
//
///////////////////////////////////////////////////////////////////////////////

namespace Essentials
{
	namespace Communications
	{
		TCP_Client::TCP_Client()
		{
			mAddress = "\n";
			mPort = -1;
			mTitle = "TCP Client";
			mConnected = false;
			mLastError = TcpClientError::NONE;

#ifdef WIN32
			mWsaData = {};
			mSocket = INVALID_SOCKET;
#elif __linux__
			mSocket = -1;
#endif
		}

		TCP_Client::TCP_Client(const std::string address, const int16_t port)
		{
			if (ValidateIP(address) >= 0)
			{
				mAddress = address;
			}
			else
			{
				mLastError = TcpClientError::BAD_ADDRESS;
			}

			if (ValidatePort(port) == true)
			{
				mPort = port;
			}
			else
			{
				mLastError = TcpClientError::BAD_PORT;
			}
		}

		TCP_Client::~TCP_Client()
		{
			Close();
		}

		int8_t TCP_Client::Configure(const std::string address, const int16_t port)
		{
			if (ValidateIP(address) >= 0)
			{
				mAddress = address;
			}
			else
			{
				mLastError = TcpClientError::BAD_ADDRESS;
				return -1;
			}

			if (ValidatePort(port) == true)
			{
				mPort = port;
			}
			else
			{
				mLastError = TcpClientError::BAD_PORT;
				return -1;
			}

			return 0;
		}

		int8_t TCP_Client::Connect()
		{
			if (mAddress == "\n" || mAddress.empty())
			{
				mLastError = TcpClientError::ADDRESS_NOT_SET;
				return -1;
			}

			if (mPort == -1)
			{
				mLastError = TcpClientError::PORT_NOT_SET;
				return -1;
			}

			if (mConnected == true)
			{
				mLastError = TcpClientError::CLIENT_ALREADY_CONNECTED;
				return -1;
			}

#ifdef WIN32
			if (WSAStartup(MAKEWORD(2, 2), &mWsaData) != 0) 
			{
				mLastError = TcpClientError::WINSOCK_FAILURE;
				return -1;
			}

			mSocket = socket(AF_INET, SOCK_STREAM, 0);

			if (mSocket == INVALID_SOCKET)
			{
				mLastError = TcpClientError::WINDOWS_SOCKET_OPEN_FAILURE;
				WSACleanup();
				return 1;
			}
#elif __linux__
			mSocket = socket(AF_INET, SOCK_STREAM, 0);

			if (mSocket == -1)
			{
				mLastError = TcpClientError::LINUX_SOCKET_OPEN_FAILURE;
				return 1;
			}
#endif
			// Set up server details
			sockaddr_in serverAddress{};
			serverAddress.sin_family = AF_INET;
			serverAddress.sin_port = htons(mPort);
			if (inet_pton(AF_INET, mAddress.c_str(), &(serverAddress.sin_addr)) <= 0)
			{
				mLastError = TcpClientError::ADDRESS_NOT_SUPPORTED;
				return -1;
			}

			// Connect to the server
			if (connect(mSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0)
			{
				mLastError = TcpClientError::CONNECTION_FAILED;
				Close();
				return -1;
			}

			return 0;
		}

		int8_t TCP_Client::Send(const char* buffer)
		{
			int sizeSent = send(mSocket, buffer, sizeof(buffer), 0);
			if (sizeSent < 0)
			{
				mLastError = TcpClientError::SEND_FAILED;
				return -1;
			}

			return sizeSent;
		}

		int8_t TCP_Client::Receive(void* buffer)
		{
			// Temp buffer to receive into
			char buf[sizeof(buffer)];

			int sizeRead = recv(mSocket, buf, sizeof(buf), 0);
			if (sizeRead < 0)
			{
				mLastError = TcpClientError::READ_FAILED;
				return -1;
			}

			// Copy temp buffer into param buffer and return size read. 
			buffer = buf;
			return sizeRead;
		}

		void TCP_Client::Close()
		{
#ifdef WIN32
			closesocket(mSocket);
			WSACleanup();
			mSocket = INVALID_SOCKET;
#else
			close(mSocket);
			mSocket = -1;
#endif
		}

		int8_t TCP_Client::ValidateIP(const std::string& ip)
		{
			// Regex expression for validating IPv4
			std::regex ipv4("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");

			// Regex expression for validating IPv6
			std::regex ipv6("((([0-9a-fA-F]){1,4})\\:){7}([0-9a-fA-F]){1,4}");

			// Checking if it is a valid IPv4 addresses
			if (std::regex_match(ip, ipv4))
			{
				return 1;
			}
			// Checking if it is a valid IPv6 addresses
			else if (std::regex_match(ip, ipv6))
			{
				return 2;
			}

			// Return Invalid
			return -1;
		}

		bool TCP_Client::ValidatePort(const int16_t port)
		{
			return (port > -1 && port < 99999);
		}
	
		std::string TCP_Client::GetLastError()
		{
			return TcpClientErrorMap[mLastError];
		}
	}
}