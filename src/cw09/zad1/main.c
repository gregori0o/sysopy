#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>


int num_elf;
int ready_elf;
int *elfes;
pthread_mutex_t elf = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t elf_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t elf_cond2 = PTHREAD_COND_INITIALIZER;


int num_reindeer;
int ready_reindeer;
pthread_mutex_t reindeer = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reindeer_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t santa_clous = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t santa_clous_cond = PTHREAD_COND_INITIALIZER;


void end ()
{
	pthread_mutex_destroy (&elf);
	pthread_mutex_destroy (&reindeer);
	pthread_mutex_destroy (&santa_clous);
	pthread_cond_destroy (&elf_cond);
	pthread_cond_destroy (&elf_cond2);
	pthread_cond_destroy (&reindeer_cond);
	pthread_cond_destroy (&santa_clous_cond);

	printf("Koniec!\n");
}


void *reindeer_f (void *info)
{
	int ID = pthread_self();
	while (1)
	{
		printf("Renifer: leci na wakacje %d\n", ID);
		sleep (5 + (rand()%6));
		pthread_mutex_lock(&reindeer);
		int k = ++num_reindeer;
		printf("Renifer: czeka %d reniferów na Mikołaja, %d\n", k, ID);
		if (k == 9)
		{
			pthread_mutex_lock(&santa_clous);
			printf("Renifer: wybudzam Mikołaja, %d\n", ID);
			ready_reindeer = 1;
			pthread_cond_broadcast(&santa_clous_cond);
			pthread_cond_broadcast(&reindeer_cond);
			pthread_mutex_unlock(&santa_clous);
		}
		else
		{
			pthread_cond_wait(&reindeer_cond, &reindeer);
		}
		pthread_mutex_unlock(&reindeer);
		sleep (3);
	}
}

void *santa_claus_f (void *info)
{
	int i = 3;
	pthread_mutex_lock(&santa_clous);
	while (i)
	{
		printf("Mikołaj: zasypiam\n");
		pthread_cond_wait(&santa_clous_cond, &santa_clous);
		printf("Mikołaj: budzę się\n");
		if (ready_reindeer == 1)
		{
			printf("Mikołaj: dostarczam zabawki\n");
			sleep (3);
			num_reindeer = 0;
			ready_reindeer = 0;
			i--;
		}
		if (ready_elf == 1)
		{
			printf("Mikołaj: rozwiązuje problemy elfów %d %d %d\n", elfes[0], elfes[1], elfes[2]);
			pthread_mutex_lock(&elf);
			pthread_cond_broadcast(&elf_cond);
			pthread_mutex_unlock(&elf);
			sleep (2);
			pthread_mutex_lock(&elf);
			num_elf = 0;
			ready_elf = 0;
			pthread_cond_broadcast(&elf_cond2);
			pthread_mutex_unlock(&elf);
		}
	}
	pthread_mutex_unlock(&santa_clous);
	pthread_exit (0);
}


void *elf_f (void *info)
{
	int ID = pthread_self();
	while (1)
	{
		printf("Elf: pracuje %d\n", ID);
		sleep (2 + (rand()%4));
		pthread_mutex_lock(&elf);
		if (num_elf == 3)
			printf("Elf: czeka na powrót elfów, %d\n", ID);
		while (num_elf == 3)
			pthread_cond_wait(&elf_cond2, &elf);
		elfes[num_elf++] = ID;
		printf("Elf: czeka %d elfów na Mikołaja, %d\n", num_elf, ID);
		if (num_elf == 3)
		{
			pthread_mutex_lock(&santa_clous);
			printf("Elf: wybudzam mikołaja, %d\n", ID);
			pthread_cond_broadcast(&santa_clous_cond);
			pthread_mutex_unlock(&santa_clous);
			ready_elf = 1;
		}
		
		pthread_cond_wait(&elf_cond, &elf);

		pthread_mutex_unlock(&elf);
		printf("Elf: Mikołaj rozwiązuje problem, %d\n", ID);
		sleep (2);
	}
}


int main (int argc, char** argv)
{
	atexit(end);

	srand(time(NULL));

	num_reindeer = 0;
	num_elf = 0;
	ready_elf = 0;
	ready_reindeer = 0;
	elfes = calloc (3, sizeof(int));

	pthread_t santa;
	pthread_t *threads = calloc(19, sizeof(pthread_t));
	pthread_create(&santa, NULL, santa_claus_f, NULL);

	for (int i = 0; i < 9; i++)
	{
		pthread_create(&threads[i], NULL, reindeer_f, NULL);
		pthread_detach(threads[i]);
	}

	for (int i = 0; i < 10; i++)
	{
		pthread_create(&threads[i+9], NULL, elf_f, NULL);
		pthread_detach(threads[i]);
	}

	pthread_join(santa, NULL);

	for (int i = 0; i < 19; i++)
		pthread_cancel(threads[i]);

	free (elfes);
	free (threads);

	return 0;
}