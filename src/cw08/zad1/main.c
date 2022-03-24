#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>


int **out = NULL;
int **src = NULL;
int type;
int W, H;

typedef struct th_info {
	int start;
	int end;
} th_info;


void *do_by_thread (void *info)
{
	struct timespec s_time, e_time;
	clock_gettime(CLOCK_REALTIME, &s_time);

	struct th_info *inf = (struct th_info *) info;
	int start = inf -> start;
	int end = inf -> end;
	if (type == 0)
	{
		for (int i = 0; i < H; i++)
			for (int j = 0; j < W; j++)
				if (src[i][j] < end && src[i][j] >= start)
					out[i][j] = 255 - src[i][j];
	}
	else if (type == 1)
	{
		for (int i = 0; i < H; i++)
			for (int j = start; j < end; j++)
				out[i][j] = 255 - src[i][j];
	}

	clock_gettime(CLOCK_REALTIME, &e_time);
	long seconds = e_time.tv_sec - s_time.tv_sec;
    long nanoseconds = e_time.tv_nsec - s_time.tv_nsec;
    //int elapsed = seconds*1e9 + nanoseconds;
    int elapsed = seconds*1e6 + nanoseconds*1e-3;
    pthread_exit (elapsed);
}

void end ()
{
	if (src != NULL && out != NULL)
	{
		for (int i = 0; i < H; i++)
		{
			free (src[i]);
			free (out[i]);
		}
		free (src);
		free (out);
	}
}



int main (int argc, char** argv)
{
	atexit(end);

	if (argc < 5)
	{
		printf("Nieprawidłowa liczba argumentów\n");
		exit (1);
	}
	int n = atoi (argv[1]);
	FILE* fp_src = fopen(argv[3], "r");
	if (fp_src == NULL)
	{
		printf("Plik %s nie został poprawnie odczytany\n", argv[3]);
		exit (1);
	}
	char *buffer = calloc (50, sizeof(char));
	fgets(buffer, 50, fp_src);
	fgets(buffer, 50, fp_src);
	sscanf(buffer, "%d %d", &W, &H);

	src = calloc (H, sizeof(int*));
	for (int i = 0; i < H; i++)
		src[i] = calloc (W, sizeof(int));

	out = calloc (H, sizeof(int*));
	for (int i = 0; i < H; i++)
		out[i] = calloc (W, sizeof(int));

	fgets(buffer, 50, fp_src);
	free (buffer);
	int size = 4 * (W+1);
	buffer = calloc (size, sizeof(char));

	int i = 0, j = 0;
	while (fgets(buffer, size, fp_src) != NULL)
	{
		char* element = strtok (buffer, " \t\n\r");
		while (element != NULL)
		{
			src[i][j++] = atoi (element);
			if (j == W)
			{
				i++;
				j = 0;
			}
			if (i == H)
				break;
			element = strtok (NULL, " \t\n\r");	
		}
		if (i == H)
			break;
	}

	free (buffer);
	fclose (fp_src);

	if (i != H)
	{
		printf("Błąd odczytu obrazu.\n");
		exit (1);
	}

	pthread_t *threads = calloc(n, sizeof(pthread_t));
	int *exit_value = calloc (n, sizeof(int));
	struct th_info **threads_info = calloc (n, sizeof(struct th_info *));

	for (int i = 0; i < n; i++)
		threads_info[i] = calloc(1, sizeof(struct th_info));

	if (strcmp (argv[2], "numbers") == 0)
	{
		type = 0;
		threads_info[0] -> start = 0;
		threads_info[n-1] -> end = 256;
		for (int i = 1; i < n; i++)
		{
			int v = i * 256/n;
			threads_info[i] -> start = v;
			threads_info[i-1] -> end = v;
		}
	}
	else if (strcmp (argv[2], "block") == 0)
	{
		type = 1;
		threads_info[0] -> start = 0;
		threads_info[n-1] -> end = W;
		for (int i = 1; i < n; i++)
		{
			int v = i * W/n;
			threads_info[i] -> start = v;
			threads_info[i-1] -> end = v;
		}
	}
	else
	{
		printf("Nieprawidłowy argument typu podziału wątków\n");
		for (int i = 0; i < n; i++)
			free(threads_info[i]);
		free (threads_info);
		free (threads);
		free (exit_value);
		exit (1);
	}

	struct timespec s_time, e_time;
	clock_gettime(CLOCK_REALTIME, &s_time);

	for (int i = 0; i < n; i++)
		pthread_create(&threads[i], NULL, do_by_thread, (void *) threads_info[i]);

	for (int i = 0; i < n; i++)
		pthread_join(threads[i], &exit_value[i]);

	clock_gettime(CLOCK_REALTIME, &e_time);
	long seconds = e_time.tv_sec - s_time.tv_sec;
    long nanoseconds = e_time.tv_nsec - s_time.tv_nsec;
    //int elapsed = seconds*1e9 + nanoseconds;
    int elapsed = seconds*1e6 + nanoseconds*1e-3;

    printf("Czas działania programu: %d\n", elapsed);
    printf("Czas działania wątków\n");
    for (int i = 0; i < n; i++)
    	printf("%d -> %d\n", i, exit_value[i]);

	for (int i = 0; i < n; i++)
		free(threads_info[i]);
	//free (threads_info);
	free (threads);
	free (exit_value);

	FILE* fp_out = fopen(argv[4], "w");
	if (fp_out == NULL)
	{
		printf("Plik %s nie został poprawnie odczytany\n", argv[4]);
		exit (1);
	}

	fprintf(fp_out, "P2\n");
	fprintf(fp_out, "%d %d\n", W, H);
	fprintf(fp_out, "255\n");

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
			fprintf(fp_out, " %d", out[i][j]);
		fprintf(fp_out, "\n");
	}

	fclose (fp_out);



/*


	FILE* fp = fopen("test.pgm", "w");
	if (fp == NULL)
	{
		printf("Plik %s nie został poprawnie odczytany\n", argv[4]);
		exit (1);
	}

	fprintf(fp, "P2\n");
	fprintf(fp, "%d %d\n", W, H);
	fprintf(fp, "255\n");

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
			fprintf(fp, " %d", src[i][j]);
		fprintf(fp, "\n");
	}

	fclose (fp);

*/

	return 0;
}