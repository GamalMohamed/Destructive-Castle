#include "Declarations.h"

//written in a seperate cpp file to avoid unexpected linkage errors

float enemy::priority()
{
	float remaining_time; //remaining time to shoot
	if (timeStep >= T)
	{
		remaining_time = PR - ((timeStep - T) % PR);
		if (remaining_time == 0)
			remaining_time = PR;
		return ((1 / (float)Distance)* P *(1 / (remaining_time + 1)*c1) + H *c2 + Type *c3); //value of priority
	}
	else
		return 0;
}
