#include <iostream>
#include <conio.h>
#include <fstream>
#include <stdlib.h>
#include "priority_queue.h"
#include "Declarations.h"
#include "utility.h"

void load_data(queue* enemyList, castle& ct, char* str);
void Print_information(queue* activeList[], queue* killedList, int paved2[], int size);
void Write_information(castle intialct, castle ct, queue* killedList, int sim, int enemyN, int paved2[], ofstream&  fout);
void Sort(queue* killedSameTime); // sorting accdording to FD



void load_data(queue* enemyList, castle& ct, char* str)
{
	enemy e;
	int TH, NE, TP;
	ifstream fin;
	fin.open(str, ios::in);

	fin >> TH;
	ct.towers[0].Health = ct.towers[1].Health = ct.towers[2].Health = ct.towers[3].Health = TH;

	fin >> NE;
	ct.towers[0].NE = ct.towers[1].NE = ct.towers[2].NE = ct.towers[3].NE = NE;

	fin >> TP;
	ct.towers[0].TP = ct.towers[1].TP = ct.towers[2].TP = ct.towers[3].TP = TP;

	fin >> c1 >> c2 >> c3;

	int x; char ch;
	fin >> x;
	while (x != -1)
	{
		e.S = x;
		fin >> e.T >> e.H >> e.P >> e.PR >> e.Type >> ch;
		if (ch == 'A')
			e.Region = 1;
		else if (ch == 'B')
			e.Region = 2;
		else if (ch == 'C')
			e.Region = 3;
		else if (ch == 'D')
			e.Region = 4;

		e.Distance = 60;
		e.timekilled = e.timeshot = 0;
		
		fin >> x;
		enqueue(enemyList, e);
	}

	fin.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Sort(queue* killedSameTime)
{
	int N = killedSameTime->count;
	enemy* enemies = new enemy[N], temp; //creating a temporary dynamic array
	int i = 0, j;

	//transfering to the array to facilitate sorting
	while (!IsEmpty(killedSameTime))
	{
		enemies[i] = dequeue(killedSameTime);
		i++;
	}

	//bubble sort code
	for (i = 0; i < N - 1; i++)
	{
		for (j = 0; j < N - 1; j++)
		{
			if ((enemies[j].timeshot - enemies[j].T) > (enemies[j + 1].timeshot) - (enemies[j + 1].T))
			{
				temp = enemies[j];
				enemies[j] = enemies[j + 1];
				enemies[j + 1] = temp;
			}
		}
	}

	//returning it back from the array
	for (i = 0; i<N; i++)
		enqueue(killedSameTime, enemies[i]);

	delete enemies;//deleting the temporary array
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Write_information(castle intialCt, castle ct, queue* killedList, int s, int no_enemies, int paved2[], ofstream&  fout)
{
	int  x;
	float FD, KD, FT, avgFD = 0, avgKD = 0, totKilled;
	queue* killedSameTime = createQueue();
	enemy Enemy, e ;
	totKilled = killedList->count;

	//moving enemies killed at same timestep
	while (!IsEmpty(killedList))
	{
		Enemy = dequeue(killedList);
		enqueue(killedSameTime, Enemy);
		x = Enemy.timekilled;

		while (!IsEmpty(killedList))
		{

			e = front(killedList);
			if (x != e.timekilled)
				break;   
			dequeue(killedList);
			enqueue(killedSameTime, e);
			x = e.timekilled;
		}

		Sort(killedSameTime);

        //getting FD,KD,FT at each time step
		while (!IsEmpty(killedSameTime))
		{
			Enemy = dequeue(killedSameTime);
			FD = Enemy.timeshot - Enemy.T;
			KD = Enemy.timekilled - Enemy.timeshot;
			FT = FD + KD;
			avgFD += FD;
			avgKD += KD;
			fout << Enemy.timekilled << " " << Enemy.S << " " << FD << " " << KD << " " << FT << endl;
		}


	}
	//get the change in towers' health (total damage)
	fout << (intialCt.towers[0].Health) - (ct.towers[0].Health) << " " << (intialCt.towers[1].Health) - (ct.towers[1].Health) << ' ';
	fout << (intialCt.towers[2].Health) - (ct.towers[2].Health) << " " << (intialCt.towers[3].Health) - (ct.towers[3].Health) << endl;

	//remaining unpaved distance in each region
	fout << ((paved2[0] <= 30) ? 30 : 60 - paved2[0]) << " " << ((paved2[1] <= 30) ? 30 : 60 - paved2[1]) << ' ';
	fout << ((paved2[2] <= 30) ? 30 : 60 - paved2[2]) << " " << ((paved2[3] <= 30) ? 30 : 60 - paved2[3]) << endl;

	//Final statistics
	if (s == 0)
	{
		fout << "Win" << endl;
		fout << no_enemies << endl;
		fout << avgFD / no_enemies << endl;
		fout << avgKD / no_enemies << endl;
	}
	else if (s == -1)
	{
		fout << "Lose" << endl;
		fout << totKilled << endl;
		fout << (no_enemies - totKilled) << endl;
		fout << (avgFD / totKilled) << endl;
		fout << (avgKD / totKilled) << endl;
	}
	else
		fout << "Continue playing" << endl;

	
	destroyQueue(killedSameTime);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Print_information(queue* activeList[], queue* killedList, int paved2[], int size)
{
	int i, j,SizeActive, SizeKilled;
	enemy** enemiesR[4], E;

	//creating an array for each region & enqueuing enemies in it respectively
	enemiesR[0] = new enemy* [activeList[0]->count];
	for (i = 0; i<activeList[0]->count; i++) 
		enemiesR[0][i] = new enemy;

	enemiesR[1] = new enemy*[activeList[1]->count];
	for (i = 0; i<activeList[1]->count; i++)
		enemiesR[1][i] = new enemy;

	enemiesR[2] = new enemy*[activeList[2]->count];
	for (i = 0; i<activeList[2]->count; i++)
		enemiesR[2][i] = new enemy;

	enemiesR[3] = new enemy*[activeList[3]->count];
	for (i = 0; i<activeList[3]->count; i++)
		enemiesR[3][i] = new enemy;

	//activating the enemies in each region respectively
	for (i = 0; i<size; i++)
	{
		cout << "Region" << (i == 0 ? 'A' : i == 1 ? 'B' : i == 2 ? 'C' : 'D') << "  : ";
		SizeActive = activeList[i]->count;
		for (j = 0; j<SizeActive; j++)
		{
			*enemiesR[i][j] = dequeue(activeList[i]);
			enqueue(activeList[i], *enemiesR[i][j]);
		}
	}

	//killing enemies and calculating their statistics
	int sum[4] = { 0 }, N[4] = { 0 };
	for (i = 0; i<4; i++)
	{
		for (j = 0; j<killedList->count; j++)
		{
			E = dequeue(killedList);
			if (E.Region - 1 == i)
			{
				if (E.timekilled == timeStep - 1)
					N[i]++;
				sum[i]++;
			}
			enqueue(killedList, E);

		}
	}


	//displaying the instaneous counters on the console
	for (i = 0; i<4; i++)
	{
		DrawEnemies(enemiesR[i], activeList[i]->count);
		cout << "Region" << (i == 0 ? 'A' : i == 1 ? 'B' : i == 2 ? 'C' : 'D') << ": ";
		cout << " Active Elements = " << activeList[i]->count;
		cout << " --- killed at last step " << N[i];
		cout << " --- Over all Killed = " << sum[i];
		cout << " --- Unpaved Distance = " << ((paved2[i] <= 30) ? 30 : 60 - paved2[i]) << endl;
	}
	cout << endl;
	color(FOREGROUND_BLUE);
	cout << "\t\t\t" << (char)219 << " Fighter";
	color(FOREGROUND_RED);
	cout << "\t\t\t" << (char)219 << " Paver";
	color(FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "\t\t\t" << (char)219 << " Shielded Fighter"<<'\n';
}

