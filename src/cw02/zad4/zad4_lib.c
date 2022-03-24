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

void find_replace (char* src_file_path, char* res_file_path, char* old_string, char* new_string)
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
	char* res_line = calloc(400, sizeof(char));
	int len_new = strlen(new_string);
	int len_old = strlen(old_string);
	while (fgets(buffer, 260, src) != NULL)
	{
		char* actual = buffer;
		char* resl = res_line;
		char* place;
		while (1)
		{
			place = strstr(actual, old_string);
			if (place == NULL)
				break;
			int k = (place - actual);
			strncpy (resl, actual, k);
			resl+=k;
			strncpy (resl, new_string, len_new);
			resl+=len_new;
			actual += (k+len_old);
		}
		strcpy (resl, actual);
		fputs (res_line, res);
	}
	fclose (src);
	fclose (res);
	free (buffer);
	free (res_line);
}


int main (int argc, char **argv)
{
	if (argc < 5)
	{
		printf("%s\n", "Nie podano wystarczającej liczby argumentów");
		exit (1);
	}

	c_start = times(&start);
	find_replace (argv[1], argv[2], argv[3], argv[4]);
	c_end = times(&end);

	save_time ("Czas wykonania zadania 4 za pomocą funkcji bibliotecznych");

	return 0;
}