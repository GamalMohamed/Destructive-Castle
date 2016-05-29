#ifndef DECLARATIONS_H  //this preprocesssor directrive is placed to prevent the common error of variable redeclaration
#define DECLARATIONS_H

struct enemy
{
	int S;             // sequence Number
	int T;            // arrival time
	int PR;          // hit period
	int Type;        
	int Region;
	int Distance;
	int timekilled;
	int timeshot;//first time to be shot
	float H;    // health
	float P;   // power
	float priority(); //prefered to be put it as a member function to be easily handled
	char state; 
};

struct tower
{
	int TW;           // tower width
	int TL;          // tower height
	int NE;         // number of enemies
	int TP;        // tower Power
	float Health; // tower health
	
};


struct castle
{
	//starting x,y
	int Xstrt;
	int Ystrt;
	int W;	//width
	int L;  //Height
	tower towers[4]; //4 towers for 4 regions
};

extern float c1, c2, c3; // "extern" variable is used to prevent overloading and to make these variables compiled in C-mode not C++-mode
extern int timeStep;

#endif