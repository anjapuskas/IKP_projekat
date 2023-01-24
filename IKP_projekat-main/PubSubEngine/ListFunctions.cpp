#include "ListFunctions.h"
#include "Util.h"



//inicira listu
void InitList(SUBSCRIBERLIST* list) {
	list->head = NULL;
	list->nodeCount = 0;
	list->tail = NULL;
}

node* create(SOCKET sub, node* next) {

	node* newNode = (node*)malloc(sizeof(node));

	if (newNode == NULL) {
		printf("\nproblem pri incijalizaciji prostora\n");
		exit(0);
	}
	newNode->subscriberSocket = sub;
	newNode->next = next;
	return newNode;
}

void AddAtEnd(SOCKET data, SUBSCRIBERLIST *list) {
	if (list == NULL) {
		printf("\nLista prazna\n");
		return;
	}
	node* newNode;
	//razlikuje se ako je lista prazna i ako ne
	if (list->nodeCount == 0) {
		newNode = create(data, NULL);
		list->head = newNode;
		list->tail = newNode;
		list->nodeCount++;
	}
	else {
		newNode = create(data, NULL);
		list->nodeCount++;

		list->tail->next = newNode;
		list->tail = newNode;

	}
}

void SendToAllInList(SUBSCRIBERLIST* list, char* message) {


	node* cursor = list->head;
	while (cursor != NULL) {
		SendToSub(message, &(cursor->subscriberSocket));
		cursor = cursor->next;
	}

	
}


