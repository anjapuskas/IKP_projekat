#pragma once

#include <ws2tcpip.h>
#include <stdio.h>
#include "Structures.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFFER_SIZE 512

/*
Funkcije: Subscribe
-----------------------------------------
Funkcionalnost: Subscribuje se na odredjen Topic i svoj izbor salje serveru

*/
DWORD WINAPI Subscribe(LPVOID lpParam);

/*
Funkcije: ReceiveFromPubSub
-----------------------------------------
Funkcionalnost: Prima odgovor od servera 

*/
DWORD WINAPI ReceiveFromPubSub(LPVOID lpParam);