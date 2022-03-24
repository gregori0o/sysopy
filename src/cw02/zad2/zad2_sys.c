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

void grep (char* file_path, char ch)
{
	int fp;
	fp = open (file_path, O_RDONLY);
	if (fp < 0)
	{
		printf("Plik %s nie został poprawnie odczytany\n", file_path);
		exit (1);
	}
	char *buffer = calloc (260, sizeof(char));
	char c;
	int flag = 0;
	int t = 0;		
	while (read (fp, &c, 1) == 1)
	{
		buffer[t++] = c;
		if (c == ch)
			flag = 1;
		if (c == 10 || c == 0)
		{
			buffer[t] = 0;
			t = 0;
			if (flag > 0)
				printf("%s", buffer);
			flag = 0;
		}
	}
	close(fp);
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

	save_time ("Czas wykonania zadania 2 za pomocą funkcji systemowych");

	free (file_path);

	return 0;
}