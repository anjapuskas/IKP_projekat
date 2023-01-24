#include <ws2tcpip.h>
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma once

typedef struct List {
	struct node* head;
	struct node* tail;
	int nodeCount;

}SUBSCRIBERLIST;


typedef struct node {
	SOCKET subscriberSocket;
	struct node* next;
}NODE;

typedef struct HashSetKeyValuePair {

	const char* topic;
	const char* type;
	SUBSCRIBERLIST list;
};

typedef struct Topic {
	char* topic;
	char* type;
};

typedef struct Data {
	char* topic;
	char* type;
	char* message;
};


typedef struct DataQueue {
	Data* dataList;
	int front;
	int rear;
	CRITICAL_SECTION criticalSection;
};

typedef struct PublisherParameters
{
	SOCKET* socket;
	DataQueue* queue;
};
typedef struct SubscriberParameters
{
	SOCKET* socket;
	HashSetKeyValuePair* key;
};

typedef struct AcceptPublisherParameters
{
	SOCKET* acceptedSocket;
	PublisherParameters* publisherParams;
	int num;
};

typedef struct AcceptSubscriberParameters
{
	SOCKET* acceptedSocket;
	SubscriberParameters* subscriberParams;
	int num;
};


typedef struct SendToSubscriberParameters
{
	DataQueue* queue;
	HashSetKeyValuePair* hashset;
};
