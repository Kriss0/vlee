/* Copyright (C) 2007-2008 Erik Faye-Lund and Egbert Teeselink
 * For conditions of distribution and use, see copyright notice in LICENSE.TXT
 */

#ifndef NETWORK_H
#define NETWORK_H

#ifdef WIN32

#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define SOCKET int
#define INVALID_SOCKET -1

#endif

bool initNetwork();
void closeNetwork();

// SOCKET clientConnect(SOCKET serverSocket);
SOCKET clientConnect(SOCKET serverSocket, sockaddr_in *host = NULL);
SOCKET serverConnect(struct sockaddr_in *addr);

bool pollRead(SOCKET socket);

#if 0
bool recvBlock(SOCKET socket, char *buffer, size_t length, int flags);
bool sendBlock(SOCKET socket, const char *buffer, size_t length, int flags);
#endif

enum RemoteCommand {
	// server -> client
	SET_KEY = 0,
	DELETE_KEY = 1,
	
	// client -> server
	GET_TRACK = 2,
	
	// client -> server, server -> client
	SET_ROW = 3,
	
	// server -> client
	PAUSE = 4,

	SAVE_TRACKS = 5
};

#endif /* NETWORK_H */
