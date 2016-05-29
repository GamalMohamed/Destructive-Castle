#include <iostream>
#include "Declarations.h"
using namespace std;

struct node
{
	enemy Data;
	node* next;
};

struct queue
{
	int count;
	node* front;
	node* rear;
};


queue* createQueue();

void  destroyQueue(queue* pQueue);

void  enqueue(queue* pQueue, enemy Enemy);

void  enqueueInOrder(queue* pQueue, enemy& Enemy); // insert sorted (descendingly)

enemy dequeueInOrder(queue* pQueue); // dequeue sorted (descendingly)

enemy dequeue(queue* pQueue);

enemy front(queue* pQueue);

enemy rear(queue* pQueue);

bool  IsEmpty(queue* pQueue);
