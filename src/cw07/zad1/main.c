#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>

#include "data.h"


int table_ID = -1, bake_ID = -1, sem_ID = -1;

void end()
{
	shmctl(table_ID, IPC_RMID, NULL);
	shmctl(bake_ID, IPC_RMID, NULL);
	semctl(sem_ID, 6, IPC_RMID, NULL);
}

int main (int argc, char** argv)
{
	if (argc != 3)
	{
		printf("Nieprawidłowe argumenty wywołania\n");
		exit (1);
	}

	atexit(end);

	char* filepath = getenv("HOME");
	key_t key = ftok (filepath, BAKE);
	bake_ID = shmget (key, sizeof(struct Bake), IPC_CREAT | IPC_EXCL | 0666);
	if (bake_ID == -1)
	{
		perror ("Nie udało się stworzyć pamięci wspólnej pieca");
		exit (1);
	}

	key = ftok (filepath, TABLE);
	table_ID = shmget (key, sizeof(struct Table), IPC_CREAT | IPC_EXCL | 0666);
	if (table_ID == -1)
	{
		perror ("Nie udało się stworzyć pamięci wspólnej stołu");
		exit (1);
	}

	key = ftok (filepath, SEM);
	sem_ID = semget (key, 6, IPC_CREAT | IPC_EXCL | 0666);
	if (sem_ID == -1)
	{
		perror ("Nie udało się stworzyć semaforów");
		exit (1);
	}

	union semun sv;
	sv.val = 1;
	semctl(sem_ID, 1, SETVAL, sv);
	semctl(sem_ID, 3, SETVAL, sv);

	sv.val = 5;
	semctl(sem_ID, 2, SETVAL, sv);
	semctl(sem_ID, 4, SETVAL, sv);

	sv.val = 0;
	semctl(sem_ID, 5, SETVAL, sv);

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


	free (pids);

	return 0;
}