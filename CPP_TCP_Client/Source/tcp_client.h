///////////////////////////////////////////////////////////////////////////////
//!
//! @file		tcp_client.h
//! 
//! @brief		A cross platform class to handle tcp traffic to a server.
//! 
//! @author		Chip Brommer
//! 
//! @date		< 04 / 30 / 2023 > Initial Start Date
//!
/*****************************************************************************/
#pragma once
///////////////////////////////////////////////////////////////////////////////
//
//  Includes:
//          name                        reason included
//          --------------------        ---------------------------------------
#ifdef WIN32
#include <stdint.h>						// Standard integer types
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <map>							// Error enum to strings.
#include <string>						// Strings
#include <regex>						// Regular expression for ip validation
//
//	Defines:
//          name                        reason defined
//          --------------------        ---------------------------------------
#ifndef     CPP_TCP_CLIENT				// Define the cpp tcp client class. 
#define     CPP_TCP_CLIENT
//
///////////////////////////////////////////////////////////////////////////////

namespace Essentials
{
	namespace Communications
	{
		constexpr static uint8_t TCP_CLIENT_VERSION_MAJOR = 0;
		constexpr static uint8_t TCP_CLIENT_VERSION_MINOR = 1;
		constexpr static uint8_t TCP_CLIENT_VERSION_PATCH = 0;
		constexpr static uint8_t TCP_CLIENT_VERSION_BUILD = 0;

		static std::string TcpClientVersion = "TCP Client v" +
			std::to_string((uint8_t)TCP_CLIENT_VERSION_MAJOR) + "." +
			std::to_string((uint8_t)TCP_CLIENT_VERSION_MINOR) + "." +
			std::to_string((uint8_t)TCP_CLIENT_VERSION_PATCH) + " - b" +
			std::to_string((uint8_t)TCP_CLIENT_VERSION_BUILD) + ".\n";

		/// <summary>enum for error codes</summary>
		enum class TcpClientError : uint8_t
		{
			NONE,
			BAD_ADDRESS,
			ADDRESS_NOT_SET,
			BAD_PORT,
			PORT_NOT_SET,
			CLIENT_ALREADY_CONNECTED,
			FAILED_TO_CONNECT,
			WINSOCK_FAILURE,
			WINDOWS_SOCKET_OPEN_FAILURE,
			LINUX_SOCKET_OPEN_FAILURE,
			ADDRESS_NOT_SUPPORTED,
			CONNECTION_FAILED,
			SEND_FAILED,
			READ_FAILED,
		};

		/// <summary>Error enum to string map</summary>
		static std::map<TcpClientError, std::string> TcpClientErrorMap
		{
			{TcpClientError::NONE,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::NONE) + ": No error.")},
			{TcpClientError::BAD_ADDRESS,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::BAD_ADDRESS) + ": Bad address.")},
			{TcpClientError::ADDRESS_NOT_SET,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::ADDRESS_NOT_SET) + ": Address not set.")},
			{TcpClientError::BAD_PORT,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::BAD_PORT) + ": Bad port.")},
			{TcpClientError::PORT_NOT_SET,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::PORT_NOT_SET) + ": Port not set.")},
			{TcpClientError::FAILED_TO_CONNECT,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::FAILED_TO_CONNECT) + ": Failed to connect.")},
			{TcpClientError::WINSOCK_FAILURE,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::WINSOCK_FAILURE) + ": Winsock creation failure.")},
			{TcpClientError::WINDOWS_SOCKET_OPEN_FAILURE,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::WINDOWS_SOCKET_OPEN_FAILURE) + ": Socket open failure.")},
			{TcpClientError::LINUX_SOCKET_OPEN_FAILURE,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::LINUX_SOCKET_OPEN_FAILURE) + ": Socket open failure.")},
			{TcpClientError::ADDRESS_NOT_SUPPORTED,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::ADDRESS_NOT_SUPPORTED) + ": Address not supported.")},
			{TcpClientError::CONNECTION_FAILED,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::CONNECTION_FAILED) + ": Connection failed.")},
			{TcpClientError::SEND_FAILED,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::SEND_FAILED) + ": Send failed.")},
			{TcpClientError::READ_FAILED,
			std::string("Error Code " + std::to_string((uint8_t)TcpClientError::READ_FAILED) + ": Read failed.")},
		};

		/// <summary>A class to handle multi-platform connections to a TCP server.</summary>
		class TCP_Client
		{
		public:
			/// <summary>Default Constructor</summary>
			TCP_Client();

			/// <summary>Constructor to receive an address and port</summary>
			TCP_Client(const std::string address, const int16_t port);

			/// <summary>Default Deconstructor</summary>
			~TCP_Client();

			/// <summary>Configure the client</summary>
			/// <param name="address"> -[in]- Address of the server</param>
			/// <param name="port"> -[in]- Port of the server</param>
			/// <returns>0 if successful, -1 if fails. Call Serial::GetLastError to find out more.</returns>
			int8_t Configure(const std::string address, const int16_t port);

			/// <summary>Connect to TCP server at the set address and port</summary>
			/// <returns>0 if successful, -1 if fails. Call Serial::GetLastError to find out more.</returns>
			int8_t Connect();

			/// <summary>Send data to a server</summary>
			/// <param name="buffer"> -[in]- Buffer to be sent</param>
			/// <returns>0+ if successful (number bytes sent), -1 if fails. Call TCP_Client::GetLastError to find out more.</returns>
			int8_t Send(const char* buffer);

			/// <summary>Receive data from a server</summary>
			/// <param name="buffer"> -[out]- Buffer to place received data into</param>
			/// <returns>0+ if successful (number bytes received), -1 if fails. Call TCP_Client::GetLastError to find out more.</returns>
			int8_t Receive(void* buffer);

			/// <summary>Closes a socket</summary>
			void Close();

			/// <summary>Get the last error in string format</summary>
			/// <returns>The last error in a formatted string</returns>
			std::string GetLastError();

		protected:
		private:
			/// <summary>Validates an IP address is IPv4 or IPv6</summary>
			/// <param name="ip"> -[in]- IP Address to be validated</param>
			/// <returns></returns>
			int8_t ValidateIP(const std::string& ip);

			/// <summary>Validates a port number is between 0-99999</summary>
			/// <param name="port"> -[in]- Port number to be validated</param>
			/// <returns>true = valid, false = invalid</returns>
			bool ValidatePort(const int16_t port);

			// Variables
			std::string			mAddress;
			int16_t				mPort;
			std::string			mTitle;
			bool				mConnected;			
			TcpClientError		mLastError;			// Last error for this utility

#ifdef WIN32
			WSADATA				mWsaData;
			SOCKET				mSocket;
#else
			int32_t				mSocket;
#endif
		};
	}
}

#endif		// CPP_TCP_CLIENT