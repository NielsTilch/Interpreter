#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>

#include <signal.h>
#include <string.h>
#include <sys/time.h>

void timer_handler (int signum)
{
 printf ("Signal Timout, la commande prend trop de temps\n");
 exit(3);
}


int execution (char* line){

	struct sigaction sa;
 	struct itimerval timer;


	memset (&sa, 0, sizeof (sa));
 	sa.sa_handler = &timer_handler;
 	sigaction (SIGVTALRM, &sa, NULL);

 	/* Configuration du Timer 5 sec... */
 	timer.it_value.tv_sec = 0;
 	timer.it_value.tv_usec = 5000000;
 	/*L'intervalle de 5 sec */
 	timer.it_interval.tv_sec = 0;
 	timer.it_interval.tv_usec = 5000000;

	int status;

	//On retire le $ en début de ligne !
	line ++;

	if (fork()==0){
		setitimer (ITIMER_VIRTUAL, &timer, NULL);
		//fprintf(stderr,"On execute le fichier");
		int returnLine = execl("/bin/sh","sh","-c",line, NULL);
		if (returnLine==-1){return -returnLine;} //si l'execution de la commande n'a pas été réussie, on renvoie 1
		exit(0);
	}
	wait(&status);

	if (WEXITSTATUS(status)){
		fprintf(stderr, "Erreur lors de l'exécution de la commande > %s < \n", line);
		if (status == 3){
			return 3;
		}
		return 4;
	}
	return 0;
}




int execCompa(char* lineCommand, char* lineSortie){

	struct sigaction sa;
 	struct itimerval timer;


	memset (&sa, 0, sizeof (sa));
 	sa.sa_handler = &timer_handler;
 	sigaction (SIGVTALRM, &sa, NULL);

 	/* Configuration du Timer 5 sec... */
 	timer.it_value.tv_sec = 0;
 	timer.it_value.tv_usec = 5000000;
 	/*L'intervalle de 5 sec */
 	timer.it_interval.tv_sec = 0;
 	timer.it_interval.tv_usec = 5000000;


	char* word;
	char* list[100];
	int currentSpace = 0;
	int status;

	word = strtok(lineCommand," "); //enlève le premier symbole de la ligne d'exécution ($)
	word = strtok(NULL," ");//découpe la line en token séparé d'un espace

	while(word != NULL){
		list[currentSpace] = word;
		word = strtok(NULL," ");
		currentSpace++;
	}
	//Pour ranger dans un fichier le retour d'éxecution de la commande
	list[currentSpace]=">>";
	list[currentSpace+1] = "temp.txt";

	//Pour finir l'éxecution
	list[currentSpace+2] = NULL;

	//Exécution du la ligne
	if (fork()==0){
		setitimer (ITIMER_VIRTUAL, &timer, NULL);
		execvp(list[0],list);
	}
	wait(&status);
	if (WEXITSTATUS(status)){
		if (status == 3){
			return 3;
		}
		fprintf(stderr, "Erreur lors de l'exécution de la commande > %s < \n", lineCommand);
		return 1;
	}
	
	//Récupération de la line de retour
	FILE* file = fopen("temp.txt", "r");
	char* lineRetour = (char*)malloc(sizeof(char)*500);
	fgets(lineRetour, 500, file);
	fclose(file);


	int comp = strcmp(lineRetour, lineSortie); 
	free(lineRetour);
	if (comp!=0){return 2;} //Si le code retourné est faux, on renvoit 2, le code d'erreur associé.
	
	return 0;
}


