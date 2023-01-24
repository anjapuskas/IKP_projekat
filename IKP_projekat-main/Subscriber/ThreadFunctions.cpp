#include "ThreadFunctions.h"
#include "SubscriberMenu.h"
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#pragma pack(1)

#pragma warning(disable:4996) 


DWORD WINAPI Subscribe(LPVOID lpParam)
{
	SOCKET connectSocket = *(SOCKET*)lpParam;
	char dataBuffer[BUFFER_SIZE];
	int iResult = -1;
	Topic top = Menu();

	memset(dataBuffer, 0, BUFFER_SIZE);

	strcpy(dataBuffer, top.topic);
	strcpy(dataBuffer + 6, "|");
	strcpy(dataBuffer + 7, top.type);
	strcpy(dataBuffer + 10, "|");

	iResult = send(connectSocket, (char*)&dataBuffer, (int)strlen(dataBuffer), 0);

	if (iResult == SOCKET_ERROR)
	{
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(connectSocket);
		WSACleanup();		
		return 1;
	}

	
}

DWORD WINAPI ReceiveFromPubSub(LPVOID lpParam)
{
	char dataBuffer[BUFFER_SIZE];
	SOCKET acceptedSocketServer = *(SOCKET*)lpParam;
	int result = -1;
	
	while (1) {
		result = -1;
		while (result == -1) {

			result = recv(acceptedSocketServer, dataBuffer, BUFFER_SIZE, 0);

		//	Sleep(2000);
			if (result > 0)
			{
				dataBuffer[result] = '\0';
				printf("\nNew: %s\n", dataBuffer);
				Sleep(1000);

			}
			else if (result == 0)
			{
				printf("Connection with server closed.\n");
				//closesocket(acceptedSocketServer);
			}
			else
			{
				int ierr = WSAGetLastError();
				if (ierr == WSAEWOULDBLOCK) {  // currently no data available
					Sleep(50);  // wait and try again
					continue;
				}
			
			}
		}
	}
}