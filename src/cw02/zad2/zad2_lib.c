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

int char_in_line (char ch, char* line)
{
	int i = 0;
	while (i < 260)
	{
		if (line[i] == 0 || line[i] == 10)
			break;
		if (line[i] == ch)
			return 1;
		i++;
	}
	return 0;
}


void grep (char* file_path, char ch)
{
	FILE *fp;
	fp = fopen (file_path, "r");
	if (fp == NULL)
	{
		printf("Plik %s nie został poprawnie odczytany\n", file_path);
		exit (1);
	}
	char *buffer = calloc (260, sizeof(char));		
	while (fgets(buffer, 260, fp) != NULL)
	{
		if (char_in_line (ch, buffer) == 1)
			printf("%s", buffer);
	}
	fclose(fp);
	free(buffer);
}


int main (int argc, char **argv)
{
	if (argc < 3)
	{
		printf("%s\n", "Nie podano wystarczającej liczby argumentów");
		exit (1);
	}
	char ch = argv[1][0];
	char* file_path = calloc (30, sizeof(char));
	strcpy (file_path, argv[2]);

	c_start = times(&start);
	grep (file_path, ch);
	c_end = times(&end);

	save_time ("Czas wykonania zadania 2 za pomocą funkcji bibliotecznych");

	free (file_path);

	return 0;
}