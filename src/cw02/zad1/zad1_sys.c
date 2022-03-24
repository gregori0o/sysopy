#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

static struct tms start, end;
static clock_t c_start, c_end;

void save_time (char *function)
{
	FILE *report;
	report = fopen ("report.txt", "w");
	fprintf (report, "%s\n", function);
	fprintf (report, "Czas rzeczywisty: %ld\n", c_end - c_start);
	fprintf (report, "Czas użytkownika: %ld\n", end.tms_utime - start.tms_utime);
	fprintf (report, "Czas systemowy:   %ld\n", end.tms_stime - start.tms_stime);
	fclose (report);
}

void print_files (char* file_first_path, char* file_second_path)
{
	int first, second;
	first = open (file_first_path, O_RDONLY);
	if (first < 0)
	{
		printf("Plik %s nie został poprawnie odczytany\n", file_first_path);
		exit (1);
	}
	second = open (file_second_path, O_RDONLY);
	if (second < 0)
	{
		printf("Plik %s nie został poprawnie odczytany\n", file_second_path);
		close (first);
		exit (1);
	}
	char *buffer = calloc (260, sizeof(char));
	char c;
	int i=1, j=1;
	while (i>0 || j>0)
	{
		if (i>0)
		{
			int t = 0;
			while (t<260)
			{
				int k = read (first, &c, 1);
				if (k == 0)
				{
					i = 0;
					break;
				}
				buffer[t++] = c;
				if (c == 10 || c == 0)
					break;
			}
			buffer[t] = 0;
			printf("%s", buffer);
		}
		if (j>0)
		{
			int t = 0;
			while (t<260)
			{
				int k = read (second, &c, 1);
				if (k == 0)
				{
					j = 0;
					break;
				}
				buffer[t++] = c;
				if (c == 10 || c == 0)
					break;
			}
			buffer[t] = 0;
			printf("%s", buffer);
		}
	}
	close(first);
	close(second);
	free (buffer);
}


int main (int argc, char **argv)
{
	char* file_first_path = calloc (30, sizeof(char));
	char* file_second_path = calloc (30, sizeof(char));
	if (argc >= 3)
	{
		strcpy (file_first_path, argv[1]);
		strcpy (file_second_path, argv[2]);
	}
	else if (argc == 2)
	{
		strcpy (file_first_path, argv[1]);
		scanf ("%s", file_second_path);
	}
	else
	{
		scanf ("%s", file_first_path);
		scanf ("%s", file_second_path);
	}

	c_start = times(&start);
	print_files (file_first_path, file_second_path);
	c_end = times(&end);

	save_time ("Czas wykonania zadania 1 za pomocą funkcji systemowych");

	free (file_first_path);
	free (file_second_path);

	return 0;
}