int executionToStdin(char* line, char* stdIn){

	struct sigaction sa;
 	struct itimerval timer;

	memset (&sa, 0, sizeof (sa));
 	sa.sa_handler = &timer_handler;
 	sigaction (SIGVTALRM, &sa, NULL);

 	/* Configuration du Timer 5 sec... */
 	timer.it_value.tv_sec = 0;
 	timer.it_value.tv_usec = 5000000;
 	/*L'intervalle de 5 sec */
 	timer.it_interval.tv_sec = 0;
 	timer.it_interval.tv_usec = 5000000;


	char* word;
	char* list[100];
	char* echoComm[100];
	int status;

	int currentSpace = 0;
	int currentSpaceecho = 1;


	int fd[2];
	pipe(fd);

	//--> On prepare l'entree  

	word = strtok(stdIn," ");

	echoComm[0]= "echo";
	echoComm[1] = stdIn;
	echoComm[2]= NULL;
	
	if (fork()==0){
		setitimer (ITIMER_VIRTUAL, &timer, NULL);
		dup2(fd[1],1);
		close(fd[1]);
		close(fd[0]);
		execvp(echoComm[0],echoComm);
	}
	wait(&status);
	if (WEXITSTATUS(status)){
		if (status == 3){
			return 3;
		}
		fprintf(stderr, "Erreur lors de l'exécution de la commande > %s < \n", line);
		return 40 + status;
	}

	word = strtok(line," "); //enlève le premier symbole de la ligne d'exécution ($)
	word = strtok(NULL," ");

	while(word != NULL){
		list[currentSpace] = word;
		word = strtok(NULL," ");
		currentSpace++;
	}

	//--> L'execution totale
	if (fork()==0){
		dup2(fd[0],0);
		close(fd[1]);
		close(fd[0]);
		execvp(list[0],list);
	}
	wait(&status);
	if (WEXITSTATUS(status)){
		return 40+status;
	}
	return 0;
}






int execCompaStdin(char *line, char* lineSortie, char* stdIn){

	struct sigaction sa;
 	struct itimerval timer;

	memset (&sa, 0, sizeof (sa));
 	sa.sa_handler = &timer_handler;
 	sigaction (SIGVTALRM, &sa, NULL);

 	/* Configuration du Timer 5 sec... */
 	timer.it_value.tv_sec = 0;
 	timer.it_value.tv_usec = 5000000;
 	/*L'intervalle de 5 sec */
 	timer.it_interval.tv_sec = 0;
 	timer.it_interval.tv_usec = 5000000;

	char* word;
	char* list[100];
	char* echoComm[100];
	int status;

	int currentSpace = 0;
	int currentSpaceecho = 1;


	int fd[2];
	pipe(fd);

	//--> On prepare l'entree  

	word = strtok(stdIn," ");

	echoComm[0]= "echo";
	echoComm[1] = stdIn;
	echoComm[2]= NULL;
	
	if (fork()==0){
		setitimer (ITIMER_VIRTUAL, &timer, NULL);
		dup2(fd[1],1);
		close(fd[1]);
		close(fd[0]);
		execvp(echoComm[0],echoComm);
	}
	wait(&status);
	if (WEXITSTATUS(status)){
		if (status == 3){
			return 3;
		}
		return 40+status;
	}

	//Pour ranger dans un fichier le retour d'éxecution de la commande
	list[currentSpace]=">>";
	list[currentSpace+1] = "temp.txt";

	//Pour finir l'éxecution
	list[currentSpace+2] = NULL;
	

	//--> L'execution totale

	word = strtok(line," "); //enlève le premier symbole de la ligne d'exécution ($)
	word = strtok(NULL," ");

	while(word != NULL){
		list[currentSpace] = word;
		word = strtok(NULL," ");
		currentSpace++;
	}
	list[currentSpace]=NULL;
	

	if (fork()==0){
		setitimer (ITIMER_VIRTUAL, &timer, NULL);
		dup2(fd[0],0);
		close(fd[1]);
		close(fd[0]);
		execvp(list[0],list);
	}
	wait(&status);
	if (WEXITSTATUS(status)){
		if (status == 3){
			return 3;
		}
		return 40+status;
	}

	//Récupération de la line de retour
	FILE* file = fopen("temp.txt", "r");
	char* lineRetour = (char*)malloc(sizeof(char)*500);
	fgets(lineRetour, 500, file);
	fclose(file);

	int compa =strcmp(lineRetour, lineSortie); 
	free(lineRetour);
	if (compa !=0){return 2;} //Si le code retourné est faux, on renvoit 2, le code d'erreur associé.
	
	
	return 0;
}