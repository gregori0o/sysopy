#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>

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
	FILE *first;
	FILE *second;
	first = fopen (file_first_path, "r");
	if (first == NULL)
	{
		printf("Plik %s nie został poprawnie odczytany\n", file_first_path);
		exit (1);
	}
	second = fopen (file_second_path, "r");
	if (second==NULL)
	{
		printf("Plik %s nie został poprawnie odczytany\n", file_second_path);
		fclose (first);
		exit (1);
	}
	char *buffer = calloc (260, sizeof(char));		
	int i=1, j=1;
	while (i>0 || j>0)
	{
		if (i>0)
		{
			if (fgets(buffer, 260, first) != NULL)
				printf("%s", buffer);
			else
				i = 0;
		}
		if (j>0)
		{
			if (fgets(buffer, 260, second) != NULL)
				printf("%s", buffer);
			else
				j = 0;
		}
	}
	fclose(first);
	fclose(second);
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

	save_time ("Czas wykonania zadania 1 za pomocą funkcji bibliotecznych");

	free (file_first_path);
	free (file_second_path);

	return 0;
}