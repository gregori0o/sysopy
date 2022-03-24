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

void make_narrow (char* src_file_path, char* res_file_path)
{
	int src, res;
	src = open (src_file_path, O_RDONLY);
	if (src < 0)
	{
		printf("Plik %s nie został poprawnie odczytany\n", src_file_path);
		exit (1);
	}
	res = open (res_file_path, O_WRONLY | O_CREAT | O_APPEND);
	if (res < 0)
	{
		close (src);
		printf("Plik %s nie został poprawnie odczytany\n", res_file_path);
		exit (1);
	}
	char* buffer = calloc(260, sizeof(char));
	char* res_line = calloc(51, sizeof(char));
	char c;
	int t = 0;
	while (read (src, &c, 1) == 1)
	{
		buffer[t++] = c;
		if (c==10 || c==0)
		{
			buffer[t] = 0;
			t = 0;
			int n = strlen (buffer);
			int i = 0;
			while (n > 50)
			{
				strncpy (res_line, buffer+i, 50);
				res_line[50] = 10;
				write (res, res_line, strlen(res_line));
				n -= 50;
				i += 50;
			}
			strcpy (res_line, buffer+i);
			write (res, res_line, strlen(res_line));
		}
	}
	close (src);
	close (res);
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

	save_time ("Czas wykonania zadania 5 za pomocą funkcji systemowych");

	return 0;
}