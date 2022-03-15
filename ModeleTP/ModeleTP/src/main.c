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
	//Timer
	allumerPeriph(Timer2);
	timerModeDelai(Timer2, HDIV2, 42000000U, NON_REPETITIF, INC);
	allumerPeriph(Timer0);
	timerModeMesure(Timer0,HDIV2,LIMITE_TIMER_MAX, DE_1_A_0);
	programmerLigne(PortB,25,FONCTIONB);
}
void positionnerMoteurLent(int moteur,int angleActuel, int angleSuivant)
{
	lancerTimer(Timer1);
	//printf("L'angle actuel : %d, l'angle suivant: %d \n", angleActuel, angleSuivant);
	
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

int tabAngle[] = {0,30,0,-30};
//BASE = 0 , EPAULE = 1
int indiceMoteur = 0;
int tabMoteur[] = {0,0,0,0,0};
void retourAZero()
{
	lancerTimer(Timer2);
	for (int i=0;i<=4;i++)
	{
		ecrireLigne(PortB,27,1);
		ecrireLigne(PortA,21,0);
		ecrireLigne(PortC,30,0);
		positionnerMoteurLent(i,tabAngle[tabMoteur[i]],0);
		tabMoteur[i]=0;
	}
	arreterTimer(Timer2);
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
		lancerTimer(Timer0);
	
		if(lireLigne(PortB,25)&&testerEtatTimer(Timer0,MESURE))
		{
			printf("Duree de pression %d \n",lireMesureTimer(Timer0));
			if(lireMesureTimer(Timer0)<200000000)
			{
				printf("Mouvement moteur \n");
				positionnerMoteurLent(indiceMoteur,tabAngle[tabMoteur[indiceMoteur]],tabAngle[(tabMoteur[indiceMoteur]+1)%4]);
				tabMoteur[indiceMoteur]=(tabMoteur[indiceMoteur]+1)%4;
			}
			else
			{
				printf("Mouvement zero \n");
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
