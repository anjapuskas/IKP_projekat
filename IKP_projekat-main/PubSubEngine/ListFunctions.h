#pragma once
//#ifndef LIST_OPERATIONS_H_
#define LIST_OPERATIONS_H_

#include "Structures.h"
#include "Util.h"
#include <ws2tcpip.h>
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

void InitList(SUBSCRIBERLIST* list);
node* create(SOCKET data, node* next);
void AddAtEnd(SOCKET data, SUBSCRIBERLIST* list);
void SendToAllInList(SUBSCRIBERLIST* list, char* message);