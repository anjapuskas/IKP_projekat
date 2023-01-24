#pragma once

#include <ws2tcpip.h>
#include <stdio.h>
#include "Structures.h"

#define QUEUESIZE 10
#define _WINSOCK_DEPRECATED_NO_WARNINGS

DataQueue* InitQueue();
void Enqueue(DataQueue* queue, Data* data);
Data Dequeue(DataQueue* queue);