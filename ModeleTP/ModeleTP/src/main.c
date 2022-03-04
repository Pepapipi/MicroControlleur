#include "libTPIUT.h"
#include "robot.h"

void init(){
	initCarte();
	configureConsole();
	initRobot();
	//On démarre le bouton BP1
	allumerPeriph(PortB);
	programmerLigne(PortB,25, ENTREE);
	activerFiltrageLigne(PortB,25,30);
	
}

int main (void)
{
	init();
	int tabPosition[] = {10,0,-10,0};
	int i = 0;
	while(true) {
		if(lireLigne(PortB,25)==1)
		{
			positionnerMoteur(BASE,tabPosition[i]);
			i=(i+1)%4;
			
			while (lireLigne(PortB,25));
		}
	}
}
