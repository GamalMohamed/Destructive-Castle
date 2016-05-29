#include "Declarations.h"
#include "priority_queue.h"

//Queue basic operations (written as in the reference)

queue* createQueue()
{
	queue*pQueue = new queue;
	pQueue->front = NULL;
	pQueue->rear = NULL;
	pQueue->count = 0;
	return pQueue;
}

void enqueue(queue *pQueue, enemy Enemy)
{
	node * newNode = new node;
	newNode->Data = Enemy;
	if (pQueue->front == NULL)
	{
		pQueue->rear = newNode;
		pQueue->front = newNode;
	}
	else 
	{
		pQueue->rear->next = newNode;

		pQueue->rear = newNode;
	}

	newNode->next = NULL;
	pQueue->count++;
}


void enqueueInOrder(queue *pQueue, enemy & Enemy)
{
	node * newNode = new node;
	newNode->Data = Enemy;

	if (pQueue->front == NULL)
	{
		pQueue->rear = newNode;
		pQueue->front = newNode;
		newNode->next = NULL;
	}
	else if (Enemy.priority() > pQueue->front->Data.priority())
	{
		newNode->next = pQueue->front;
		pQueue->front = newNode;
	}
	else
	{
		node*current = pQueue->front;
		while (current->next != NULL)
			if (Enemy.priority() < current->next->Data.priority())
				current = current->next;
			else
				break;

		if (current->next == NULL)
		{
			pQueue->rear = newNode; newNode->next = NULL;
		}
		newNode->next = current->next;
		current->next = newNode;

	}
	pQueue->count++;
}

enemy dequeueInOrder(queue* pQueue)
{

	node*delNode = pQueue->front;
	enemy elem = delNode->Data;

	if (delNode->next == NULL)
	{
		pQueue->front = pQueue->rear = NULL;
	}
	else
		pQueue->front = delNode->next;

	delete delNode;
	pQueue->count--;
	return elem;
}

enemy dequeue(queue* pQueue)
{

	node*delNode = pQueue->front;
	enemy elem = delNode->Data;

	if (delNode->next == NULL)
	{
		pQueue->front = pQueue->rear = NULL;
	}
	else
		pQueue->front = delNode->next;


	delete delNode;
	pQueue->count--;
	return elem;
}

enemy front(queue* pQueue)
{
	return pQueue->front->Data;

}

enemy rear(queue* pQueue)
{
	return pQueue->rear->Data;

}

bool IsEmpty(queue* pQueue)
{
	return pQueue->front == NULL;
}

void destroyQueue(queue* pQueue)
{
	while (!IsEmpty(pQueue))
		dequeue(pQueue);
	delete pQueue;
}

extern int time;