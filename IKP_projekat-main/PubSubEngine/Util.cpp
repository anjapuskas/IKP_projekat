#pragma once
#include "Util.h"
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma pack(1)
#pragma warning(disable:4996) 
#include "Structures.h"

Data Splituj(char* dataBuffer)
{
	Data data = {};
	Topic top = {};

	data.topic = (char*)malloc(sizeof(char*));
	data.type = (char*)malloc(sizeof(char*));
	data.message = (char*)malloc(sizeof(char*));

	char* dataBufferTemp = dataBuffer;

	data.message = dataBuffer + 11;
	dataBufferTemp[10] = '\0';
	data.type = strtok(dataBufferTemp + 6, "|");
	dataBufferTemp[6] = '\0';
	data.topic = strtok(dataBufferTemp, "|");

	return data;
}
void createHashSet(HashSetKeyValuePair* kvp) {

	SUBSCRIBERLIST sockets1;
	InitList(&sockets1);
	kvp[0].topic = "ANALOG";
	kvp[0].type = "MER";
	kvp[0].list = sockets1;

	SUBSCRIBERLIST sockets2;
	InitList(&sockets2);
	kvp[1].topic = "STATUS";
	kvp[1].type = "SWG";
	kvp[1].list = sockets2;

	SUBSCRIBERLIST sockets3;
	InitList(&sockets3);
	kvp[2].topic = "STATUS";
	kvp[2].type = "CRB";
	kvp[2].list = sockets3;

}


Topic SplitujSub(char* dataBuffer)
{
	Topic top = {};	

	top.topic = (char*)malloc(sizeof(char*));
	top.type = (char*)malloc(sizeof(char*));

	char* dataBufferTemp = dataBuffer;

	top.type = strtok(dataBufferTemp + 6, "|");
	dataBufferTemp[6] = '\0';
	top.topic = strtok(dataBufferTemp, "|");

	return top;
}



void SendToSub(char* message, SOCKET* socket)
{
	SOCKET connectSocket = *socket;
	int iResult;

	int duzina = strlen(message);
	iResult = send(connectSocket, message, duzina, 0);
	Sleep(1000);
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();
		//return 1;
	}
	
}