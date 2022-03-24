#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>

#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "data.h"

int main ()
{
	
	int bake_ID = shm_open ("bake", O_RDWR, 0);
	if (bake_ID == -1 || ftruncate(bake_ID, sizeof(struct Bake)) == -1)
	{
		perror ("Nie udało się stworzyć pamięci wspólnej pieca");
		exit (1);
	}
	struct Bake* bake = mmap(NULL, sizeof(struct Bake), PROT_READ | PROT_WRITE, MAP_SHARED, bake_ID, 0);


	int table_ID = shm_open ("table", O_RDWR, 0);
	if (table_ID == -1 || ftruncate(table_ID, sizeof(struct Table)) == -1)
	{
		perror ("Nie udało się stworzyć pamięci wspólnej stołu");
		exit (1);
	}
	struct Table* table = mmap(NULL, sizeof(struct Table), PROT_READ | PROT_WRITE, MAP_SHARED, table_ID, 0);

	
	srand(time(NULL));
	int start = time(NULL);
	int pid = getpid();

	sem_t* s1 = sem_open("open_bake", O_WRONLY);
	sem_t* s2 = sem_open("pizza_bake", O_WRONLY);
	sem_t* s3 = sem_open("open_table", O_WRONLY);
	sem_t* s4 = sem_open("pizza_table", O_WRONLY);
	sem_t* s5 = sem_open("supplier", O_WRONLY);


	while (1)
	{
		int t = rand() % PIZZA_TYPE;
		int ind, m, k;
		sleep (1);
		printf("%d %ld Przygotowuje pizze: %d.\n", pid, time(NULL)-start, t);
		sleep (1);

		sem_wait(s2);
		sem_wait(s1);

		ind = bake -> index;
		bake -> pizzas[ind].type = t;
		bake -> busy ++;
		m = bake -> busy;
		bake -> index ++;
		bake -> index %= SIZE;

		sem_post(s1);
		printf("%d %ld Dodałem pizze: %d. Liczba pizz w piecu: %d.\n", pid, time(NULL)-start, t, m);
		sleep (4);

		sem_wait(s1);
		t = bake -> pizzas[ind].type;
		bake -> busy --;
		m = bake -> busy;
		sem_post(s1);
		sem_post(s2);

		sem_wait(s4);
		sem_wait(s3);
		table -> pizzas[table -> index].type = t;
		table -> busy ++;
		k = table -> busy;
		table -> index ++;
		table -> index %= SIZE;
		sem_post(s3);
		printf("%d %ld Wyjmuję pizze: %d. Liczba pizz w piecu: %d. Liczba pizz na stole %d.\n", pid, time(NULL)-start, t, m, k);
		sem_post(s5);
		
		sleep (1);
	}

	munmap (bake, sizeof(struct Bake));
	munmap (table, sizeof(struct Table));

	sem_close(s1);
	sem_close(s2);
	sem_close(s3);
	sem_close(s4);
	sem_close(s5);

	return 0;
}