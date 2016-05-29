#include <iostream>
#include <fstream>
#include <conio.h>
#include <stdlib.h>
#include "utility.h"
#include "priority_queue.h"
using namespace std;

//Some declarations and prototypes
int timeStep;
float c1, c2, c3;
void load_data(queue* enemyList, castle& ct, char* str);//from the file
void Print_information(queue* activeList[], queue* killedList, int paved2[], int size);//on the console
void Write_information(castle intialCt, castle ct, queue* killedList, int s, int enemyN, int paved2[], ofstream&  fout);//in file
int  Play(queue* enemyList, castle& ct, queue* activeList[], queue* killedList, int paved1[], int paved2[], int size);


void main()
{
	SetWindow();

	// define the castle 
	castle ct, intialCt;
	ct.Xstrt = CastleXStrt;
	ct.Ystrt = CastleYStrt;
	ct.W = CastleWidth;
	ct.L = CastleLength;

	//intialize towers' health
	ct.towers[0].Health = ct.towers[1].Health = ct.towers[2].Health = ct.towers[3].Health = 200; 
	
	int i, n, x, mode, no_enemies, paved1[4], paved2[4];
	queue *enemyList, *killedList, *activeList[4];//different lists of enemies
	ofstream fout;//output file

	//array of pointers to characters (to read the input files one after the other automatically)
	char* In[10] = { "ModCastle-FewEnemies-InputF.txt", "ModCastle-ManyEnemies-InputF.txt", "ModCastle-ModEnemies-InputF.txt", "StrongCastle-FewEnemies-InputF.txt", "StrongCastle-ManyEnemies-InputF.txt", "StrongCastle-ModEnemies-InputF.txt", "WeakCastle-FewEnemies-InputF.txt", "WeakCastle-ManyEnemies-InputF.txt", "WeakCastle-ModEnemies-InputF.txt", "in.txt" };
	char* Out[10] = { "ModCastle-FewEnemies-OutputF.txt", "ModCastle-ManyEnemies-OutputF.txt", "ModCastle-ModEnemies-OutputF.txt", "StrongCastle-FewEnemies-OutputF.txt", "StrongCastle-ManyEnemies-OutputF.txt", "StrongCastle-ModEnemies-OutputF.txt", "WeakCastle-FewEnemies-OutputF.txt", "WeakCastle-ManyEnemies-OutputF.txt", "WeakCastle-ModEnemies-OutputF.txt", "out.txt" };

	//Game starts on each input file one after the other automatically
	for (n = 0; n<10; n++)
	{
		//choose the mode 
		cout << "For the " << n + 1 << ((n == 0) ? "st" : ((n == 1) ? "nd" : ((n == 2) ? "rd" : "th"))) << " input file" << endl;
		cout <<"\n\tChoose\n[0]-Interactive mode\n[1]-Step by Step Mode\n[2]-Silent Mode\n";
		cin >> mode;

		//create enemies lists
		enemyList = createQueue();
		killedList = createQueue();

		//create active enemies list for each region and initialize the paving distance
		for (i = 0; i<4; i++)
		{
			activeList[i] = createQueue();
			paved1[i] = paved2[i] = 30; //current and next Paved Distance for each region
		}

		//load data from the file
		load_data(enemyList, ct, In[n]);
		no_enemies = enemyList->count;
		intialCt = ct;

		fout.open(Out[n], ios::out);  //open output file

		// War starts !!
		x = 1; timeStep = 0;
		while (x == 1 && timeStep <1000)    //assume that maximum no. of timesteps is 1000 timestep (as we must set an upperlimit for the game for the step by step mode and to avoid any unexpected things)  
		{
			timeStep++;

			x = Play(enemyList, ct, activeList, killedList, paved1, paved2, 4);

			if (mode == 0)
				getchar(); //interactive mode
			else if (mode == 1)
				Sleep(1000); //step by step mode
	
			if (mode == 0 || mode == 1) //Draw and display instantenous results of data changes
			{
				DrawCastle(ct, timeStep);
				Print_information(activeList, killedList, paved2, 4);
			}

		}

		//write statistics
		Write_information(intialCt, ct, killedList, x, no_enemies, paved2, fout);
		fout.close(); //close output file

		//Free the memory 
		//delete the enemy list,the active enemies list for each region,the killed enemies list
		destroyQueue(enemyList);

		for (i = 0; i<4; i++)
			destroyQueue(activeList[i]);
		
		destroyQueue(killedList);
	}
}


