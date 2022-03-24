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

void make_narrow (char* src_file_path, char* res_file_path)
{
	FILE *src;
	src = fopen (src_file_path, "r");
	if (src == NULL)
	{
		printf("Plik %s nie został poprawnie odczytany\n", src_file_path);
		exit (1);
	}
	FILE *res;
	res = fopen (res_file_path, "w");
	if (res == NULL)
	{
		fclose (src);
		printf("Plik %s nie został poprawnie odczytany\n", res_file_path);
		exit (1);
	}
	char* buffer = calloc(260, sizeof(char));
	char* res_line = calloc(51, sizeof(char));
	while (fgets(buffer, 260, src) != NULL)
	{
		int n = strlen (buffer);
		int i = 0;
		while (n > 50)
		{
			strncpy (res_line, buffer+i, 50);
			res_line[50] = 10;
			fputs(res_line, res);
			n -= 50;
			i += 50;
		}
		strcpy (res_line, buffer+i);
		fputs(res_line, res);
	}
	fclose (src);
	fclose (res);
	free (buffer);
	free (res_line);
}


int main (int argc, char **argv)
{
	if (argc < 3)
	{
		printf("%s\n", "Nie podano wystarczającej liczby argumentów");
		exit (1);
	}

	c_start = times(&start);
	make_narrow (argv[1], argv[2]);
	c_end = times(&end);

	save_time ("Czas wykonania zadania 5 za pomocą funkcji bibliotecznych");

	return 0;
}