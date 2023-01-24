#pragma once

#include <ws2tcpip.h>
#include <stdio.h>
#include "Structures.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFFER_SIZE 512

/*
Funkcije: ListenPublisher
-----------------------------------------
Funkcionalnost: Osluskuje da li se pojavio novi Publisher
*/

DWORD WINAPI ListenPublisher(LPVOID lpParam);

/*
Funkcije: ListenSubscriber
-----------------------------------------
Funkcionalnost: Osluskuje da li se pojavio novi Subscriber
*/

DWORD WINAPI ListenSubscriber(LPVOID lpParam);

/*
Funkcije: AcceptPublisher
-----------------------------------------
Funkcionalnost: Prihvatanje dolaznog zahteva za uspostavu veze
*/
DWORD WINAPI AcceptPublisher(LPVOID lpParam);

/*
Funkcije: AcceptSubscriber
-----------------------------------------
Funkcionalnost: Prihvatanje dolaznog zahteva za uspostavu veze
*/
DWORD WINAPI AcceptSubscriber(LPVOID lpParam);

/*
Funkcije: SendToSubscriber
-----------------------------------------
Funkcionalnost: Skida publisherove poruke sa reda(queue) i salje ih odredjenim subscriberima koji su se pretplatili za taj topic
*/
DWORD WINAPI SendToSubscriber(LPVOID lpParam);
