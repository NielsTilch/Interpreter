#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void timer_handler (int signum)
{
 static int count = 0;
 printf ("timer expired %d times\n", ++count);
 exit(1);
}

void main ()
{
 execl("/bin/sh","sh","-c","mkdir Fichier test", NULL);
}