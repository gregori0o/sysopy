#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "data.h"

int main ()
{
	char* filepath = getenv("HOME");

	key_t key = ftok (filepath, TABLE);
	int table_ID = shmget (key, 0, 0);
	if (table_ID == -1)
	{
		perror ("Nie udało się stworzyć pamięci wspólnej stołu");
		exit (1);
	}
	struct Table* table = shmat(table_ID, NULL, 0);

	key = ftok (filepath, SEM);
	int sem_ID = semget (key, 0, 0);
	if (sem_ID == -1)
	{
		perror ("Nie udało się stworzyć semaforów");
		exit (1);
	}

	int start = time(NULL);
	int pid = getpid();

	struct sembuf table_in;
	table_in.sem_num = 3;
	table_in.sem_op = -1;
	table_in.sem_flg = SEM_UNDO;

	struct sembuf pizzat_out;
	pizzat_out.sem_num = 4;
	pizzat_out.sem_op = 1;
	pizzat_out.sem_flg = SEM_UNDO;

	struct sembuf table_out;
	table_out.sem_num = 3;
	table_out.sem_op = 1;
	table_out.sem_flg = SEM_UNDO;

	struct sembuf table_sup;
	table_sup.sem_num = 5;
	table_sup.sem_op = -1;
	table_sup.sem_flg = SEM_UNDO;

	while (1)
	{
		int t, k;

		semop(sem_ID, &table_sup, 1);

		semop(sem_ID, &table_in, 1);
		t = table -> pizzas[table -> supp].type;
		table -> busy --;
		k = table -> busy;
		table -> supp ++;
		table -> supp %= SIZE;
		semop(sem_ID, &table_out, 1);
		semop(sem_ID, &pizzat_out, 1);
		printf("%d %ld Pobieram pizze: %d. Liczba pizz na stole %d.\n", pid, time(NULL)-start, t, k);

		sleep (5);

		printf("%d %ld Dostarczam pizze: %d.\n", pid, time(NULL)-start, t);

		sleep (4);
	}

	return 0;
}