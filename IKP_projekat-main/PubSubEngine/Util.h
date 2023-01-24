#include <ws2tcpip.h>
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Structures.h"
#include "ListFunctions.h"
#pragma once

/*
Funkcije: Splituj
-----------------------------------------
Funkcionalnost:Od promenljive tipa char* splitovanjem izdvaja parametre i smesna u promenljivu tipa Data

Povratna vrednost: Promenljiva tipa Data(nasa struktura)
*/

Data Splituj(char* dataBuffer);

/*
Funkcije: createHashSet
-----------------------------------------
Funkcionalnost: Kreira HashSet

Povratna vrednost: Nema
*/

void createHashSet(HashSetKeyValuePair* kvp);

/*
Funkcije: SplitujSub
-----------------------------------------
Funkcionalnost:Od promenljive tipa char* splitovanjem izdvaja parametre i smesta u promenljivu tipa Topic

Povratna vrednost: Promenljiva tipa Topic(nasa struktura)
*/

Topic SplitujSub(char* dataBuffer);

/*
Funkcije: SendToSub
-----------------------------------------
Funkcionalnost: Salje poruku subscriber-u

Povratna vrednost: Nema
*/

void SendToSub(char* message,SOCKET* socket);