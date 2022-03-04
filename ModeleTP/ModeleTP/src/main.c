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
	//On démarre le bouton BP2;
	allumerPeriph(PortC);
	programmerLigne(PortC,26,ENTREE);
	activerFiltrageLigne(PortC,26,30);
	//On demarre les dels
	
	programmerLigne(PortC,30,SORTIE0);//Gauche
	programmerLigne(PortB,27,SORTIE0);//Droite
}

int main (void)
{
	init();
	int tabAngle[] = {10,0,-10,0};
	//BASE = 0 , EPAULE = 1
	int indiceMoteur = 0;
	int i = 0;
	while(true) {
		if(indiceMoteur)
		{
			programmerLigne(PortB,27,SORTIE1);
			programmerLigne(PortC,30,SORTIE1);
		}
		else
		{
			programmerLigne(PortB,27,SORTIE0);
			programmerLigne(PortC,30,SORTIE0);
		}
		if(lireLigne(PortB,25))
		{
			positionnerMoteur(indiceMoteur,tabAngle[i]);
			i=(i+1)%4;
			
			while (lireLigne(PortB,25));
		}
		if(lireLigne(PortC,26))
		{
			
			indiceMoteur=(indiceMoteur+1)%2;
			while(lireLigne(PortC,26));
		}
	}
}
