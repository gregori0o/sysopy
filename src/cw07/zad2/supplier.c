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
	int table_ID = shm_open ("table", O_RDWR, 0);
	if (table_ID == -1 || ftruncate(table_ID, sizeof(struct Table)) == -1)
	{
		perror ("Nie udało się stworzyć pamięci wspólnej stołu");
		exit (1);
	}
	struct Table* table = mmap(NULL, sizeof(struct Table), PROT_READ | PROT_WRITE, MAP_SHARED, table_ID, 0);

	int start = time(NULL);
	int pid = getpid();

	sem_t* s3 = sem_open("open_table", O_WRONLY);
	sem_t* s4 = sem_open("pizza_table", O_WRONLY);
	sem_t* s5 = sem_open("supplier", O_WRONLY);

	while (1)
	{
		int t, k;

		sem_wait (s5);

		sem_wait (s3);
		t = table -> pizzas[table -> supp].type;
		table -> busy --;
		k = table -> busy;
		table -> supp ++;
		table -> supp %= SIZE;
		sem_post (s3);
		sem_post (s4);

		printf("%d %ld Pobieram pizze: %d. Liczba pizz na stole %d.\n", pid, time(NULL)-start, t, k);

		sleep (5);

		printf("%d %ld Dostarczam pizze: %d.\n", pid, time(NULL)-start, t);

		sleep (4);
	}

	munmap (table, sizeof(struct Table));

	sem_close(s3);
	sem_close(s4);
	sem_close(s5);

	return 0;
}