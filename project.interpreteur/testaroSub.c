#include <signal.h>
#include <string.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "testaroFonciton.h"




static char* ligne;
int marqueurStdin = 0;
char* bufferStdin;
int marqueurStdout = 0;
char* bufferStdout; //askip on paut pas faire de malloc hors d'une fonction :|

int testFile (const char* filePath){
	FILE* file = fopen(filePath,"r");
	if (file==NULL){
		return 1;
	}
	else{
		return 0;
	}
	
}



int execLine(char* line){
	
 	
	switch (line[0]) {
		case '<':
			//besoin de mettre en mémoire en tant qu'argument
			switch (marqueurStdin)
			{
			case 0 :
				bufferStdin = (char*)malloc(500*sizeof(char));
				bufferStdin = strcpy(bufferStdin, line);
				marqueurStdin++;
				break;
			
			default:
				strcat(bufferStdin, line);
				break;
			}
			break;
		case '>' :
			//besoin de mettre en mémoire pour comparaison
			switch (marqueurStdout)
			{
			case 0 :
				bufferStdout = (char*)malloc(500*sizeof(char));
				bufferStdout = strcpy(bufferStdout,line);
				marqueurStdout++;
				break;
			
			default:
				strcat(bufferStdout, line);
				break;
			}
			break;
		case '$' :
		printf("Phase execution... de %s\n",line);
			//besoin d'exécuter la ligne avec possiblement des arguments supplémentaires
			if (marqueurStdin!=0 && marqueurStdout!=0) //Si les deux buffers ont été atteints
			{
				//comparaison entre exec(line+bufferstin) et bufferStdout
				//TODO
				int result = execCompaStdin(line, bufferStdout, bufferStdin);

				//Remise à 0 des paramètres
				marqueurStdin = 0;
				free(bufferStdin);
				marqueurStdout = 0;
				free(bufferStdout);

			} 
			else if (marqueurStdout!=0) //Si uniquement le buffer de la sortie standard a été atteint 

			{
				//comparaison entre exec(line) et bufferStdout
				//TODO
				int result = execCompa(line, bufferStdout);
				//Remise à 0 des paramètres
				marqueurStdout = 0;
				free(bufferStdout);

				if (result!=0){return(result);}

			}
			else if (marqueurStdin)
			{
				//Execution de line+bufferStdin
				//TODO
				int retour = executionToStdin(line, bufferStdin);
				//Remise à 0 des paramètres
				marqueurStdin = 0;
				free(bufferStdin);
				if (retour!=0){return retour;}
				
			}
			else
			{
				//Execution de line
				//TODO
				int result = execution(line);
				if (result!=0){return(result);}
				
			}			
			break;
		default : //Si c'est une autre début, il y a une erreur de syntaxe dans le fichier

			printf("La ligne > %s < n'a pas été reconnu : code d'erreur 1 \n", line);
			return(1); //on renvoie le code d'erreur 1
			break;
	}
	return 0;
}



int analyzer (const char* filePath){


	FILE* file =fopen(filePath,"r");
	char* line = (char*)malloc(sizeof(char)*500);
// --> Ligne de code actuelle



	char* lastLine = (char*)malloc(sizeof(char)*500);
// --> Cette variable sert à concatener deux lignes de code si séparé par '/'

	int increm;
//--> Variable incrémentation pour chaque ligne

	int testVide;
//--> Variable vérifiant le caractère non-vide (non-uniquement composée d'espace)

	int lineJump =0;
//--> Variable notant si un saut de ligne est réalisé 

	int goodExec;
//--> Varriable notant la bonne exécution d'une ligne, ou sinon, son code d'erreur

//On analyse ligne par ligne jusqu'à la fin du fichier
	while ((fgets(line,500,file))!= NULL){
		increm=0;

		testVide = 0;

		//On compte le nombre de caractère
		while(line[increm] != '\n'){
			if (line[increm]!=' '){testVide++;}
			increm++;
		}

		if(increm!=0 && testVide!=0){
			//On enlève les commentaires
			if (line[0]!='#'){

				//On regarde s'il y a un '\' en fin de ligne :
				//Si oui, il y a une concaténation pour la prochaine ligne
				if (line[increm-1]=='\\'){

					lastLine=(char*)realloc(lastLine,(sizeof(lastLine)+sizeof(char)*(increm+5)));

					// --> On rallonge l'allocation pour bien concatener la ligne
					lastLine = strncat(lastLine,line, increm-1);
					lineJump++;
				}
				else if (lineJump!=0) //Si on arrive à la fin d'une suite de lignes concaténées
				{
					lastLine=(char*)realloc(lastLine,(sizeof(lastLine)+sizeof(char)*(increm+5)));
					// --> On rallonge l'allocation pour bien concatener la ligne
					lastLine = strncat(lastLine,line, increm);

					goodExec = execLine(lastLine); //On execute la commande


					if (goodExec!=0){ //Si on trouve un code d'erreur à l'éxecution
						free(lastLine); //On libère les espaces mémoires
						free(line);
						fclose(file); //On ferme le fichier
						return goodExec; //On arrête la fonction;
					}
					
					//Et on repart de zéro pour la lecture de ligne
					free(lastLine);
					lastLine = (char*)malloc(sizeof(char*)*500);
					lineJump = 0;
				}
				else{ //Si il s'agit d'une ligne simple,
					printf("%s\n",line);
					goodExec = execLine(line);
					if (goodExec!=0){ //Si on trouve un code d'erreur à l'éxecution
						free(lastLine); //On libère les espaces mémoires
						free(line);
						fclose(file); //On ferme le fichier
						return goodExec; //On arrête la fonction
					}	
				}

			}
			/*Toute l'analyse du programme (l'analyse des &, ...)
			--> idée : un programme pour chaque commande à implémenter
			--> Ne pas oublier les codes d'erreurs 
			*/
		}
		
	}
	free(lastLine); lastLine = NULL;
	free(line); lastLine = NULL;

	
	fclose(file);
	return 0;

}