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
	
	//Timer
	allumerPeriph(Timer1);
	timerModeDelai(Timer1, HDIV2, 1680000UL, REPETITIF, INC);
}

int tabAngle[] = {30,0,-30,0};
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
void positionnerMoteurLent(int moteur,int angleActuel, int angleSuivant)
{
	lancerTimer(Timer1);
	printf("L'angle actuel : %d, l'angle suivant: %d \n", angleActuel, angleSuivant);
	
	if (angleActuel>=angleSuivant)
	{
		for (int i=angleActuel;i>angleSuivant;i--)
		{
			positionnerMoteur(moteur,i);
			while (!(testerEtatTimer(Timer1,LIMITE)));
		}
	}
	else
	{
		for (int i=angleActuel;i<angleSuivant;i++)
		{
			positionnerMoteur(moteur,i);
			while (!(testerEtatTimer(Timer1,LIMITE)));
		}
	}
	arreterTimer(Timer1);
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
				positionnerMoteurLent(indiceMoteur,tabAngle[tabMoteur[indiceMoteur]],tabAngle[(tabMoteur[indiceMoteur]+1)%4]);
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
