#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

void filter_numbers (char* file_path)
{
	int data;
	data = open (file_path, O_RDONLY);
	if (data < 0)
	{
		printf("Plik %s nie został poprawnie odczytany\n", file_path);
		exit (1);
	}
	int even = 0;
	int doz, sqr;
	doz = open ("b.txt", O_WRONLY | O_CREAT);
	sqr = open ("c.txt", O_WRONLY | O_CREAT);
	char *buffer = calloc (22, sizeof(char));
	char c;
	int t = 0;		
	while (read (data, &c, 1) == 1)
	{
		buffer[t++] = c;
		if (c==10 || c==0)
		{
			buffer[t] = 0;
			t = 0;
			long long int num;
			sscanf (buffer, "%lld", &num);
			int d = abs(num) % 100;
			d = d / 10;
			int s = sqrt(num);
			int l = strlen (buffer);

			if (num % 2 == 0)
				even++;
			if (d == 0 || d == 7)
				write (doz, buffer, l);
			if (s*s == num)
				write (sqr, buffer, l);
		}
	}
	close (data);
	close (doz);
	close (sqr);
	free (buffer);

	char* res = calloc (50, sizeof(char));
	sprintf (res, "Liczb parzystych jest %d\n", even);
	int ev = open ("a.txt", O_WRONLY | O_CREAT);
	write (ev, res, strlen(res));
	close (ev);
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

	save_time ("Czas wykonania zadania 3 za pomocą funkcji systemowych");

	return 0;
}