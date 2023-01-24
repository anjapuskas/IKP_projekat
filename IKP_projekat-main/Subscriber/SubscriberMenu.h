#pragma once
#include <ws2tcpip.h>
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BUFFER_SIZE 512
#include "Structures.h"

/*
Funkcije: Menu
-----------------------------------------
Funkcionalnost:Pravi korisnicki meni

Povratna vrednost: Promenljiva tipa Topic(nasa struktura) koji se dobije nakon sto korisnik izabere vrednosti istog
*/

Topic Menu();