#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "conio.h"
#include "ThreadFunctions.h"
#include "Structures.h"
#include "QueueFunctions.h"
#include "Util.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma pack(1)

#pragma warning(disable:4996) 

#define BUFFER_SIZE 512
#define SERVER_PORT_PUBLISHER "27016"
#define SERVER_PORT_SUBSCRIBER "27020"

int  main(void)
{
	// Socket used for listening for new clients 
	SOCKET listenSocketPublisher = INVALID_SOCKET;
	SOCKET listenSocketSubscriber = INVALID_SOCKET;

	int iResult;
	WSADATA wsaData;
 
	DataQueue* dataQueue = InitQueue();

	HashSetKeyValuePair hashset[3];
	createHashSet(hashset);


	// Initialize windows sockets library for this process
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed with error: %d\n", WSAGetLastError());
		return 1;
	}

	addrinfo *resultingAddress = NULL;
	addrinfo hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;       // IPv4 address
	hints.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
	hints.ai_protocol = IPPROTO_TCP; // Use TCP protocol
	hints.ai_flags = AI_PASSIVE;     // 

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, SERVER_PORT_PUBLISHER, &hints, &resultingAddress);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	listenSocketPublisher = socket(AF_INET,      // IPv4 address family
		SOCK_STREAM,  // Stream socket
		IPPROTO_TCP); // TCP protocol

	// Check if socket is successfully created
	if (listenSocketPublisher == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	iResult = bind(listenSocketPublisher, resultingAddress->ai_addr, (int)resultingAddress->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(resultingAddress);
		closesocket(listenSocketPublisher);
		WSACleanup();
		return 1;
	}

	addrinfo *resultingAddress2 = NULL;
	addrinfo hints2;

	memset(&hints2, 0, sizeof(hints2));
	hints2.ai_family = AF_INET;       // IPv4 address
	hints2.ai_socktype = SOCK_STREAM; // Provide reliable data streaming
	hints2.ai_protocol = IPPROTO_TCP; // Use TCP protocol
	hints2.ai_flags = AI_PASSIVE;     // 

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, SERVER_PORT_SUBSCRIBER, &hints2, &resultingAddress2);
	if (iResult != 0)
	{
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	

	listenSocketSubscriber = socket(AF_INET,      // IPv4 address family
		SOCK_STREAM,  // Stream socket
		IPPROTO_TCP); // TCP protocol

	// Check if socket is successfully created
	if (listenSocketSubscriber == INVALID_SOCKET)
	{
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}


	// Setup the TCP listening socket - bind port number and local address to socket
	
	iResult = bind(listenSocketSubscriber, resultingAddress2->ai_addr, (int)resultingAddress2->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(resultingAddress2);
		closesocket(listenSocketSubscriber);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(resultingAddress);
	freeaddrinfo(resultingAddress2);

	bool bOptVal = true;
	int bOptLen = sizeof(bool);
	unsigned long  mode = 1;

	iResult = setsockopt(listenSocketPublisher, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char *)&bOptVal, bOptLen);
	if (iResult == SOCKET_ERROR) {
		printf("setsockopt for SO_CONDITIONAL_ACCEPT failed with error: %u\n", WSAGetLastError());
	}

	iResult = setsockopt(listenSocketSubscriber, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char *)&bOptVal, bOptLen);
	if (iResult == SOCKET_ERROR) {
		printf("setsockopt for SO_CONDITIONAL_ACCEPT failed with error: %u\n", WSAGetLastError());
	}
	
	if (ioctlsocket(listenSocketPublisher, FIONBIO, &mode) != 0)
		printf("ioctlsocket failed with error.");

	if (ioctlsocket(listenSocketSubscriber, FIONBIO, &mode) != 0)
		printf("ioctlsocket failed with error.");

	// Set listenSocket in listening mode
	iResult = listen(listenSocketPublisher, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocketPublisher);
		WSACleanup();
		return 1;
	}

	iResult = listen(listenSocketSubscriber, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(listenSocketSubscriber);
		WSACleanup();
		return 1;
	}

	printf("PubSubEngine server je uspesno pokrenut.\n");

	PublisherParameters publisherParams;
	publisherParams.socket = &listenSocketPublisher;
	publisherParams.queue = dataQueue;

	SubscriberParameters subParams;
	subParams.socket = &listenSocketSubscriber;
	subParams.key = hashset;

	SendToSubscriberParameters sendToSubParams;
	sendToSubParams.hashset = hashset;
	sendToSubParams.queue = dataQueue;

	DWORD pub,sub,seSub;
	HANDLE p,s,se;
	p = CreateThread(NULL, 0, &ListenPublisher, &publisherParams, 0, &pub);
	s = CreateThread(NULL, 0, &ListenSubscriber, &subParams, 0, &sub);
	se= CreateThread(NULL, 0, &SendToSubscriber, &sendToSubParams, 0, &seSub);

// ovo sam stavila da se ne ugasi konzola sama
	char ooo;
	scanf("%c", &ooo);
	WSACleanup();

	return 0;
}

