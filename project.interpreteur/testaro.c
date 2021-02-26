#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/time.h>

#include <signal.h>
#include <string.h>
#include <sys/time.h>


#include "testaroSub.h"

static char* fichier;



void main(int argc, char* argv[]){

	
	int status;

	for (int i=1; i<argc; i++){
		if (fork()==0){

			//Erreur fichier inexistant
			if (testFile(argv[i])){
				exit(1);
			}
			//On commence a interpreter le i-eme fichier 
			else{
				fichier = argv[i]; 
				int code = analyzer(argv[i]);
				if (code != 0){
					exit(i);
				}
			}
			exit(0);
		}
		wait(&status);
		if (WIFEXITED(status) && WEXITSTATUS(status)){
			printf("%d\n",status );
			exit(WEXITSTATUS(status));
		}

	}
	exit(0);

}