#pragma once
#include <ws2tcpip.h>
#include <stdio.h>
#include "Structures.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFFER_SIZE 512

/*
Funkcije: Menu
-----------------------------------------
Funkcionalnost:Pravi korisnicki meni

Povratna vrednost: Promenljiva tipa Data(nasa struktura) koji se dobije nakon sto korisnik izabere vrednosti istog
*/

Data Menu();
