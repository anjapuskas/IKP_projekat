#include "PublisherMenu.h"
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma pack(1)
#pragma warning(disable:4996)

Data Menu()
{
	Data data = {};

	data.message = (char*)malloc(sizeof(char*));
	data.topic = (char*)malloc(sizeof(char*));
	data.type = (char*)malloc(sizeof(char*));

	char vrstaTopica[10];
	char tipTopica[10];
	char poruka[BUFFER_SIZE];
	bool i = true;

	while (i)
	{

		printf("\n MENI \n");
		printf("\tIzaberite topic:\n");
		printf("\t\t1. ANALOG\n");
		printf("\t\t2. STATUS\n");

		gets_s(vrstaTopica,10);
		int in = vrstaTopica[0] - '0';
		
		if (!(in == 1 || in == 2))
		{
			printf("\nUneli ste opciju koja ne postoji! Unesite ponovo.\n\n");
		}
		else
		{
			if (in == 1)
			{
				strcpy(data.topic, "ANALOG");
				break;
			}
			else
			{
				strcpy(data.topic, "STATUS");
				break;
			}
		}
	}
	while (i)
	{
					
			if (strcmp(data.topic, "STATUS") == 0)
			{
				printf("\tIzaberite tip topic-a:\n");
				printf("\t\t1. SWG\n");
				printf("\t\t2. CRB\n");

				gets_s(tipTopica,10);
				int in = tipTopica[0] - '0';

				if (!(in == 1 || in == 2))
				{
					printf("\nUneli ste opciju koja ne postoji! Unesite ponovo.\n\n");
				}
				else
				{
					if (in == 1)
					{
						
						strcpy(data.type, "SWG");
						break;
					}
					else
					{
				
						strcpy(data.type, "CRB");
						break;

					}
				}
			}
			else
			{
				strcpy(data.type, "MER");
				break;
			}
			
	}

	while (i)
	{
			printf("\tUnesite poruku:\n");
			gets_s(poruka, BUFFER_SIZE);
			strcpy(data.message, poruka);
			break;

	}
	
	printf("\n\n--------------------\n");
	printf("TOPIC: %s, TYPE: %s, MESSAGE: %s",data.topic,data.type,data.message);
		

	return data;

}