#include "SubscriberMenu.h"
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma pack(1)
#pragma warning(disable:4996)
#include "Structures.h"

Topic Menu()
{
	Topic top = {};

	top.topic = (char*)malloc(sizeof(char*));
	top.type = (char*)malloc(sizeof(char*));

	char vrstaTopica[10];
	char tipTopica[10];
	bool i = true;

	while (i)
	{

		printf("\n MENU \n");
		printf("\tIzaberite topic:\n");
		printf("\t\t1. ANALOG\n");
		printf("\t\t2. STATUS\n");

		gets_s(vrstaTopica, 10);
		int i = vrstaTopica[0] - '0';

		if (!(i == 1 || i == 2))
		{
			printf("\nUneli ste opciju koja ne postoji! Unesite ponovo.\n\n");
		}
		else
		{
			if (i == 1)
			{
				strcpy(top.topic, "ANALOG");
				break;
			}
			else
			{
				strcpy(top.topic, "STATUS");
				break;
			}
		}
	}
	while (i)
	{

		if (strcmp(top.topic, "STATUS") == 0)
		{
			printf("\tIzaberite tip topic-a:\n");
			printf("\t\t1. SWG\n");
			printf("\t\t2. CRB\n");

			gets_s(tipTopica, 10);
			int i = tipTopica[0] - '0';

			if (!(i == 1 || i == 2))
			{
				printf("\nUneli ste opciju koja ne postoji! Unesite ponovo.\n\n");
			}
			else
			{
				if (i == 1)
				{

					strcpy(top.type, "SWG");
					break;
				}
				else
				{

					strcpy(top.type, "CRB");
					break;

				}
			}
		}
		else
		{
			strcpy(top.type, "MER");
			break;
		}

	}

	printf("\n\n-------------------------\n");
	printf("TOPIC: %s, TYPE: %s\n", top.topic, top.type);

	return top;
}