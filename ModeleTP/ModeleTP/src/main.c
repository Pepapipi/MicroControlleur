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
	programmerLigne(PortA,21,SORTIE1);//Milieu
}

int tabAngle[] = {10,0,-10,0};
//BASE = 0 , EPAULE = 1
int indiceMoteur = 0;
int tabMoteur[] = {0,0,0,0,0};
void retourAZero()
{
	for (int i=0;i<=4;i++)
	{
		ecrireLigne(PortB,27,1);
		ecrireLigne(PortA,21,0);
		ecrireLigne(PortC,30,0);
		positionnerMoteur(i,0);
		tabMoteur[i]=0;
	}
}

int main (void)
{
	init();
	

	while(true) {
		if(indiceMoteur !=5){
		ecrireLigne(PortB,27,indiceMoteur&1);
		ecrireLigne(PortA,21,~indiceMoteur>>1&1);
		ecrireLigne(PortC,30,~indiceMoteur>>2&1);
		}
		else
		{
			ecrireLigne(PortB,27,1);
			ecrireLigne(PortA,21,0);
			ecrireLigne(PortC,30,0);
		}

		if(lireLigne(PortB,25))
		{
			if(indiceMoteur!=5)
			{
				positionnerMoteur(indiceMoteur,tabAngle[tabMoteur[indiceMoteur]]);
				tabMoteur[indiceMoteur]=(tabMoteur[indiceMoteur]+1)%4;
			}
			else
			{
				retourAZero();
			}
			while (lireLigne(PortB,25));
		}
		if(lireLigne(PortC,26))
		{
	
			indiceMoteur=(indiceMoteur+1)%6;
			while(lireLigne(PortC,26));
		}
	}
	
}
