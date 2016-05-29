#include <iostream>
#include <fstream>
#include <conio.h>
#include "priority_queue.h"
#include "Declarations.h"
#include "utility.h"
using namespace std;

int  Play(queue* enemyList, castle &ct, queue* activeList[], queue* killedList, int paved1[], int paved2[], int size);
bool Fight(tower t[], queue* q[], queue* killedList, int p1[], int p2[]); // THIS FUNCTION IS ACTUALLY THE HEART OF THE PROGRAM :D 
bool Activate(tower t[], queue* enemyList, queue* activeList[], int size);
bool TransferActiveEnemies(tower t[], queue* q[], int &region); //handling case of destroyed towers (already existing enemies)
void paving(enemy &e, int &b);
void damagetoTower(tower &Tower, enemy e1);
void damagetoEnemy(tower Tower, enemy &e1);
void update_distance(enemy &e, int b);



bool Activate(tower t[], queue* enemyList, queue* activeList[], int size)
{
	enemy Enemy;
	int region, count;

	//activating enemies
	if (!IsEmpty(enemyList))
	{
		Enemy = front(enemyList);
		while (Enemy.T == timeStep && Enemy.H>0 && (!IsEmpty(enemyList)))
		{
			count = 0;
			Enemy = dequeue(enemyList);
			region = Enemy.Region - 1;

			while ((t[region].Health <= 0) && count <3) //case of destroyed towers (newly inserted enemies) 
			{
				region = (region + 1) % 4;
				count++;
			}
			if (count == 3)
				return false;

			enqueueInOrder(activeList[region], Enemy);
			if (!IsEmpty(enemyList)) 
				Enemy = front(enemyList);
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Play(queue* enemyList, castle  &ct, queue* activeList[], queue* killedList, int paved1[], int paved2[], int size)
{
	bool lose;
	//all towers are destroyed
	lose = !(Activate(ct.towers, enemyList, activeList, 4)); 
	if (lose)
		return -1;

	lose = !(Fight(ct.towers, activeList, killedList, paved1, paved2)); 
	if (lose) 
		return -1;

	if ((enemyList->count + activeList[0]->count + activeList[1]->count + activeList[2]->count + activeList[3]->count) == 0)//no enemies
		return 0;

	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Fight(tower t[], queue* e[], queue* killedList, int p1[], int p2[])
{ 
	//e[] aray of active enimes for each region,n=NE,p1[] is the already paved distance,p2[] will change only in case of paver enemy
	tower Tower[4];
	bool check;
	enemy E;
	int NE = t[0].NE, numE, rem, NEQueuesize;
	queue* NEQueue = createQueue(); //Queue to store the eneimes from the pirority queue
	
	//initializing p2 from p1 for all regions
	for (int k = 0; k<4; k++)
		p2[k] = p1[k];

	// This Big LOOP is the core of the whole program :D 
	for (int j = 0; j<4; j++)
	{
		if (t[j].Health <= 0) // tower is destroyted
			continue;

		else
		{
			numE = e[j]->count;
			if (numE == 0)    // if there are no enemies do nothing
				continue;
		
			
			// Enemies will Attack the tower 
			// Update tower Health

			Tower[j] = t[j];

			//Order of enemies is irrelevent here
			for (int i = 0; i<numE; i++)
			{
				E = dequeue(e[j]);

				if (E.Type != 1)
					damagetoTower(Tower[j], E);

				enqueue(e[j], E);
			}

			/* if total number of enemeies numE >= NE,then dequeue the NE highest pirority enemies
			by repeativie dequeue from the pirority queue into a normal queue		
			Here tower attack NE enemies*/

			if (numE >= NE)
			{
				/*Transferring NE enemies from priority queue into a normal queue
				It is done to prevent from processing the same enemy twice
				i.e. if the enemy with highest priority after processing is enqueued then it may be processed again*/

				for (int i = 0; i<NE; i++)
				{
					E = dequeueInOrder(e[j]);
					enqueue(NEQueue, E);
				}

				//Processing the NE Queue and enqueue them back into the priority queue
				for (int i = 0; i<NE; i++)
				{
					E = dequeue(NEQueue);
					damagetoEnemy(t[j], E);

				// all active non paver enemies will damage the tower and move and if paver it'll pave

					if (E.H>0)
						enqueueInOrder(e[j], E);
					else
					{
						enqueue(killedList, E);
						numE--;
					}
				}
			}
			else
			{
				/* if total number of enemeies numE < NE ,then dequeue the numE highest pirority enemies
				only numE enemies will take actions [pave if paver ] -[damage tower and move if not paver]
				But tower will processing them and change their priorities again and again until process NE enemies*/

				rem = NE;

				do{
					NEQueuesize = rem >= numE ? numE : rem;
					if (rem >= numE)
						rem -= numE;
					else
						rem = 0;

					for (int i = 0; i<NEQueuesize; i++)
					{
						E = dequeueInOrder(e[j]);
						enqueue(NEQueue, E);
					}
					for (int i = 0; i<NEQueuesize; i++)
					{
						E = dequeue(NEQueue);
						damagetoEnemy(t[j], E);
						if (E.H>0)
							enqueueInOrder(e[j], E);
						else
						{
							enqueue(killedList, E);
							numE--;
						}
					}


				} while (rem>0 && numE>0);
			}

			//transfering enemies
			t[j] = Tower[j];
			if (t[j].Health <= 0)
			{
				check = TransferActiveEnemies(t, e, j);
				if (!check) 
					return false;// All towers are destroyed.........Game over !! 
			}

			//updating distances
			for (int i = 0; i<numE; i++)
			{
				E = dequeue(e[j]);
				if (E.Type == 1)
					paving(E, p2[j]);
				else
					update_distance(E, p1[j]);
				enqueue(e[j], E);
			}

		}
	}
	//updating the paved distances again
	for (int k = 0; k<4; k++)
		p1[k] = p2[k];
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void paving(enemy &e, int &p)
{
	int remainingtime;
	bool pave;

	if (e.Distance >2)
	{
		remainingtime = e.PR - ((timeStep - e.T) % e.PR);
		pave = (60 - e.Distance) < p;
		if (remainingtime == e.PR)  //It's his time to shoot (pave)
		{
			e.Distance -= e.P;
			if (e.Distance<2) //minimum distance must be always 2
				e.Distance = 2;
			p = (60 - e.Distance);
		}
		if (remainingtime != e.PR && pave)
			(e.Distance)--;

		//else do nothing
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void damagetoTower(tower &Tower, enemy e1)
{
	float Damagetotower;
	int remainingtime = e1.PR - ((timeStep - e1.T) % e1.PR);
	if (remainingtime == e1.PR)
	{
		Damagetotower = (1.0 / (float)e1.Distance)*(float)e1.P; //typecasted to float to get the exact value of damage
		Tower.Health -= Damagetotower;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void damagetoEnemy(tower Tower, enemy &e)
{
	float Damagetoenemy, k;
	if (e.Type == 3) //Shielded enemies
		k = 2;
	else         //otherwise
		k = 1;

	Damagetoenemy = (1.0 / (float)e.Distance)*(float)Tower.TP*(1.0 / k); ////typecasted to float to get the exact value of damage
	e.H -= Damagetoenemy;

	if (e.H <= 0)
		e.timekilled = timeStep;

	if (e.timeshot == 0)
		e.timeshot = timeStep;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void update_distance(enemy &e, int paved)
{
	if (e.Distance > 2)
	{
		if ((60 - (e.Distance)) < paved)
			e.Distance--;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TransferActiveEnemies(tower t[], queue* q[], int &region)
{
	int j, count = 0;
	enemy e;
	j = (region + 1) % 4; //A-B-C-D rotative direction

	//Moving enemies from region to another respectively
	while (t[j].Health <= 0 && count < 3)
	{
		j = (j + 1) % 4;
		count++;
	}
	if (count == 3) 
		return false;

	while (!IsEmpty(q[region]))
	{
		e = dequeue(q[region]);
		enqueueInOrder(q[j], e);
	}
	region = j;
	return true;
}