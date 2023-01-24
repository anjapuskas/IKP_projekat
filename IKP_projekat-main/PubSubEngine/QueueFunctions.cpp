#include "QueueFunctions.h"
#include "Structures.h"
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#pragma pack(1)

#pragma warning(disable:4996)

DataQueue* InitQueue() {

	Data* dataArray = (Data*)malloc(sizeof(QUEUESIZE));

	DataQueue* queue = (DataQueue*)malloc(sizeof(DataQueue));
	queue->front = -1;
	queue->rear = -1;
	queue->dataList = dataArray;

	InitializeCriticalSection(&(queue->criticalSection));

	return queue;
}

void Enqueue(DataQueue* queue, Data* data)
{
	EnterCriticalSection(&(queue->criticalSection));

	if (queue->rear == QUEUESIZE - 1)
		printf("\nFULL!");
	else {
		if (queue->front == -1)
			queue->front = 0;
		queue->rear++;
		queue->dataList[queue->rear] = *(data);
		printf("\nInserted message -> message: %s  - signal: %s - type: %s", data->message, data->topic, data->type);
	}

	LeaveCriticalSection(&(queue->criticalSection));
}

Data Dequeue(DataQueue* queue)
{
	EnterCriticalSection(&(queue->criticalSection));

	Data data;

	if (queue->front == -1) {
		//printf("\nQueue is Empty!!");
		data.message = NULL;
		data.topic = NULL;
	}
	else {
		printf("\nDeleted : %s", (queue->dataList[queue->front]).message);

		data.message = (queue->dataList[queue->front]).message;
		data.topic = (queue->dataList[queue->front]).topic;
		data.type = (queue->dataList[queue->front]).type;

		queue->front++;
		if (queue->front > queue->rear)
			queue->front = queue->rear = -1;
	}

	LeaveCriticalSection(&(queue->criticalSection));


	return data;
}