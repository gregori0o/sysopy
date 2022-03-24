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
	key_t key = ftok (filepath, BAKE);
	int bake_ID = shmget (key, 0, 0);
	if (bake_ID == -1)
	{
		perror ("Nie udało się stworzyć pamięci wspólnej pieca");
		exit (1);
	}
	struct Bake* bake = shmat(bake_ID, NULL, 0);

	key = ftok (filepath, TABLE);
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

	srand(time(NULL));
	int start = time(NULL);
	int pid = getpid();

	struct sembuf bake_in;
	bake_in.sem_num = 1;
	bake_in.sem_op = -1;
	bake_in.sem_flg = SEM_UNDO;

	struct sembuf pizza_in;
	pizza_in.sem_num = 2;
	pizza_in.sem_op = -1;
	pizza_in.sem_flg = SEM_UNDO;

	struct sembuf pizza_out;
	pizza_out.sem_num = 2;
	pizza_out.sem_op = 1;
	pizza_out.sem_flg = SEM_UNDO;

	struct sembuf bake_out;
	bake_out.sem_num = 1;
	bake_out.sem_op = 1;
	bake_out.sem_flg = SEM_UNDO;



	struct sembuf table_in;
	table_in.sem_num = 3;
	table_in.sem_op = -1;
	table_in.sem_flg = SEM_UNDO;

	struct sembuf pizzat_in;
	pizzat_in.sem_num = 4;
	pizzat_in.sem_op = -1;
	pizzat_in.sem_flg = SEM_UNDO;

	struct sembuf table_out;
	table_out.sem_num = 3;
	table_out.sem_op = 1;
	table_out.sem_flg = SEM_UNDO;

	struct sembuf table_sup;
	table_sup.sem_num = 5;
	table_sup.sem_op = 1;
	table_sup.sem_flg = SEM_UNDO;


	while (1)
	{
		int t = rand() % PIZZA_TYPE;
		int ind, m, k;
		sleep (1);
		printf("%d %ld Przygotowuje pizze: %d.\n", pid, time(NULL)-start, t);
		sleep (1);

		semop(sem_ID, &pizza_in, 1);
		semop(sem_ID, &bake_in, 1);
		ind = bake -> index;
		bake -> pizzas[ind].type = t;
		bake -> busy ++;
		m = bake -> busy;
		bake -> index ++;
		bake -> index %= SIZE;

		semop(sem_ID, &bake_out, 1);
		printf("%d %ld Dodałem pizze: %d. Liczba pizz w piecu: %d.\n", pid, time(NULL)-start, t, m);
		sleep (4);

		semop(sem_ID, &bake_in, 1);
		t = bake -> pizzas[ind].type;
		bake -> busy --;
		m = bake -> busy;
		semop(sem_ID, &bake_out, 1);
		semop(sem_ID, &pizza_out, 1);

		semop(sem_ID, &pizzat_in, 1);
		semop(sem_ID, &table_in, 1);
		table -> pizzas[table -> index].type = t;
		table -> busy ++;
		k = table -> busy;
		table -> index ++;
		table -> index %= SIZE;
		semop(sem_ID, &table_out, 1);
		semop(sem_ID, &table_sup, 1);
		printf("%d %ld Wyjmuję pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole %d.\n", pid, time(NULL)-start, t, m, k);

		sleep (1);
	}

	return 0;
}