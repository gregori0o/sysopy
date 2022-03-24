#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

void filter_numbers (char* file_path)
{
	FILE *data;
	data = fopen (file_path, "r");
	if (data == NULL)
	{
		printf("Plik %s nie został poprawnie odczytany\n", file_path);
		exit (1);
	}
	int even = 0;
	FILE *doz;
	doz = fopen ("b.txt", "w");
	FILE *sqr;
	sqr = fopen ("c.txt", "w");
	char *buffer = calloc (22, sizeof(char));		
	while (fgets(buffer, 22, data) != NULL)
	{
		long long int num;
		sscanf (buffer, "%lld", &num);
		int d = abs(num) % 100;
		d = d / 10;
		int s = sqrt (num);

		if (num % 2 == 0)
			even++;
		if (d == 0 || d == 7)
			fputs (buffer, doz);
		if (s*s == num)
			fputs (buffer, sqr);
	}
	fclose (data);
	fclose (doz);
	fclose (sqr);
	free (buffer);

	char* res = calloc (50, sizeof(char));
	sprintf (res, "Liczb parzystych jest %d\n", even);
	FILE *ev;
	ev = fopen ("a.txt", "w");
	fputs (res, ev);
	fclose (ev);
	free (res);
}



int main (int argc, char **argv)
{
	char* file_path = calloc (30, sizeof(char));
	if (argc < 2)
		file_path = "dane.txt";
	else
		file_path = strcpy (file_path, argv[1]);

	c_start = times(&start);
	filter_numbers (file_path);
	c_end = times(&end);

	save_time ("Czas wykonania zadania 3 za pomocą funkcji bibliotecznych");

	return 0;
}