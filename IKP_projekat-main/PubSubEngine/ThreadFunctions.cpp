#include "ThreadFunctions.h"
#include "Util.h"
#include "QueueFunctions.h"
#include "ListFunctions.h"
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#pragma pack(1)

#pragma warning(disable:4996) 


DWORD WINAPI ListenPublisher(LPVOID lpParam)
{
	PublisherParameters params = *(PublisherParameters*) lpParam;
	SOCKET listenSocket = *(SOCKET*)params.socket;

	SOCKET acceptedSocket=INVALID_SOCKET;
	short lastIndex = 0;
	char dataBuffer[BUFFER_SIZE];
	int iResult = -1;
	unsigned long int mode = 1; //neblokirajuci rezim

	while (true) {
		fd_set readfds;
		timeval timeVal;
		timeVal.tv_sec = 1;
		timeVal.tv_usec = 0;

		// initialize socket set
		FD_ZERO(&readfds); 
		FD_SET(listenSocket, &readfds); //stavljanje listen socketa u rezim read
	
		// wait for events on set
		int selectResult = select(0, &readfds, NULL, NULL, &timeVal); //da li je uticnica ima dogadjaj

		if (selectResult == SOCKET_ERROR)
		{
			printf("Select failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		else if (selectResult == 0) // timeout expired
		{
			continue;
		}
		else if (FD_ISSET(listenSocket, &readfds)) //select vraca>0, i provera da li je dogadjaj nastao na listenSocketu
		{
			lastIndex++;
			AcceptPublisherParameters pubPar;
			pubPar.acceptedSocket = &acceptedSocket;
			pubPar.publisherParams = &params;
			pubPar.num = lastIndex;

			DWORD pub;
			HANDLE p;

			p = CreateThread(NULL, 0, &AcceptPublisher, &pubPar, 0, &pub);
			Sleep(1000);

			CloseHandle(p);

		}
	}

}

DWORD WINAPI ListenSubscriber(LPVOID lpParam)
{
	char dataBuffer[BUFFER_SIZE];

	SubscriberParameters params = *(SubscriberParameters*)lpParam;
	SOCKET listenSocket = *(SOCKET*)params.socket;
	HashSetKeyValuePair* pair = params.key;
	
	SOCKET acceptedSocket = INVALID_SOCKET;
	short lastIndex = 0;
	int iResult = -1;
	unsigned long int mode = 1;

	fd_set readfds;
	timeval timeVal;
	timeVal.tv_sec = 1;
	timeVal.tv_usec = 0;

	while (true)
	{
		// initialize socket set
		FD_ZERO(&readfds);
		FD_SET(listenSocket, &readfds);

		int selectResult = select(0, &readfds, NULL, NULL, &timeVal);

		if (selectResult == SOCKET_ERROR)
		{
			printf("Select failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		else if (selectResult == 0) // timeout expired
		{
			continue;
		}
		else if (FD_ISSET(listenSocket, &readfds)) //select vraca>0, i provera da li je dogadjaj nastao na listenSocketu
		{

			lastIndex++;
			AcceptSubscriberParameters subPar;
			subPar.acceptedSocket = &acceptedSocket;
			subPar.subscriberParams = &params;
			subPar.num = lastIndex;

			DWORD sub;
			HANDLE s;

			s = CreateThread(NULL, 0, &AcceptSubscriber, &subPar, 0, &sub);
			Sleep(1000);
			
			CloseHandle(s);

		}
	}
}

DWORD WINAPI AcceptPublisher(LPVOID lpParam)
{
	char dataBuffer[BUFFER_SIZE];
	unsigned long int mode = 1;
	AcceptPublisherParameters params = *(AcceptPublisherParameters*)lpParam;

	DataQueue* queue = params.publisherParams->queue;

	SOCKET acceptedSocket = *(SOCKET*)(params.acceptedSocket);
	SOCKET listenSocket = *(SOCKET*)(params.publisherParams->socket);

	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(struct sockaddr_in);

    acceptedSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);

	if (acceptedSocket == INVALID_SOCKET)
	{
		if (WSAGetLastError() == WSAECONNRESET)
		{
			printf("accept failed, because timeout for client request has expired.\n");
		}
		else
		{
			printf("acceptt failed with error: %d\n", WSAGetLastError());
		}
	}
	else
	{
		if (ioctlsocket(acceptedSocket, FIONBIO, &mode) != 0)
		{
			printf("ioctlsocket failed with error.");

		}
		else
		{			
			int iResult = 0;
			do
			{
				FD_SET set;
				timeval timeVal;

				FD_ZERO(&set);
				FD_SET(acceptedSocket, &set);
				timeVal.tv_sec = 0;
				timeVal.tv_usec = 0;

				iResult = select(0, &set, NULL, NULL, &timeVal);

				if (iResult == SOCKET_ERROR)
				{
					fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());

					continue;
					
				}
				else if (iResult == 0)
				{
					Sleep(500);
					continue;
				}
				else if (FD_ISSET(acceptedSocket, &set))
				{

					iResult = recv(acceptedSocket, dataBuffer, BUFFER_SIZE, 0);
				    

					if (iResult > 0 )
					{
						dataBuffer[iResult] = '\0';
						
						Data data = Splituj(dataBuffer);

						Sleep(1000);
					
						printf("\nPublisher NO[%d]:",params.num);
						Enqueue(queue, &data);

					}
					else if (iResult == 0 )
					{
						// connection was closed gracefully
						printf("Connection with clientclosed.\n");
						closesocket(acceptedSocket);

					}
				}
			} while (iResult >= 0);
		}
	}
	return 0;
}

DWORD WINAPI AcceptSubscriber(LPVOID lpParam)
{
	char dataBuffer[BUFFER_SIZE];
	unsigned long int mode = 1;
	AcceptSubscriberParameters params = *(AcceptSubscriberParameters*)lpParam;

	SOCKET acceptedSocket = *(SOCKET*)(params.acceptedSocket);
	SOCKET listenSocket = *(SOCKET*)(params.subscriberParams->socket);

	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(struct sockaddr_in);

	HashSetKeyValuePair* kvp = params.subscriberParams->key;


	acceptedSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);

	if (acceptedSocket == INVALID_SOCKET)
	{
		if (WSAGetLastError() == WSAECONNRESET)
		{
			printf("accept failed, because timeout for client request has expired.\n");
		}
		else
		{
			printf("acceptt failed with error: %d\n", WSAGetLastError());
		}
	}
	else
	{
		if (ioctlsocket(acceptedSocket, FIONBIO, &mode) != 0)
		{
			printf("ioctlsocket failed with error.");

		}
		else
		{
			int iResult = 0;
			do
			{
				FD_SET set;
				timeval timeVal;

				FD_ZERO(&set);
				FD_SET(acceptedSocket, &set);
				timeVal.tv_sec = 0;
				timeVal.tv_usec = 0;

				iResult = select(0, &set, NULL, NULL, &timeVal);

				if (iResult == SOCKET_ERROR)
				{
					fprintf(stderr, "select failed with error: %ld\n", WSAGetLastError());

					continue;

				}
				else if (iResult == 0)
				{
					Sleep(500);
				//	continue;
				}
				else if (FD_ISSET(acceptedSocket, &set))
				{

					iResult = recv(acceptedSocket, dataBuffer, BUFFER_SIZE, 0);

					if (iResult > 0)
					{
						dataBuffer[iResult] = '\0';

						Topic top = SplitujSub(dataBuffer);


						for (int i = 0; i < 3; ++i) {
							if (strcmp(kvp[i].topic, top.topic) == 0 && strcmp(kvp[i].type, top.type) == 0) {
								AddAtEnd(acceptedSocket, &(kvp[i].list));
								break;
							}
						}

						Sleep(1000);

					}
					else if (iResult == 0)
					{
						// connection was closed gracefully
						printf("Connection with clientclosed.\n");
						closesocket(acceptedSocket);

					}
				
				}
			} while (iResult >= 0);
		}
	}
	return 0;
}

DWORD WINAPI SendToSubscriber(LPVOID lpParam) {

	char dataBuffer[BUFFER_SIZE];
	SendToSubscriberParameters params = *(SendToSubscriberParameters*)lpParam;
	HashSetKeyValuePair* hashset = params.hashset;
	DataQueue* queue = params.queue;
	SUBSCRIBERLIST list;

	char* message = (char*)malloc(sizeof(char*));
	char* topic = (char*)malloc(100);
	char* type = (char*)malloc(100);


	while (1)
	{
		Data data = Dequeue(queue);
		if (data.topic!=NULL) { //AKO POSTOJI NESTO U TOM REDU

			message = data.message;
			topic = data.topic;
			type = data.type;

			strcpy(dataBuffer, "Poruka: ");
			strcat(dataBuffer, message);
			strcat(dataBuffer, "\ntopic: ");
			strcat(dataBuffer, topic);
			strcat(dataBuffer, ", ");
			strcat(dataBuffer, type);
			strcat(dataBuffer, "\n");

			int duzina = strlen(dataBuffer);
			dataBuffer[duzina] = '\0';

			//PROVERAVAM HASHSET

			for (int i = 0; i < 3; ++i) {

				if (strcmp(topic, hashset[i].topic) == 0 && strcmp(type, hashset[i].type) == 0) {

					if (hashset[i].list.nodeCount > 0) {
						list = hashset[i].list;
						SendToAllInList(&list, dataBuffer);
					}

					break;
				}


			}
			Sleep(200);
			

		}
		
	}


}