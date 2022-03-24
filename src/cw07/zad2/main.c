#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "data.h"


void end() 
{
	shm_unlink("bake");
	shm_unlink("table");
	sem_unlink("open_bake");
	sem_unlink("pizza_bake");
	sem_unlink("open_table");
	sem_unlink("pizza_table");
	sem_unlink("supplier");
}

int main (int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Nieprawidłowe argumenty wywołania\n");
		exit (1);
	}

	atexit(end);

	int bake_ID = shm_open ("bake", O_RDWR | O_CREAT | O_EXCL, 0666);
	if (bake_ID == -1 || ftruncate(bake_ID, sizeof(struct Bake)) == -1)
	{
		perror ("Nie udało się stworzyć pamięci wspólnej pieca");
		exit (1);
	}


	int table_ID = shm_open ("table", O_RDWR | O_CREAT | O_EXCL, 0666);
	if (table_ID == -1 || ftruncate(table_ID, sizeof(struct Table)) == -1)
	{
		perror ("Nie udało się stworzyć pamięci wspólnej stołu");
		exit (1);
	}


	sem_t* s1 = sem_open("open_bake", O_WRONLY | O_CREAT | O_EXCL, 0666, 1);
	sem_t* s2 = sem_open("pizza_bake", O_WRONLY | O_CREAT | O_EXCL, 0666, 5);
	sem_t* s3 = sem_open("open_table", O_WRONLY | O_CREAT | O_EXCL, 0666, 1);
	sem_t* s4 = sem_open("pizza_table", O_WRONLY | O_CREAT | O_EXCL, 0666, 5);
	sem_t* s5 = sem_open("supplier", O_WRONLY | O_CREAT | O_EXCL, 0666, 0);

	int n, m;
	n = atoi(argv[1]);
	m = atoi(argv[2]);
	int *pids = calloc (m+n, sizeof(int));
	int k = 0;
	for (int i = 0; i < n; i++)
	{
		pid_t child = vfork ();
		if (child == 0)
		{
			execlp ("./cook", "./cook", NULL);
			printf("exec cook\n");
			return 0;
		}
		pids[k++] = child;
		//if (i%2 == 0)
		//	sleep (2);
	}

	for (int i = 0; i < m; i++)
	{
		pid_t child = vfork ();
		if (child == 0)
		{
			execlp ("./supplier", "./supplier", NULL);
			printf("exec supplier\n");
			return 0;
		}
		pids[k++] = child;
		//if (i%2 == 0)
		//	sleep (2);
	}

	while(wait(NULL) != -1);


	//sleep (50);
	//for (int i = 0; i < m+n; i++)
	//	kill (pids[i], SIGABRT);

	sem_close(s1);
	sem_close(s2);
	sem_close(s3);
	sem_close(s4);
	sem_close(s5);

	free (pids);

	return 0;
}