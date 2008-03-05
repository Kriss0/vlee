/* Copyright (C) 2007-2008 Erik Faye-Lund and Egbert Teeselink
 * For conditions of distribution and use, see copyright notice in LICENSE.TXT
 */

#include "stdafx.h"
#include "network.h"
#include <stdio.h>
#include <string.h>

#ifndef WIN32

#include "../wifi9.h"
#include <nds/jtypes.h>
#include <dswifi9.h>
#include <netdb.h>

#endif

bool initNetwork()
{

#ifdef WIN32
	WSADATA wsaData;
	if (0 != WSAStartup(MAKEWORD( 2, 0 ), &wsaData)) return false;
	if (LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 0) return false;
#endif

	// unix sockets need no init
	
	return true;
}

void closeNetwork()
{
#ifdef WIN32
	WSACleanup();
#endif
}

static const char *clientGreeting = "hello, synctracker!";
static const char *serverGreeting = "hello, demo!";

SOCKET clientConnect(SOCKET serverSocket, sockaddr_in *host)
{
	sockaddr_in hostTemp;
	int hostSize = sizeof(sockaddr_in);
	SOCKET clientSocket = accept(serverSocket, (sockaddr*)&hostTemp, &hostSize);
	if (INVALID_SOCKET == clientSocket) return INVALID_SOCKET;
	
	const char *expectedGreeting = clientGreeting;
	char recievedGreeting[128];
	
	recv(clientSocket, recievedGreeting, int(strlen(expectedGreeting)), 0);
	
	if (strncmp(expectedGreeting, recievedGreeting, strlen(expectedGreeting)) != 0)
	{
		closesocket(clientSocket);
		return INVALID_SOCKET;
	}
	
	const char *greeting = serverGreeting;
	send(clientSocket, greeting, int(strlen(greeting)), 0);
	
	if (NULL != host) *host = hostTemp;
	return clientSocket;
}

SOCKET serverConnect(struct sockaddr_in *addr)
{
	SOCKET serverSocket = socket( AF_INET, SOCK_STREAM, 0 );
	connect( serverSocket,(struct sockaddr *)addr, sizeof(struct sockaddr_in));
	
	const char *greeting = clientGreeting;
	send(serverSocket, greeting, int(strlen(greeting)), 0);
	
	const char *expectedGreeting = serverGreeting;
	char recievedGreeting[128];
	
	recv(serverSocket, recievedGreeting, int(strlen(expectedGreeting)), 0);
	if (strncmp(expectedGreeting, recievedGreeting, strlen(expectedGreeting)) != 0)
	{
		closesocket(serverSocket);
		return INVALID_SOCKET;
	}
	
	return serverSocket;
}

bool pollRead(SOCKET socket)
{
	struct timeval timeout = { 0, 0 };
	fd_set fds;
	
	FD_ZERO(&fds);
	FD_SET(socket, &fds);
	
	// look for new commands
	return select(0, &fds, NULL, NULL, &timeout) > 0;
}
#if 0
bool recvBlock(SOCKET socket, char *buffer, size_t length, int flags)
{
	size_t pos = 0;
	while (pos < length)
	{
		int ret = recv(socket, &buffer[pos], int(length - pos), flags);
		if (0 > ret) return false; // error
		pos += ret;
	}
	return true;
}

bool sendBlock(SOCKET socket, const char *buffer, size_t length, int flags)
{
	size_t pos = 0;
	while (pos < length)
	{
		int ret = send(socket, &buffer[pos], int(length - pos), flags);
		if (0 > ret) return false; // error
		pos += ret;
	}
	return true;
}
#endif
